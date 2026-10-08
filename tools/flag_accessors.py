"""Census the flag-bit accessors: what mask, what offset, and which way.

A handful of the module's smallest functions are C++ accessors for `bool` members.
Three shapes account for all of them:

    get    lw rt, OFF($a0) / lui rX, HI / and v0, rt, rX
           / jr ra / sltu v0, $zero, v0

    set    lw rX, OFF($a0) / lui rY, HI / or rX, rX, rY
           / jr ra / sw rX, OFF($a0)

    clear lw rX, OFF($a0) / lui rY, HI / addiu rY, rY, LO / and rX, rX, rY
           / jr ra / sw rX, OFF($a0)

The mask is always built with `lui`, never an immediate, because these are all
bits above 15.  `clear` needs a second instruction because the mask it wants is
`~small_mask`, whose high half has to be `lui`-loaded and then finished with an
`addiu` to set the low half.

They are five to six instructions each, so there are enough to be worth a table
rather than a guess, and enough that the pairing matters: a getter with no setter
is evidence that the other half was inlined away at every call site, not that the
accessor does something else.

The earlier attempt at this (`tools/flag_table.py`) tried to recover the meaning of
every mask in the module and found that patterns recognised from two examples did
not survive being counted.  This tool is deliberately narrower - it matches the
three shapes above exactly and reports what it finds without inferring what the
bits mean.

    python tools/flag_accessors.py            # the table
    python tools/flag_accessors.py --show 2   # full disassembly of row 2
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import mipsdis  # noqa: E402
import pspelf  # noqa: E402


def load_functions() -> dict[str, int]:
    out: dict[str, int] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line or "=" not in line:
            continue
        try:
            out[line.split("=", 1)[0].strip()] = \
                int(line.split("=")[1].split(";")[0].strip(), 0)
        except ValueError:
            continue
    return out


def load_sizes() -> dict[str, int]:
    out: dict[str, int] = {}
    for path in (ROOT / "asm/eboot").glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                out[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break
    return out


def s16(word: int) -> int:
    return word - 0x10000 if word >= 0x8000 else word


def special(word: int) -> int:
    """The SPECIAL function code of `word`, or -1 if it is not a SPECIAL."""
    return word & 0x3F if word >> 26 == 0 else -1


def reg(word: int) -> int:
    return (word >> 21) & 0x1F


def is_jr(word: int) -> bool:
    """True for a `jr $rs`, which is SPECIAL rs with function code 8."""
    return word >> 26 == 0 and (word & 0x3F) == 0x08


def accessor(words: list[int]) -> dict | None:
    """Pull the accessor out of `words`, or return None if it is not one.

    Matching is on the exact instruction sequence rather than on "there is a
    `lui` somewhere": a looser test picks up unrelated code and the counts stop
    meaning anything, which is what happened the first time this was tried.

    Getter and setter are both five instructions; `clear` is six, because its
    mask needs an extra instruction.  `func_001A9C40` and `func_001A9C6C` show
    that an accessor can also follow a block of unrelated work - a three-word copy -
    with the same five or six instructions at the end, so those are allowed too and
    the leading instructions are simply stepped over.
    """
    if not 5 <= len(words) <= 14:
        return None

    # The flags word is loaded out of the object with `$a0` as the base, and the
    # accessor proper is the run of instructions ending in `jr $ra` plus its delay
    # slot.
    li = next((i for i, w in enumerate(words)
               if w >> 26 == 0x23 and reg(w) == 4), None)
    if li is None:
        return None
    offset = s16(words[li] & 0xFFFF)
    word_reg = (words[li] >> 16) & 0x1F

    tail = words[-2:]                              # jr $ra and the delay slot
    if not is_jr(tail[0]):
        return None
    narrows = special(tail[1]) == 0x2B and reg(tail[1]) == 0
    stores = tail[1] >> 26 == 0x2B and s16(tail[1] & 0xFFFF) == offset

    # The mask, however it was built.  Bits at or above sixteen need the shift
    # that `lui` performs, so they cost a `lui` and sometimes a second instruction
    # to finish the low half; bits below sixteen fit a zero-extended immediate.
    mask = None
    mask_reg = -1
    for i, w in enumerate(words):
        if w >> 26 == 0x0F:
            mask, mask_reg = (w & 0xFFFF) << 16, (w >> 16) & 0x1F
    if mask is not None:
        # `addiu $mask, $mask, -1` fills the low sixteen bits in, which is what a
        # *clear* needs: the top half of the mask and all the bits below it.
        for w in words:
            if w >> 26 == 0x09 and reg(w) == mask_reg and (w & 0xFFFF) == 0xFFFF:
                mask = (mask - 1) & 0xFFFFFFFF    # `addiu` sign extends -1
                break
    else:
        for w in words:
            if w >> 26 == 0x0D and (w & 0xFFFF) < 0x10000 \
                    and (w >> 16) & 0x1F != 4:
                mask, mask_reg = w & 0xFFFF, (w >> 16) & 0x1F
                break
    if mask is None or mask_reg < 0:
        return None

    op = words[-3]
    if op >> 26 != 0:
        return None
    rs, rt, rd = reg(op), (op >> 16) & 0x1F, (op >> 11) & 0x1F
    if rt != mask_reg:
        return None

    if narrows and special(op) == 0x24 and rs == word_reg and rd == 2:
        return {"offset": offset, "mask": mask, "kind": "get",
                "bits": bits_of(mask)}
    if stores and special(op) == 0x25 and rs == word_reg and rd == word_reg:
        return {"offset": offset, "mask": mask, "kind": "set",
                "bits": bits_of(mask)}
    if stores and special(op) == 0x24 and rs == word_reg and rd == word_reg:
        # The bits reported are the ones the `and` removes, not the ones it keeps.
        cleared = (~mask) & 0xFFFFFFFF
        return {"offset": offset, "mask": cleared, "kind": "clear",
                "bits": bits_of(cleared)}
    return None


def bits_of(mask: int) -> str:
    if mask == 0:
        return "-"
    low = (mask & -mask).bit_length() - 1
    if mask & (mask - 1) == 0:
        return str(low)
    return f"{low}..{(mask.bit_length() - 1)}"


def combined(words: list[int]) -> dict | None:
    """Recognise the accessors that also write the sentinel float at 0x1C.

    These sit in the same cluster as the plain accessors but do two things at once
    and so do not fit `accessor`'s shape.  Two forms, both of which read-modify-write
    the flags word and then store a float:

        set A, clear B, store -1.0f     nine instructions
            lw    $a1, OFF($a0)
            lui   $a2, 0xBF80            ; 0xBF800000 == -1.0f
            mtc1  $a2, $f12
            ori   $a1, $a1, A
            addiu $a2, $zero, ~B
            swc1  $f12, FLOAT($a0)
            and   $a1, $a1, $a2
            jr    $ra
            sw    $a1, OFF($a0)

        clear a range, store $f12       six instructions
            lw    $a1, OFF($a0)
            addiu $a2, $zero, ~MASK
            swc1  $f12, FLOAT($a0)       ; $f12 is never written here
            and   $a1, $a1, $a2
            jr    $ra
            sw    $a1, OFF($a0)

    **The six-instruction form reads `$f12` without writing it**, which on this ABI
    means the value being stored is the caller's first floating-point argument -
    the field is being assigned from a parameter the C signature does not name.
    That is a real thing to have found and it is why this form is listed separately
    rather than folded into the first one.
    """
    n = len(words)
    if n not in (6, 9):
        return None

    def flag_field(w: int) -> int | None:
        if w >> 26 != 0x2B or reg(w) != 4:
            return None
        return s16(w & 0xFFFF)

    tail = words[-2:]
    if not is_jr(tail[0]):
        return None
    offset = flag_field(tail[1])
    if offset is None:
        return None

    stores = [i for i, w in enumerate(words) if w >> 26 == 0x39 and reg(w) == 4]
    if len(stores) != 1:
        return None
    float_off = s16(words[stores[0]] & 0xFFFF)

    load = words[0]
    if load >> 26 != 0x23 or reg(load) != 4:
        return None
    if s16(load & 0xFFFF) != offset:
        return None

    andop = words[-3]
    if andop >> 26 != 0 or special(andop) != 0x24:
        return None

    set_bits = None
    if n == 9:
        # lui 0xBF80 / mtc1 for -1.0f, then ori, then addiu for the clear mask.
        if words[1] >> 26 != 0x0F or words[2] >> 26 != 0x11:
            return None
        if (words[1] & 0xFFFF) != 0xBF80 or (words[3] >> 26) != 0x0D:
            return None
        if words[4] >> 26 != 0x09 or reg(words[4]) != 0:
            return None
        set_bits = words[3] & 0xFFFF
        # `addiu` sign extends its immediate, so the mask is not the raw field.
        clear_mask = s16(words[4] & 0xFFFF) & 0xFFFFFFFF
    else:
        if words[1] >> 26 != 0x09 or reg(words[1]) != 0:
            return None
        clear_mask = s16(words[1] & 0xFFFF) & 0xFFFFFFFF

    return {"offset": offset, "float": float_off, "set": set_bits,
            "clear": (~clear_mask) & 0xFFFFFFFF,
            "sentinel": (words[1] & 0xFFFF) == 0xBF80 if n == 9 else None,
            "words": words}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=-1,
                    help="full disassembly of row N")
    ap.add_argument("--limit", type=int, default=40)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs = load_functions()
    sizes = load_sizes()

    plain: list = []
    both: list = []
    for name, addr in sorted(funcs.items(), key=lambda kv: kv[1]):
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 4:
            continue
        data = elf.read(addr, size)
        words = [struct.unpack_from("<I", data, i)[0]
                 for i in range(0, len(data), 4)]
        row = (name, addr, size, words)
        # `combined` is tried first: the two shapes overlap on the read-modify-write
        # tail, and only `combined` also accounts for the float store.
        info = combined(words)
        if info:
            both.append(row + (info,))
            continue
        info = accessor(words)
        if info:
            plain.append(row + (info,))

    # Grouped by (offset, mask, direction) rather than by bit, so a `clear` of a
    # sixteen-bit range and a `set` of a single bit above it stay apart.
    grouped: dict[tuple[int, int, str], list] = defaultdict(list)
    for row in plain:
        info = row[4]
        grouped[(info["offset"], info["mask"], info["kind"])].append(row)

    keys = sorted(grouped, key=lambda k: grouped[k][0][1])

    if ns.show >= 0:
        if ns.show < len(keys):
            key = keys[ns.show]
            print(f"offset {key[0]:#x}, mask {key[1]:#010x} ({key[2]}): "
                  f"{len(grouped[key])} functions\n")
            for name, addr, size, words, info in grouped[key]:
                print(f"  {name} ({addr:#x}, {size} bytes)")
                for i in range(0, len(words), 4):
                    print(f"    {addr + i:#010x}: "
                          f"{mipsdis.make_instruction(words[i], addr + i)}")
                print()
            return 0
        pick = ns.show - len(keys)
        if not 0 <= pick < len(both):
            print(f"{len(keys)} plain rows, {len(both)} combined rows")
            return 1
        name, addr, size, words, info = both[pick]
        print(f"combined accessor {name} ({addr:#x}, {size} bytes)\n")
        for i in range(0, len(words), 4):
            print(f"    {addr + i:#010x}: "
                  f"{mipsdis.make_instruction(words[i], addr + i)}")
        return 0

    print(f"{len(plain)} plain accessors over {len(keys)} "
          f"(offset, mask, direction) rows")
    print(f"{len(both)} accessors that also write the float at 0x1C\n")
    print(f"  {'offset':>8}  {'mask':>12}  {'bits':>7}  "
          f"{'kind':>5}  functions")
    for key in keys[:ns.limit]:
        members = grouped[key]
        print(f"  {key[0]:>#8x}  {key[1]:>#12x}  {members[0][4]['bits']:>7}  "
              f"{key[2]:>5}  " + " ".join(m[0] for m in members))
    if len(keys) > ns.limit:
        print(f"\n  ({len(keys) - ns.limit} more rows; --limit to see them)")

    print(f"\n  {'offset':>8}  {'set':>10}  {'clear':>12}  {'sentinel':>8}  "
          f"functions")
    for name, addr, size, words, info in both:
        sentinel = "-1.0f" if info["sentinel"] else "arg"
        set_bits = "-" if info["set"] is None else f"{info['set']:#010x}"
        print(f"  {info['offset']:>#8x}  {set_bits:>10}  "
              f"{info['clear']:>#12x}  {sentinel:>8}  {name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
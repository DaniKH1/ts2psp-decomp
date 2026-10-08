"""Which functions share a data address, and what stride do they walk it with?

Between 0x0F924C and 0x0F9590 there is a cluster of nine functions that all build
the same address the same way - `lui`/`addiu` to 0x0EB850, then index it - and store
into it.  Half of them scale an index with a pair of shifts and a subtract
(`index << 5 - index << 2`, which is 28); half walk it with `addiu $p, $p, 0x1C` in
a loop's delay slot.

Two things are worth counting here and neither is a "family" in the sense that has
failed twice already in this project:

*   **How many functions materialise the same address.**  That is a property of
    one pair of instructions and is worth exactly as much as it says.
*   **Whether the address is data.**  For this cluster it is not - 0x0EB850 is
    sixteen bytes into `func_000EB840`'s prologue and carries two `jal`
    relocations, so the module has nine functions writing into its own code.  The
    census prints that verdict rather than leaving it to be assumed.

The earlier version of this tool filtered on the *shape* - a shift pair, or an
`addiu` in a delay slot - and matched 1,231 functions over 819 address pairs.  That
is "addressing is common", not a family, and it is the same error as
`tools/flag_table.py`'s.  Filtering on the address is both tighter and the thing
actually being counted.

    python tools/stride_table.py             # every shared address
    python tools/stride_table.py --show 3    # full disassembly of one group
    python tools/stride_table.py --why      # what the module holds at that address
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


def base_of(words: list[int]) -> list[tuple[int, int]]:
    """Every address a `lui` + `addiu` pair builds, in order.

    The two halves need not be adjacent - three of the cluster put `sw $s3`,
    `ori $count, $zero, N` or a `move` between them - so the rule is simply that
    some later `addiu` adds the low half back into the register the `lui` named.
    Anything else in between is the compiler's business.
    """
    out = []
    for i, w in enumerate(words):
        if w >> 26 != 0x0F:
            continue
        target = (w >> 16) & 0x1F
        for j in range(i + 1, len(words)):
            nxt = words[j]
            if nxt >> 26 != 0x09 or ((nxt >> 21) & 0x1F) != target \
                    or ((nxt >> 16) & 0x1F) != target:
                continue
            lo = nxt & 0xFFFF
            if lo >= 0x8000:
                lo -= 0x10000            # the immediate is sign extended
            out.append((i, ((w & 0xFFFF) << 16) + lo))
            break
    return out


def scaled_stride(words: list[int]) -> int | None:
    """`index << k1 - index << k2`, which is how the module spells a small stride."""
    shifts = [((w >> 11) & 0x1F, (w >> 16) & 0x1F, (w >> 6) & 0x1F)
              for w in words if w >> 26 == 0 and (w & 0x3F) == 0x00]
    subs = [w for w in words if w >> 26 == 0 and (w & 0x3F) == 0x21]
    if len(shifts) != 2 or len(subs) != 1:
        return None
    sub = subs[0]
    used = [s for s in shifts if s[0] in ((sub >> 21) & 0x1F, (sub >> 16) & 0x1F)]
    if len(used) != 2 or len({u[1] for u in used}) != 1:
        return None
    amounts = sorted(u[2] for u in used)
    if amounts[0] == amounts[1]:
        return None
    return (1 << amounts[1]) - (1 << amounts[0])


def walked_stride(words: list[int]) -> int | None:
    """`addiu $p, $p, k` sitting in a delay slot, i.e. a loop's pointer step."""
    for i in range(1, len(words)):
        w, prev = words[i], words[i - 1]
        if w >> 26 != 0x09 or prev >> 26 not in (1, 2, 3, 4, 5, 6, 7, 0x11):
            continue
        rs, rt = (w >> 21) & 0x1F, (w >> 16) & 0x1F
        if rs != rt or rs == 0:
            continue
        step = w & 0xFFFF
        if step >= 0x8000:
            step -= 0x10000
        if step > 0:
            return step
    return None


def verdict(base: int, elf, sizes, enclosing) -> str:
    """`code`, `data`, or `immediate` - and the distinction matters.

    Most of what this tool finds is not an address at all.  A `lui` + `addiu` pair
    whose result lands inside no section is a 32-bit constant, and the module's
    four-character chunk tags are built exactly that way - "surf" is
    `lui 0x6672 / addiu 0x7573` - which is why the biggest group in the census is
    the tags rather than any one array.
    """
    env = enclosing(base)
    if env and base < env[0] + sizes.get(env[1], 0):
        return "code"
    for s in elf.sections:
        if getattr(s, "addr", 0) and \
                getattr(s, "addr", 0) <= base < getattr(s, "addr", 0) + \
                getattr(s, "size", 0):
            return "data"
    return "immediate"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=-1,
                    help="full disassembly of group N")
    ap.add_argument("--why", type=lambda s: int(s, 0), default=None,
                    metavar="ADDR",
                    help="what the module holds at ADDR")
    ap.add_argument("--min", type=int, default=2,
                    help="only addresses shared by at least this many functions")
    ap.add_argument("--limit", type=int, default=20)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs = load_functions()
    sizes = load_sizes()
    relocs = elf.relocs()
    order = sorted((v, k) for k, v in funcs.items())

    def enclosing(addr: int) -> tuple[int, str] | None:
        prev = None
        for a, name in order:
            if a <= addr:
                prev = (a, name)
            else:
                break
        return prev

    groups: dict[int, list] = defaultdict(list)
    for addr, name in order:
        size = sizes.get(name)
        if not size or size % 4:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        scaled = scaled_stride(words)
        walked = walked_stride(words)
        steps = set()
        for w in words:
            if w >> 26 == 0x09 and ((w >> 21) & 0x1F) == ((w >> 16) & 0x1F) \
                    and ((w >> 21) & 0x1F) != 0:
                step = w & 0xFFFF
                steps.add(step - 0x10000 if step >= 0x8000 else step)
        stride = scaled if scaled else walked
        seen = set()
        for at, base in base_of(words) or []:
            # One function can materialise the same address twice; it is one use.
            if (name, base) in seen:
                continue
            seen.add((name, base))
            groups[base].append((name, addr, size, words,
                                 stride, sorted(steps)))

    keys = sorted((b for b, m in groups.items() if len(m) >= ns.min),
                  key=lambda b: (-len(groups[b]), b))

    if ns.why is not None:
        base = ns.why
        members = groups.get(base, [])
        env = enclosing(base)
        print(f"{base:#x}: {len(members)} functions materialise it; verdict "
              f"{verdict(base, elf, sizes, enclosing)}.\n")
        print("  " + " ".join(m[0] for m in members))
        if env:
            print(f"\n  it falls {base - env[0]:#x} bytes into {env[1]} "
                  f"({env[0]:#x}, {sizes.get(env[1])} bytes)")
        print()
        for i in range(0, 48, 4):
            w = struct.unpack("<I", elf.read(base + i, 4))[0]
            tag = "".join(f"  <-- reloc type {r.type} -> {r.target:#x}"
                          for r in relocs if r.offset == base + i)
            print(f"  {base + i:#010x}: "
                  f"{mipsdis.make_instruction(w, base + i)}{tag}")
        return 0

    if ns.show >= 0:
        if not 0 <= ns.show < len(keys):
            print(f"{len(keys)} groups")
            return 1
        base = keys[ns.show]
        print(f"address {base:#x}, {len(groups[base])} functions\n")
        for name, addr, size, words, stride, steps in groups[base]:
            print(f"  {name} ({addr:#x}, {size} bytes, "
                  f"indexed stride {stride if stride else '?'}, "
                  f"pointer steps {steps})")
            for i in range(0, len(words), 4):
                print(f"    {addr + i:#010x}: "
                      f"{mipsdis.make_instruction(words[i], addr + i)}")
            print()
        return 0

    total = sum(len(groups[b]) for b in keys)
    print(f"{total} function uses over {len(keys)} addresses that "
          f"{ns.min}+ functions materialise\n")
    print(f"  {'address':>10}  {'uses':>4}  {'lands in':>8}  "
          f"{'strides':>8}  functions")
    for base in keys[:ns.limit]:
        members = groups[base]
        strides = sorted({m[4] for m in members if m[4]})
        text = ("/".join(str(s) for s in strides) if len(strides) <= 2
                else f"{len(strides)} kinds")
        print(f"  {base:>#10x}  {len(members):>4}  {verdict(base, elf, sizes, enclosing):>9}  {text:>8}  "
              + " ".join(m[0] for m in members[:6])
              + (" ..." if len(members) > 6 else ""))
    if len(keys) > ns.limit:
        print(f"\n  ({len(keys) - ns.limit} more; --limit to see them)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
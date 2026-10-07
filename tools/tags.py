"""Decode the packed four-character tags the engine registers.

`func_00106D84(tag, length, alignment)` is the Elem file-format registration
call: `tag` is a 32 bit value holding four characters, which the callee unpacks
byte by byte into a C string before adding it to the chunk table.  The Sims 2
asset formats are built out of these, and they are the single best source of
real names in the binary - `indx`, `surf`, `bmsh`, `body`, ... each identify a
record inside a `.mesh`/`.anim` file.

GCC materialises the constant with a `lui`/`addiu` pair, so this walks every
static constructor in `.cplinit`, finds those pairs immediately before each
registration call, and decodes them.

    python tools/tags.py            # every tag, by address
    python tools/tags.py --by-func  # grouped by the function that registers it
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import pspelf  # noqa: E402

REGISTER = 0x00106D84        # func_00106D84: register_chunk_tag(tag, len, align)


def decode(value: int) -> str | None:
    """A four character tag, if it is printable ASCII."""
    chars = [(value >> (8 * i)) & 0xFF for i in range(4)]
    if all(0x20 <= c < 0x7F for c in chars):
        return "".join(chr(c) for c in chars)
    return None


def is_hi(word: int) -> bool:
    return word >> 26 == 0x0F


def materialised_pairs(elf, start: int, size: int):
    """Constants passed to the registration call, by forward simulation.

    A backwards window is not enough: gcc keeps several `lui`s live at once and
    a window picks up whichever register happens to be written last.  So the
    function is walked once, tracking what each GPR holds, and the value of
    `$a0` at every `jal func_00106D84` is recorded.
    """
    import rabbitizer
    from rabbitizer import InstrCategory

    category = InstrCategory.R4000ALLEGREX
    reg: dict[int, int] = {}
    out = []
    for i in range(0, size, 4):
        addr = start + i
        data = elf.read(addr, 4)
        if len(data) < 4:
            break
        word = struct.unpack("<I", data)[0]
        ins = rabbitizer.Instruction(word, category=category, vram=addr)
        name = ins.getOpcodeName()
        # `mtc1 $a0, $f13` reports $a0 as its destination GPR even though it
        # writes the coprocessor register, so these must not invalidate it.
        if name.startswith(("mtc", "mthc", "mtl", "mf")):
            continue

        if name in ("j", "jal"):
            target = (word & 0x03FFFFFF) << 2 if word >> 26 == 0x03 else None
            if target == REGISTER and 4 in reg:
                value = reg[4]
                # CodeWarrior fills the low half in the branch's delay slot,
                # which is still "after" the jal in address order, so the slot
                # has to be applied before the argument is read.
                slot = elf.read(addr + 4, 4)
                if len(slot) == 4:
                    delay = struct.unpack("<I", slot)[0]
                    op = delay >> 26
                    rs = (delay >> 21) & 0x1F
                    rt = (delay >> 16) & 0x1F
                    if op == 0x09 and rs == rt == 4 and value is not None:
                        low = delay & 0xFFFF
                        if low & 0x8000:
                            low -= 0x10000
                        value = (value + low) & 0xFFFFFFFF
                    elif op == 0x0D and rs == rt == 4 and value is not None:
                        value = (value | (delay & 0xFFFF)) & 0xFFFFFFFF
                if value is not None:
                    out.append((value, addr))
            continue

        dst = ins.getDestinationGpr()
        if dst is None:
            continue
        index = getattr(dst, "value", dst)
        if not isinstance(index, int):
            continue
        if name == "lui":
            reg[index] = (word & 0xFFFF) << 16
        elif name == "ori" and ((word >> 21) & 0x1F) == index:
            base = reg.get(index)
            reg[index] = None if base is None else (base | (word & 0xFFFF)) \
                & 0xFFFFFFFF
        elif name == "addiu" and ((word >> 21) & 0x1F) == index:
            base = reg.get(index)
            if base is None:
                reg[index] = None
            else:
                low = word & 0xFFFF
                if low & 0x8000:
                    low -= 0x10000
                reg[index] = (base + low) & 0xFFFFFFFF
        elif name == "or" and ((word >> 21) & 0x1F) == index \
                and ((word >> 16) & 0x1F) == 0:
            src = (word >> 21) & 0x1F
            reg[index] = reg.get(src)
        elif name in ("move",):
            src = ((word >> 16) & 0x1F) if name == "move" else 0
            reg[index] = reg.get(src)
        else:
            reg[index] = None
    return out


def constructor_tags(elf, start: int, size: int):
    """(tag, call address) for each registration in one constructor."""
    return materialised_pairs(elf, start, size)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--by-func", action="store_true")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    cplinit = elf.section(".cplinit")
    entries = []
    for i in range(0, len(cplinit.data), 4):
        value = struct.unpack_from("<I", cplinit.data, i)[0]
        if value:
            entries.append(value)

    sizes: dict[str, int] = {}
    for path in (ROOT / "asm/eboot").glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                sizes[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break

    names: dict[int, str] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "=" not in line:
            continue
        nm = line.split("=", 1)[0].strip()
        val = line.split("=")[1].split(";")[0].strip()
        try:
            names.setdefault(int(val, 0), nm)
        except ValueError:
            continue

    by_func: dict[str, list[str]] = {}
    total = 0
    for entry in entries:
        name = names.get(entry, f"sub_{entry:08x}")
        size = sizes.get(name)
        if not size:
            continue
        tags = []
        for tag, _ in constructor_tags(elf, entry, size):
            text = decode(tag)
            if text:
                tags.append(text)
                total += 1
        if tags:
            by_func[name] = tags

    if ns.by_func:
        print(f"{total} tags registered by {len(by_func)} constructors\n")
        for name, tags in by_func.items():
            print(f"  {name}: {', '.join(tags)}")
    else:
        seen: dict[str, int] = {}
        for tags in by_func.values():
            for t in tags:
                seen[t] = seen.get(t, 0) + 1
        print(f"{len(seen)} distinct tags, {total} registrations\n")
        for tag in sorted(seen):
            print(f"  {tag!r:<12} x{seen[tag]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
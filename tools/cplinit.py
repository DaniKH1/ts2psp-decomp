"""Map the `.cplinit` static constructor table.

PSPLINK collects every `__attribute__((constructor))` / C++ static
initialiser into `.cplinit` as a flat array of function pointers, in link
order.  That makes it the closest thing the binary has to a symbol table: it
tells us which translation units exist, in which order they are initialised,
and - because each constructor references its own strings and globals - what
each subsystem is called.

    python tools/cplinit.py                 # the table
    python tools/cplinit.py --detail        # with each constructor disassembled
    python tools/cplinit.py --strings       # with the strings each one touches
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import mipsdis  # noqa: E402
import pspelf  # noqa: E402

JAL = re.compile(r"^\s*(?:jal|j)\s+(func_[0-9A-F]{8}|\w+)\s*$")
JAL_TARGET = re.compile(r"\bjal\s+([\w.$]+)")


def load_symbols() -> dict[int, str]:
    out: dict[int, str] = {}
    path = ROOT / "config/eboot.symbol_addrs.txt"
    for line in path.read_text(encoding="utf-8").splitlines():
        if "=" not in line:
            continue
        name = line.split("=", 1)[0].strip()
        value = line.split("=")[1].split(";")[0].strip()
        try:
            out.setdefault(int(value, 0), name)
        except ValueError:
            continue
    return out


def load_sizes() -> dict[str, int]:
    out: dict[str, int] = {}
    asm_dir = ROOT / "asm/eboot"
    for path in asm_dir.glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                out[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break
    return out


def read_cstring(elf, addr: int, limit: int = 64) -> str | None:
    data = elf.read(addr, limit)
    end = data.find(b"\x00")
    if end <= 0:
        return None
    raw = data[:end]
    if not all(32 <= b < 127 for b in raw):
        return None
    return raw.decode("ascii")


def strings_referenced_by(elf, start: int, size: int) -> list[str]:
    """The string literals a function points at, in order and deduplicated.

    Reading the relocation table is the reliable way to do this: the `%hi`/`%lo`
    pairs have already been recombined into exact targets by tools/pspelf.py, so
    each entry is simply the string that lives there.
    """
    found: list[str] = []
    seen: set[int] = set()
    for rel in elf.relocs():
        if not (start <= rel.offset < start + size):
            continue
        if rel.target in seen:
            continue
        text = read_cstring(elf, rel.target)
        if text and len(text) > 2:
            seen.add(rel.target)
            found.append(text)
    return found


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--detail", action="store_true",
                    help="disassemble each constructor")
    ap.add_argument("--strings", action="store_true",
                    help="list the strings each constructor references")
    ap.add_argument("--limit", type=int, default=0,
                    help="only show the first N entries")
    ns = ap.parse_args()

    elf = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    names = load_symbols()
    sizes = load_sizes()

    cplinit = elf.section(".cplinit")
    entries = []
    for i in range(0, len(cplinit.data), 4):
        value = struct.unpack_from("<I", cplinit.data, i)[0]
        if value:
            entries.append((i + 8, value))

    if ns.limit:
        entries = entries[:ns.limit]
    print(f".cplinit: {len(entries)} static constructors "
          f"(table at {cplinit.addr:#x}, {len(cplinit.data) // 4} slots)")

    for slot, addr in entries:
        name = names.get(addr, "?")
        size = sizes.get(name)
        calls: list[str] = []
        if size:
            for off in range(addr, addr + size, 4):
                word = struct.unpack("<I", elf.read(off, 4))[0]
                if word >> 26 == 0x03:            # jal
                    index = (word & 0x03FFFFFF) << 2
                    target = names.get(index, f"sub_{index:08x}")
                    if target not in calls:
                        calls.append(target)
        print(f"  [{slot:>#6x}] {addr:#010x} {name:<20} "
              f"{'size=0x%x ' % size if size else ''}"
              f"calls {', '.join(calls[:6]) if calls else '-'}")
        if ns.detail and size:
            for off in range(addr, addr + size, 4):
                data = elf.read(off, 4)
                if len(data) < 4:
                    break
                word = struct.unpack("<I", data)[0]
                print(f"        {off:#010x}: {mipsdis.make_instruction(word, off)}")
        if ns.strings and size:
            for text in strings_referenced_by(elf, addr, size):
                print(f"        {text!r}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
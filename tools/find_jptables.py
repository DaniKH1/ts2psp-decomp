"""Locate jump tables and floating point constants in the EBOOT .text.

spimdisasm emits `lui $fp, %hi(.Leboot_XXXXXXXX)` for MIPS jump tables, which
need a target label in .rodata.  This tool finds the tables and emits the
labels so the generated asm links.
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
import mipsdis  # noqa: E402

LUI = 0x0F
JAL = 0x03
JR = 0x00


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--elf", default=str(ELF_PATH))
    ns = ap.parse_args()

    elf = pspelf.load(ns.elf)
    text = elf.section(".text")
    words = [struct.unpack("<I", text.data[i:i + 4])[0]
             for i in range(0, len(text.data), 4)]

    # `lui $r, hi` followed shortly by `addiu $r, $r, lo` or a load/store off
    # $r is how gcc materialises an address; a jump table is one whose low half
    # points into .rodata and whose instructions index into it with a computed
    # offset (`jr $r` on an indexed value).
    lui_regs: dict[int, int] = {}
    tables = []
    for i, w in enumerate(words):
        op = w >> 26
        rs = (w >> 21) & 0x1F
        rt = (w >> 16) & 0x1F
        if op == LUI:
            lui_regs[rt] = (w & 0xFFFF) << 16
        elif op == 0x09 and rs == rt and rs in lui_regs:      # addiu $r,$r,lo
            lo = w & 0xFFFF
            addr = (lui_regs[rs] + (lo - 0x10000 if lo & 0x8000 else lo)) \
                & 0xFFFFFFFF
            tables.append((text.addr + i * 4, rs, addr))
            lui_regs.pop(rs, None)
        elif op in (0x23, 0x2B) and rs in lui_regs:            # lw/sw $x,lo($r)
            lo = w & 0xFFFF
            addr = (lui_regs[rs] + (lo - 0x10000 if lo & 0x8000 else lo)) \
                & 0xFFFFFFFF
            tables.append((text.addr + i * 4, rs, addr))
            lui_regs.pop(rs, None)

    rodata = elf.section(".rodata")
    data = elf.section(".data")
    ranges = []
    for sec in (rodata, data):
        if sec:
            ranges.append((sec.name, sec.addr, sec.addr + sec.size))

    print(f"{len(tables)} address materialisations")
    inside = [(v, a) for v, _, a in tables
              if any(lo <= a < hi for _, lo, hi in ranges)]
    print(f"{len(inside)} land inside .rodata/.data")
    for vaddr, addr in sorted(inside)[:20]:
        sec = next(n for n, lo, hi in ranges if lo <= addr < hi)
        print(f"  .text+{vaddr:#x} -> {addr:#x} ({sec}+{addr - next(lo for _, lo, _ in ranges):#x})")
    return 0


if __name__ == "__main__":
    sys.exit(main())

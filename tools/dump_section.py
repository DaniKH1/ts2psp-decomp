"""Dump the PSPLINK generated structures (library stubs, NID table, module info)."""

import struct
import sys

sys.path.insert(0, "tools")
import pspelf  # noqa: E402

elf = pspelf.load("disks/pgs-si2/EBOOT.dec")


def hexdump(sec, limit=None):
    data = sec.data[:limit] if limit else sec.data
    for i in range(0, len(data), 16):
        chunk = data[i:i + 16]
        hexs = " ".join(f"{b:02x}" for b in chunk)
        txt = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        print(f"{sec.addr + i:08X}  {hexs:<47}  {txt}")


for name in sys.argv[1:]:
    sec = elf.section(name)
    if sec is None:
        print(f"-- {name}: not found")
        continue
    print(f"-- {name} addr={sec.addr:#x} size={sec.size:#x}")
    hexdump(sec)
    print()

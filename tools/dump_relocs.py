"""Inspect relocation entries of the Sims 2 PSP EBOOT."""

import struct
import sys

sys.path.insert(0, "tools")
import pspelf  # noqa: E402

elf = pspelf.load(sys.argv[1] if len(sys.argv) > 1 else "disks/pgs-si2/EBOOT.dec")

for sec in elf.sections:
    if sec.name in (".rel.text", ".rel.rodata", ".rel.data"):
        print(f"--- {sec.name} size={sec.size:#x} entsize={sec.entsize}")
        for i in range(0, min(len(sec.data), 96), sec.entsize):
            off, info = struct.unpack_from("<II", sec.data, i)
            r_sym = info >> 8
            r_type = info & 0xFF
            word = elf.read(off, 4)
            print(f"  off={off:#010x} sym={r_sym:<3} type={r_type:<3} "
                  f"data={word.hex() if word else '-'}")

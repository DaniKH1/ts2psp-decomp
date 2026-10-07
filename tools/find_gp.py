"""Locate the `_gp` setup so the linker script and objdiff agree with the
original module."""

import struct
import sys

sys.path.insert(0, "tools")
import pspelf  # noqa: E402

elf = pspelf.load("disks/pgs-si2/EBOOT.dec")


def scan(section):
    data = section.data
    hits = []
    for i in range(0, len(data) - 4, 4):
        w = struct.unpack_from("<I", data, i)[0]
        if (w & 0xFFFF0000) == 0x3C1C0000:  # lui $gp, hi
            hits.append((section.addr + i, w & 0xFFFF))
    return hits


for sec in elf.sections:
    if sec.is_text and sec.size:
        hits = scan(sec)
        for addr, hi in hits:
            # look at the next few instructions for `addiu $gp, $gp, lo`
            words = [struct.unpack("<I", elf.read(addr + k * 4, 4))[0]
                     for k in range(1, 6)]
            print(f"{addr:#010x} lui $gp, {hi:#06x}   next: "
                  + " ".join(f"{w:08x}" for w in words))

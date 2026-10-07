"""One-off ELF section dump used while bootstrapping the project."""

import sys
from io import BytesIO

from elftools.elf.elffile import ELFFile

raw = open(sys.argv[1], "rb").read()
ef = ELFFile(BytesIO(raw))
for i, s in enumerate(ef.iter_sections()):
    h = s.header
    t = h["sh_type"]
    print(f"{i:3} {s.name:<34} {str(t):<24} addr={h['sh_addr']:#010x} "
          f"off={h['sh_offset']:#010x} size={h['sh_size']:#010x} "
          f"ent={h['sh_entsize']:#x} link={h['sh_link']} info={h['sh_info']}")

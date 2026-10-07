"""Check that every data blob in the splat config maps to an ELF section."""

import io
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402

elf = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
cfg = io.open(ROOT / "config/eboot.splat.yaml", encoding="utf-8").read()


def section_at(offset: int):
    for sec in elf.sections:
        if sec.size and sec.offset <= offset < sec.offset + sec.size:
            return sec.name
    return None


count = 0
for line in cfg.splitlines():
    blob = re.match(r"^  - \{start: (0x[0-9A-Fa-f]+), type: bin, "
                    r"name: '([^']+)'\}", line)
    if blob:
        count += 1
        offset = int(blob.group(1), 16)
        print(f"{offset:#x} -> {section_at(offset)!s:<28} {blob.group(2)}")
print("total blobs", count)

"""Compare the ELF headers and program headers of the original and the build.

Section *contents* matching is what the decompilation is judged on, but the
module header matters too: PSP's loader reads the program headers to decide what
to map, and a build whose segments differ would not boot even if the bytes were
right.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import pspelf  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ELF_PATH))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ns = ap.parse_args()

    a = pspelf.load(ns.target)
    b = pspelf.load(ns.built)

    print(f"{'ELF header':<16}{'original':<28}{'built':<28}status")
    for key in ("e_type", "e_machine", "e_flags", "e_phnum", "e_shnum"):
        va, vb = a.header[key], b.header[key]
        same = "same" if va == vb else "differs"
        print(f"{key:<16}{str(va):<28}{str(vb):<28}{same}")

    print()
    print(f"{'segment':<10}{'field':<10}{'original':<20}{'built':<20}status")
    for i, (s1, s2) in enumerate(zip(a.segments, b.segments)):
        for key in ("type", "vaddr", "filesz", "memsz", "flags"):
            va, vb = s1[key], s2[key]
            hexed = isinstance(va, int)
            fmt = "{:#x}" if hexed else "{}"
            same = "same" if va == vb else "differs"
            print(f"seg{i:<7}{key:<10}{fmt.format(va):<20}"
                  f"{fmt.format(vb):<20}{same}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

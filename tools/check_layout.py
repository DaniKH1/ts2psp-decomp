"""Report per-section address/offset deltas in both the original and the build.

For a PSP module the mapping from module address to file offset is constant
(`offset = vaddr + sizeof(ELF header)`), so any section whose delta differs is
where the linker inserted or dropped padding.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402


def deltas(elf) -> dict[str, int]:
    out = {}
    for sec in elf.sections:
        if sec.type == 8 or not sec.size:
            continue
        out[sec.name] = sec.offset - sec.addr
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ns = ap.parse_args()

    a = deltas(pspelf.load(ns.target))
    b = deltas(pspelf.load(ns.built))

    print(f"{'section':<34}{'original':>10}{'built':>10}  status")
    for name in list(a) + [n for n in b if n not in a]:
        da, db = a.get(name), b.get(name)
        same = "ok" if da == db else "MISMATCH"
        print(f"{name:<34}{('-' if da is None else hex(da)):>10}"
              f"{('-' if db is None else hex(db)):>10}  {same}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

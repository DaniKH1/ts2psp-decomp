"""Compare the built module with the original byte for byte.

`tools/build.py --diff` compares section *contents*, which is the useful check
while the C sources are being written: it tells you which section is wrong
without caring where the linker put it.  It cannot see a layout error, though -
every section can be correct while the file bytes after the last one are shifted,
which is exactly what happened to the four padding bytes in front of the second
`PT_LOAD`.  This checks the module image itself, contiguous from the end of the
ELF header to the end of the last file-backed section, and reports the first
difference.

    python tools/check_image.py
    python tools/check_image.py --first 40
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

EHDR_SIZE = 0x34 + 2 * 32        # Elf32_Ehdr plus the two Elf32_Phdr entries


def image_range(elf) -> tuple[int, int]:
    """The contiguous file range the module loader actually reads."""
    start = min(s["offset"] for s in elf.segments)
    end = max(s["offset"] + s["filesz"] for s in elf.segments)
    return start, end


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ELF_PATH))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ap.add_argument("--first", type=int, default=20,
                    help="how many differences to list")
    ns = ap.parse_args()

    want = pspelf.load(ns.target)
    got = pspelf.load(ns.built)

    w_start, w_end = image_range(want)
    g_start, g_end = image_range(got)

    print(f"{'region':<22}{'original':>12}{'built':>12}")
    print(f"{'image start':<22}{hex(w_start):>12}{hex(g_start):>12}")
    print(f"{'image end':<22}{hex(w_end):>12}{hex(g_end):>12}")

    problems = []
    if (w_start, w_end) != (g_start, g_end):
        problems.append(f"image range differs: {hex(w_start)}-{hex(w_end)} vs "
                        f"{hex(g_start)}-{hex(g_end)}")

    for i, (w, g) in enumerate(zip(want.segments, got.segments)):
        for field in ("offset", "vaddr", "filesz", "memsz", "flags", "align"):
            if w[field] != g[field]:
                problems.append(
                    f"seg{i} p_{field}: {hex(w[field])} vs {hex(g[field])}")

    a = Path(ns.target).read_bytes()
    b = Path(ns.built).read_bytes()
    lo = min(w_start, g_start)
    hi = max(w_end, g_end)
    diff = [i for i in range(lo, min(hi, len(a), len(b)))
            if a[i] != b[i]]

    total = hi - lo
    print(f"\n{total - len(diff)}/{total} image bytes identical "
          f"({100.0 * (total - len(diff)) / max(total, 1):.2f}%)")

    for p in problems:
        print(f"  {p}", file=sys.stderr)

    if diff:
        print(f"  {len(diff)} byte(s) differ, first at {diff[0]:#x}")
        for i in diff[:ns.first]:
            print(f"    {i:#010x}: {a[i]:#04x} vs {b[i]:#04x}")

    if diff or problems:
        return 1
    print("  module image is byte identical")
    return 0


if __name__ == "__main__":
    sys.exit(main())

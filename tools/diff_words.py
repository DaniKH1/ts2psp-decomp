"""Show the first N differing words between the original and the built ELF.

    python tools/diff_words.py --addr 0x1BD8CC --count 20
    python tools/diff_words.py --function func_00042430
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import mipsdis  # noqa: E402
import pspelf  # noqa: E402


def find_function(elf, name: str) -> int:
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if line.split("=", 1)[0].strip() == name:
            return int(line.split("=")[1].split(";")[0].strip(), 0)
    raise SystemExit(f"no such function: {name}")


def function_end(elf, addr: int) -> int:
    best = addr + 4
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line:
            continue
        value = int(line.split("=")[1].split(";")[0].strip(), 0)
        if addr < value < best:
            best = value
    return best


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ap.add_argument("--addr", type=lambda s: int(s, 0))
    ap.add_argument("--function")
    ap.add_argument("--count", type=int, default=24)
    ap.add_argument("--words", action="store_true")
    ns = ap.parse_args()

    want = pspelf.load(ns.target)
    got = pspelf.load(ns.built)

    start = ns.addr if ns.addr is not None else find_function(want, ns.function)
    end = start + ns.count * 4
    if ns.function and ns.addr is None:
        end = min(function_end(want, start), start + ns.count * 4)

    print(f"{'addr':<10} {'original':<38} {'built':<38}")
    for off in range(start, end, 4):
        a = want.read(off, 4)
        b = got.read(off, 4)
        aw = struct.unpack("<I", a)[0] if len(a) == 4 else 0
        bw = struct.unpack("<I", b)[0] if len(b) == 4 else 0
        if ns.words:
            ta, tb = f"{aw:08x}", f"{bw:08x}"
        else:
            ta = str(mipsdis.make_instruction(aw, off)) if aw else "-"
            tb = str(mipsdis.make_instruction(bw, off)) if bw else "-"
        flag = "" if aw == bw else "   <-- differs"
        print(f"{off:<10x} {ta:<38} {tb:<38}{flag}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

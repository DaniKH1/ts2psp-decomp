"""Rank functions by how likely psp-gcc is to reproduce them exactly.

The retail EBOOT was built with CodeWarrior, whose instruction selection psp-gcc
does not match in general (see progress.md).  It is not hopeless everywhere,
though: the two compilers agree whenever the code has no choice to make.  This
scores every function on that, so the time spent hunting for C that matches is
spent on the ones that can.

What makes a function easy:

* **No floats.**  CodeWarrior materialises float constants with `lui` + `mtc1`
  and uses `$f12`..`$f15` for temporaries; GCC never does.  A float function is
  almost certainly unreproducible.
* **Short.**  Every extra instruction is another opportunity to schedule or
  register-allocate differently.
* **Straight line.**  Branches mean block ordering, and the two compilers lay
  blocks out differently.
* **Few callees.**  A call fixes an argument order and a delay slot, but more
  calls mean more chances to differ in prologue shape.

    python tools/c_candidates.py                # the shortlist
    python tools/c_candidates.py --limit 60
    python tools/c_candidates.py --worst 20     # what not to bother with
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402

# Opcodes that touch the floating point unit.  mt*/mf* move between the FPU and
# the integer registers; the rest are the arithmetic themselves.
FP_OPCODES = {
    0x11,                     # cop1
}
FP_FUNCS = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x0B, 0x0D, 0x0E, 0x0F}


def load_functions() -> dict[str, int]:
    out: dict[str, int] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line or "=" not in line:
            continue
        name = line.split("=", 1)[0].strip()
        try:
            out[name] = int(line.split("=")[1].split(";")[0].strip(), 0)
        except ValueError:
            continue
    return out


def load_sizes() -> dict[str, int]:
    out: dict[str, int] = {}
    for path in (ROOT / "asm/eboot").glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                out[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break
    return out


def profile(elf, start: int, size: int) -> dict:
    """Instruction mix of one function."""
    data = elf.read(start, size)
    fp = calls = branches = words = 0
    for i in range(0, len(data) - 3, 4):
        word = struct.unpack_from("<I", data, i)[0]
        words += 1
        op = word >> 26
        if op in FP_OPCODES and ((word >> 21) & 0x1F) in FP_FUNCS:
            fp += 1
        elif op == 0x03:                       # jal
            calls += 1
        elif op in (0x02, 0x03, 0x04, 0x05, 0x06, 0x07) or \
                (word & 0x3F000000) == 0x10000000:   # beq/bne/blez/bgtz...
            branches += 1
    return {"words": words, "fp": fp, "calls": calls, "branches": branches}


def score(p: dict) -> float:
    """Lower is more promising."""
    return (p["fp"] * 1000.0
            + p["branches"] * 8.0
            + p["calls"] * 3.0
            + p["words"] * 0.1)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--limit", type=int, default=50)
    ap.add_argument("--worst", action="store_true",
                    help="list the least promising instead")
    ap.add_argument("--max-words", type=int, default=0)
    ap.add_argument("--min-words", type=int, default=4,
                    help="skip the two-instruction import stubs")
    ap.add_argument("--stubs", action="store_true",
                    help="include the PSP import stubs")
    ns = ap.parse_args()

    elf = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    funcs = load_functions()
    sizes = load_sizes()

    rows = []
    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size:
            continue
        if not ns.stubs and size // 4 < ns.min_words:
            continue
        if ns.max_words and size // 4 > ns.max_words:
            continue
        p = profile(elf, addr, size)
        if ns.worst == (p["fp"] == 0):
            continue
        rows.append((score(p), name, addr, size, p))

    rows.sort(reverse=ns.worst)
    rows = rows[:ns.limit]
    print(f"{'score':>8}  {'name':<24}{'size':>6}  mix")
    for s, name, addr, size, p in rows:
        print(f"{s:>8.1f}  {name:<24}{size:>6}  "
              f"{p['words']}w {p['fp']}fp {p['calls']}call {p['branches']}br")
    return 0


if __name__ == "__main__":
    sys.exit(main())
"""How much of a float function's body is stack traffic?

The module has a few functions whose arithmetic is a handful of instructions and
whose body is almost entirely loads from and stores to the frame: the two vector
lerps at 0x197414 and 0x1AF16C, and the 4x4 matrix scale at 0xC3470, which at
sixteen elements is 632 bytes.  All three are transcribed as machine code because
psp-gcc will not produce the spills.

Before writing that down as a property of "float functions" it is worth counting how
many there are, and the count corrects the impression: **the three this project
transcribed are at the mild end of it.**  They sit at 6 stores / 6 loads, 8 / 8 and
48 / 40, while the worst function in the module is at 4 / 28 across 2,028 bytes.  The
three were found by the work queue, not by being the extreme cases.

The filter is deliberately permissive - at least three float frame stores, at least
six float frame loads, and at least one floating-point arithmetic instruction - so
the number it reports is an upper bound on the shape rather than a family.

    python tools/spill_frame.py             # every qualifying function
    python tools/spill_frame.py --limit 5   # the worst five
    python tools/spill_frame.py --ratio 3   # only ones at or above this
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import mipsdis  # noqa: E402
import pspelf  # noqa: E402

from stride_table import load_functions, load_sizes  # noqa: E402

# COP1 arithmetic is one opcode (0x11) with the operation in the function field, so
# the table is keyed on that, not on the opcode.
ARITH = {0x00: "add.s", 0x01: "sub.s", 0x02: "mul.s", 0x03: "div.s",
         0x0C: "sqrt.s", 0x21: "cvt.s.w", 0x24: "cvt.w.s", 0x25: "cvt.s.l"}


def traffic(words: list[int]) -> tuple[int, int, list[str]]:
    """(float frame stores, float frame loads, arithmetic mnemonics).

    **Counting only `$sp`-based traffic misses most of the loads.**  The original
    materialises each frame offset into a register once - `addiu $a2, $sp, 0x50` -
    and then loads through *that* register, so a function can have forty-eight
    `swc1`s against `$sp` and not one `lwc1` against it.  A register formed as an
    offset from `$sp` therefore counts as frame traffic too, which is what makes
    `func_000C3470` come out as 48 stores and 48 loads rather than 48 and 0.
    """
    frame = {29}                     # $sp itself, plus...
    stores = loads = 0
    arith: list[str] = []
    for w in words:
        op = w >> 26
        if op == 0x09 and ((w >> 21) & 0x1F) == 29:
            frame.add((w >> 16) & 0x1F)      # a frame pointer, formed just now
            continue
        base = (w >> 21) & 0x1F
        if op == 0x39 and base in frame:     # swc1
            stores += 1
        elif op == 0x31 and base in frame:   # lwc1
            loads += 1
        elif op == 0x11 and (w & 0x3F) in ARITH:
            arith.append(ARITH[w & 0x3F])
    return stores, loads, arith


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--limit", type=int, default=20)
    ap.add_argument("--ratio", type=float, default=0.0,
                    help="only functions with loads/stores at or above this")
    ap.add_argument("--min-stores", type=int, default=3)
    ap.add_argument("--min-loads", type=int, default=6)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs = load_functions()
    sizes = load_sizes()

    rows = []
    for addr, name in sorted((v, k) for k, v in funcs.items()):
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 8:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        stores, loads, arith = traffic(words)
        if stores < ns.min_stores or loads < ns.min_loads or not arith:
            continue
        ratio = loads / stores
        if ratio < ns.ratio:
            continue
        rows.append((ratio, stores, loads, len(words), name, addr, size, words))

    rows.sort(key=lambda r: (-r[0], -r[3]))
    total = sum(1 for name in funcs if sizes.get(name))
    print(f"{len(rows)} functions pass more float values through the stack than "
          f"through registers, out of {total} in the module\n")
    print(f"  {'name':<24}{'bytes':>7}{'ins':>6}{'stores':>8}{'loads':>7}"
          f"{'ratio':>7}  arithmetic")
    for ratio, stores, loads, count, name, addr, size, words in rows[:ns.limit]:
        kinds = sorted(set(traffic(words)[2]))
        print(f"  {name:<24}{size:>7}{count:>6}{stores:>8}{loads:>7}"
              f"{ratio:>7.2f}  {' '.join(kinds)}")
    if len(rows) > ns.limit:
        print(f"\n  ({len(rows) - ns.limit} more; --limit to see them)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
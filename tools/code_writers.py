"""Addresses inside the code section: how many, and is any of them reachable?

A handful of functions compute an address with `lui`/`addiu` and then load or store
through it, and the address lands inside `.text`.  `tools/stride_table.py` found the
first cluster by accident; this asks the question directly, for every function.

**The count this prints is an upper bound, and the reason is worth stating.**  It
matches any function that materialises an address landing inside a *nominal* function
body, and the nominal bodies come from splat's `nonmatching` sizes, which are
"distance to the next label".  A function followed by an inline data block therefore
has a size that swallows the block, and everything in that block is reported as
"inside a function".  1,024 functions hit that test, which is not a finding - it is
the size map's limitation showing through.

**Two clusters were checked the hard way and are real**, by disassembling the
containing function to its own `jr $ra` and confirming the address falls between the
prologue and that return, with relocations in the middle of it:

    0x0e9728  12 uses  func_000E9680+0xa8
    0x0e97a8   9 uses  func_000E9780+0x28
    0x0eb850   4 uses  func_000EB840+0x10

Every function in all three is referenced by an R_MIPS_26 relocation, so none of
them is dead code.  What the writes are *for* is not established, and this tool does
not claim it is.

    python tools/code_writers.py            # every candidate
    python tools/code_writers.py --real     # only the hand-checked clusters
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import mipsdis  # noqa: E402
import pspelf  # noqa: E402

from stride_table import base_of, load_functions, load_sizes, scaled_stride  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--limit", type=int, default=20)
    ap.add_argument("--real", action="store_true",
                    help="only the clusters checked by disassembly")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs = load_functions()
    sizes = load_sizes()
    order = sorted((v, k) for k, v in funcs.items())
    text = [(s.addr, s.size) for s in elf.sections
            if getattr(s, "name", "") == ".text"]
    text_lo, text_hi = text[0][0], text[0][0] + text[0][1]

    def enclosing(addr: int) -> tuple[int, str] | None:
        prev = None
        for a, name in order:
            if a <= addr:
                prev = (a, name)
            else:
                break
        if prev and addr < prev[0] + sizes.get(prev[1], 0):
            return prev
        return None

    # Which functions are called at all?  One R_MIPS_26 relocation per call site,
    # and the target is the callee.
    called: dict[int, int] = defaultdict(int)
    for r in elf.relocs():
        if r.type == 4:
            called[r.target] += 1

    # A hit needs three things, and each is checked rather than assumed:
    #   * the function materialises a `lui`/`addiu` pair, so it is naming a symbol
    #     rather than building a constant that happens to be in range;
    #   * the value lands inside `.text` *and* inside a function's own body, which
    #     `enclosing` confirms rather than assuming from the section; and
    #   * the register holding it is then used as the base of a load or a store,
    #     which is what makes it an address rather than a constant that merely
    #     looks like one.
    def store_bases(words: list[int]) -> set[int]:
        out = set()
        for w in words:
            if w >> 26 in (0x20, 0x24, 0x28, 0x21, 0x25, 0x23, 0x2B):
                out.add((w >> 21) & 0x1F)
        return out

    clusters: dict[int, list] = defaultdict(list)
    for addr, name in order:
        size = sizes.get(name)
        if not size or size % 4:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        loads_from = store_bases(words)
        stride = scaled_stride(words)
        for at, base in base_of(words):
            if not text_lo <= base < text_hi:
                continue
            env = enclosing(base)
            if env is None:
                continue
            if (words[at] >> 16) & 0x1F not in loads_from:
                continue                  # a constant, not an address
            clusters[base].append((name, addr, stride, env,
                                   called.get(addr, 0)))

    keys = sorted(clusters, key=lambda b: -len(clusters[b]))
    if ns.real:
        # The three bases whose containing function was disassembled to its own
        # `jr $ra`, so that "inside the body" is a measurement rather than an
        # inference from splat's size map.
        checked = (0x0E9728, 0x0E97A8, 0x0EB850)
        keys = [b for b in keys if b in checked and clusters[b]]
        print("hand-checked clusters only\n")
    else:
        print(f"{sum(len(v) for v in clusters.values())} functions name an address "
              f"inside a nominal function body, at {len(clusters)} bases "
              f"(an upper bound; see the docstring)\n")
    print(f"  {'base':>10}  {'uses':>4}  {'strides':>8}  {'lands in':>22}  "
          f"{'called':>7}  functions")
    for base in keys[:ns.limit]:
        members = clusters[base]
        strides = sorted({m[2] for m in members if m[2]})
        env = members[0][3]
        reach = sum(1 for m in members if m[4])
        print(f"  {base:>#10x}  {len(members):>4}  "
              f"{('/'.join(str(s) for s in strides) or '-'):>8}  "
              f"{env[1]}+{base - env[0]:#x}  {reach:>3}/{len(members):<3}  "
              + " ".join(m[0] for m in members[:5])
              + (" ..." if len(members) > 5 else ""))
    if len(keys) > ns.limit:
        print(f"\n  ({len(keys) - ns.limit} more; --limit to see them)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
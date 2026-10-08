"""Every `svr.q` + `svl.q` pair in the module, and the offsets they store at.

`syncSkeleton_22B8` and `func_0012A9D0` both do:

    vqmul.q R100, R200, R201
    svr.q  R100, 0x0($a0)
    svl.q  R100, 0xC($a0)

Two 64-bit stores of one quad's two halves, **staggered twelve bytes apart** - which
is not the eight a contiguous four-float vector would need.  So either the
destination genuinely has a gap or overlap at offset 8, or one of these two
instructions stores at an offset the assembler adds something to.

**The bytes cannot settle it and this is written to find out whether other instances
can.**  If a third function stores the same register at different offsets, the
relationship between the two instructions and the offsets may become visible.  If
every instance is `0x0` and `0xC`, the layout is fixed and the question is one of ISA
semantics, not of this module.

    python tools/vfpu_split_store.py
    python tools/vfpu_split_store.py --show 3
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mipsdis  # noqa: E402
import pspelf  # noqa: E402
from paths import ELF_PATH  # noqa: E402

from stride_table import load_functions, load_sizes  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=0)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs, sizes = load_functions(), load_sizes()

    pairs = []
    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 4:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        for i in range(len(words) - 1):
            ops = []
            for w in (words[i], words[i + 1]):
                insn = mipsdis.make_instruction(w, addr + i * 4)
                ops.append((insn.getOpcodeName(), w & 0xFFFF))
            kinds = {k for k, _ in ops}
            if kinds == {"svr.q", "svl.q"}:
                pairs.append((name, addr, size, i, ops))
                break

    print(f"{len(pairs)} functions store one quad with an `svr.q` + `svl.q` pair\n")
    print(f"  {'name':<24}{'bytes':>7}  offsets")
    for name, addr, size, at, ops in sorted(pairs, key=lambda r: r[1]):
        text = ", ".join(f"{k} at {o:#x}" for k, o in ops)
        print(f"  {name:<24}{size:>7}  {text}")

    offsets = {tuple(o for _, o in ops) for _, _, _, _, ops in pairs}
    print(f"\n  {len(offsets)} distinct offset combination(s): "
          f"{sorted({tuple(hex(o) for o in comb) for comb in offsets})}")
    if len(offsets) == 1:
        print("  Every instance uses the same two offsets, so nothing in this module\n"
              "  disambiguates which half each store writes.  That is an ISA question.")
    if ns.show:
        print()
        for name, addr, size, at, ops in sorted(pairs, key=lambda r: r[1])[:ns.show]:
            print(f"  {name} (0x{addr:x}):")
            words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                     for i in range(0, size, 4)]
            for i in range(max(0, at - 2), min(len(words), at + 4)):
                print(f"      {addr + i*4:#08x}: "
                      f"{mipsdis.make_instruction(words[i], addr + i*4)}")
            print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
"""How much of the original naming survived into the ELF?

The decompilation has been treating names as lost.  Everything is `func_00123456`,
and the naming section of `progress.md` says the names have to be recovered by
recognising patterns in the code, with the PSP import table explicitly ruled out as
needing an outside table.  That is true of *most* symbols and false of a large
minority of them.

`config/eboot.symbol_addrs.txt` contains entries like

    sortAndCullScene_1078 = 0x1b4c94; // type:func

which is not something this project made up.  It is a symbol in the ELF's own symbol
table, produced by the original CodeWarrior link, and its type is `func`.  So the
question is not whether names exist but **how many, and of what kind**.

What the answers look like:

* **The suffixes are uniquifiers.**  Names end in `_NNNN` where `NNNN` is a hex VRAM
  address, because two objects with the same C identifier get different addresses and
  PSPLINK disambiguated them that way.  `sortAndCullScene_1078` and
  `sortAndCullScene_1844` are one function name, linked twice.
* **Some names are source paths.**  `str_c_ad_clean_sims_psp_src_elem_bent_circular_h`
  is `/c/AD/clean/sims_psp/src/elem/bent/circular.h` with the separators and the
  extension flattened to underscores.  The build tree's root was `c:/AD/clean` and it
  is still readable in the binary.  That is worth more than any number of transcribed
  functions: it is the project's source layout, recovered from the binary itself.

**Why only some functions have names.**  A name survives when something *outside the
same translation unit* refers to the symbol, which is what makes it worth an entry in
the linker's table.  The game was built with `-ffunction-sections`, so each TU became
its own output section - which is also where the names `collision`, `drawing`,
`renderCommon` in `config/eboot.splat.yaml` come from.  So **the named functions are
each TU's exported surface**, and the unnamed ones are the ones only their own file
calls.  That predicts the pattern and the census bears it out: the surviving names are
the interesting ones, not a random sample.

An earlier version of this tool classified by the address in the relocation *line*,
which is the address of the reference site rather than of the symbol - so every string
literal looked like it lived in `.text`.  The addresses come from the symbol map.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402
import pspelf  # noqa: E402

SYMBOLS = ROOT / "config" / "eboot.symbol_addrs.txt"
ENTRY = re.compile(r"^(\S+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*(.*)$")

# A suffix of hex digits: a VRAM address PSPLINK appended to make the name unique.
SUFFIX = re.compile(r"_[0-9A-Fa-f]{4,}$")

# Prefixes this project invented when it named things itself.
OURS = ("func_", "sym_", "D_", "L_", "l_", "d_", "__", "R_MIPS", "j_")

# A name that reads like a flattened path: several lowercase words in a row.
PATHISH = re.compile(r"(?:_[a-z]{2,}){4,}")


def load_symbols() -> list[tuple[str, int, str]]:
    out = []
    for line in SYMBOLS.read_text(encoding="utf-8", errors="replace").splitlines():
        line = line.strip()
        if not line or line.startswith("//"):
            continue
        m = ENTRY.match(line)
        if m:
            out.append((m.group(1), int(m.group(2), 0), m.group(3)))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--code", action="store_true",
                    help="only the names that land in a code section")
    ap.add_argument("--paths", action="store_true",
                    help="only names that look like flattened source paths")
    ap.add_argument("--section", action="store_true",
                    help="group the surviving names by section")
    ap.add_argument("--limit", type=int, default=60)
    args = ap.parse_args()

    syms = load_symbols()
    real = [(n, a, k) for n, a, k in syms if not n.startswith(OURS)]
    print(f"{len(syms)} symbols in the map")
    print(f"  {len(syms) - len(real)} synthesised by this project")
    print(f"  {len(real)} with an original name from the CodeWarrior link\n")

    collapsed = {SUFFIX.sub("", n) for n, _, _ in real}
    suffixed = [n for n, _, _ in real if SUFFIX.search(n)]
    print(f"  {len(suffixed)} end in a _NNNN VRAM suffix; those collapse to "
          f"{len(collapsed)} distinct names")

    elf = pspelf.load(str(ELF_PATH))

    def is_code(addr: int) -> bool:
        sec = elf.section_of(addr)
        return sec is not None and sec.is_text

    if args.paths:
        hits = [(n, a) for n, a, _ in real if PATHISH.search(n)]
        print(f"\n  {len(hits)} names look like flattened paths or long composites:")
        for n, a in hits[:args.limit]:
            print(f"    0x{a:08x}  {n}")
        return 0

    if args.section:
        kinds = collections.Counter()
        for n, a, _ in real:
            sec = elf.section_of(a)
            kinds[sec.name if sec else "(unmapped)"] += 1
        print("\n  by section:")
        for sec, count in kinds.most_common(args.limit):
            print(f"    {count:>5}  {sec}")
        return 0

    if args.code:
        code = [(n, a) for n, a, _ in real if is_code(a)]
        names = collections.Counter(SUFFIX.sub("", n) for n, _ in code)
        print(f"\n  {len(code)} of them land in a code section")
        print(f"  {len(names)} distinct names once the suffixes are dropped")
        print(f"  (out of {len(syms)} symbols, of which "
              f"{len(syms) - len(real)} are this project's own)\n")
        for name, count in names.most_common(args.limit):
            print(f"    {count:>3}x  {name}")
        return 0

    kinds = collections.Counter()
    for n, a, _ in real:
        sec = elf.section_of(a)
        kinds[sec.name if sec else "(unmapped)"] += 1
    print("\n  by section:")
    for sec, count in kinds.most_common(15):
        print(f"    {count:>5}  {sec}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
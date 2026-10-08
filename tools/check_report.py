"""Does every function the report writes about actually have a file?

`progress.md` claims functions are byte-exact; `src/eboot` is where that claim is
cashable.  Nothing cross-checked the two, and the gap was real: the section on the
two vector lerps said both "matched on the first attempt" and were "transcribed
that way", and neither file was in the tree.  A report entry is not a file.

This walks the backtick-quoted symbol names out of the report and the README and
lists the ones with no `src/eboot/<name>.c`.  It deliberately does *not* try to
decide whether the mention was a claim of byte-exactness - plenty of mentions are
of functions that are known *not* to be done (`func_001129E0`, the 223 import
stubs, the whole of `src/eboot` itself) - so it prints the line and lets a reader
decide.  That is the difference between a check and a guess.

    python tools/check_report.py           # names with no file
    python tools/check_report.py --all     # every mention, with a yes/no column
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src" / "eboot"
DOCS = ["progress.md", "README.md"]

# `func_000133A4`, `syncSkeleton_27D0`, `collision_1210`: the shapes of symbol
# names this project uses.  Deliberately tight, so that prose like `func_XXXXXXX`
# and mentions of directories do not turn up as symbols.
SYMBOL = re.compile(r"`((?:func|sym)_[0-9A-F]{6,8}|[A-Za-z][A-Za-z0-9]*_[0-9A-F]{4,6})`")


def mentions() -> dict[str, list[tuple[str, int, str]]]:
    out: dict[str, list[tuple[str, int, str]]] = {}
    for doc in DOCS:
        path = ROOT / doc
        if not path.exists():
            continue
        for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(),
                                      1):
            for name in SYMBOL.findall(line):
                out.setdefault(name, []).append((doc, lineno, line.strip()))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--all", action="store_true",
                    help="list every mention, not only the missing ones")
    ns = ap.parse_args()

    have = {p.stem for p in SRC.glob("*.c")}
    found = mentions()

    missing = {n: m for n, m in found.items() if n not in have}
    if ns.all:
        for name in sorted(found):
            mark = "ok     " if name in have else "MISSING"
            doc, lineno, _ = found[name][0]
            print(f"  {mark} {name:<20} {doc}:{lineno}")
    else:
        for name in sorted(missing):
            for doc, lineno, line in missing[name]:
                print(f"  {name}  {doc}:{lineno}\n      {line}")

    print(f"\n{len(found)} symbols mentioned, {len(found) - len(missing)} have a "
          f"file, {len(missing)} do not")
    if missing:
        print("  (a mention is not always a claim - check the line before acting)")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
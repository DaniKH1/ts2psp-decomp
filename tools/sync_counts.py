"""Sync the function counts in progress.md and README.md from the generated data.

These numbers have drifted twice because each batch patch hardcoded them.  Read them
from `config/matched_c.txt` (written by `verify_c.py --adopt`) and from the file count
in `src/eboot/`, and rewrite every place they appear.

Run after `verify_c.py --adopt` and `split.py --build`:

    python tools/sync_counts.py            # report what would change
    python tools/sync_counts.py --write    # write it
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MATCHED = ROOT / "config" / "matched_c.txt"
SRC = ROOT / "src" / "eboot"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true")
    ns = ap.parse_args()

    matched = len([l for l in MATCHED.read_text(encoding="utf-8").splitlines()
                   if l.strip() and not l.startswith("//")])
    written = len(list(SRC.glob("*.c")))
    unmatched = written - matched
    print(f"config/matched_c.txt: {matched}")
    print(f"src/eboot/*.c:         {written}")
    print(f"documented, unmatched: {unmatched}\n")

    subs = [
        (r"\| \*\*C functions that byte-match\*\* \| \*\*(\d+)\*\*",
         f"| **C functions that byte-match** | **{matched}**"),
        (r"\| functions written in C \| (\d+) \(see below\) \|",
         f"| functions written in C | {written} (see below) |"),
        (r"\| C functions byte-exact and hand written \| (\d+) \|",
         f"| C functions byte-exact and hand written | {matched} |"),
        (r"(\d+) of the \d+ files here are verified",
         f"{matched} of the {written} files here are verified"),
        (r"the other (\d+) are attempts",
         f"the other {unmatched} are attempts"),
        (r"and it is partial\.\*\*  \d+ of 7,497 functions\.",
         f"and it is partial.**  {matched} of 7,497 functions."),
        (r"proprietary compiler; \d+ were reached without it",
         f"proprietary compiler; {matched} were reached without it"),
        (r"What those \d+ have in common",
         f"What those {matched} have in common"),
    ]

    for name in ("progress.md", "README.md"):
        path = ROOT / name
        text = path.read_text(encoding="utf-8")
        before = text
        for pattern, replacement in subs:
            text = re.sub(pattern, replacement, text)
        if text == before:
            print(f"{name}: already in step")
            continue
        print(f"{name}: {sum(1 for a, b in zip(before.splitlines(),
                                               text.splitlines()) if a != b)} "
              f"line(s) would change")
        if ns.write:
            path.write_text(text, encoding="utf-8", newline="")
    if ns.write:
        print("\nwritten")
    return 0


if __name__ == "__main__":
    sys.exit(main())
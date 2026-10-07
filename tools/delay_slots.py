#!/usr/bin/env python3
"""Group functions by what sits in the return's delay slot.

Nearly every function here ends `jr $ra` plus one more instruction, and that
instruction is the one worth grouping by: it is where psp-gcc and CodeWarrior
disagree most often, because scheduling into a delay slot is a choice rather than
a requirement.  Functions with the same delay slot are usually the same problem,
so they are the natural unit of work.

    python tools/delay_slots.py              # the summary
    python tools/delay_slots.py --top 20     # biggest groups
    python tools/delay_slots.py --group "or $v0, $a0, $zero"
    python tools/delay_slots.py --solved      # only what is not done yet

A `*` marks a function whose C already matches byte for byte, so the count in
the summary is how much of each group is left.

The grouping turns out to be worth more than a to-do list.  The largest group is
not a pattern at all: 2955 functions teardown a 0x20-byte stack frame in their
delay slot, and the *size* is the key, so this is effectively a census of how much
stack each function needs - 0x10 for a leaf with two spilled temporaries, 0x60 for
one that saved six arguments.  That is free structural information about 2955
functions nobody has looked at yet.
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from paths import ROOT  # noqa: E402

ASM = ROOT / "asm" / "eboot"
MATCHED = ROOT / "config" / "matched_c.txt"

LABEL = re.compile(r"^glabel (\w+)")
INSN = re.compile(r"/\*[^*]*\*/\s+(\S+)\s*(.*)$")


def load_solved() -> set[str]:
    """The functions `verify_c.py --adopt` has promoted."""
    if not MATCHED.exists():
        return set()
    return {
        line.split()[0]
        for line in MATCHED.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.startswith("//")
    }


def read_functions(path: Path) -> dict[str, list[tuple[str, str]]]:
    """Split one `.s` file into its glabel blocks, as (mnemonic, operands)."""
    out: dict[str, list[tuple[str, str]]] = {}
    current: str | None = None
    for raw in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = LABEL.match(raw)
        if m:
            current = m.group(1)
            out[current] = []
            continue
        if current:
            mi = INSN.search(raw)
            if mi:
                out[current].append((mi.group(1), " ".join(mi.group(2).split())))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--top", type=int, default=12, help="how many groups to list")
    ap.add_argument("--group", help="show only this exact delay slot")
    ap.add_argument("--solved", action="store_true",
                    help="hide functions whose C already matches")
    ns = ap.parse_args()

    solved = load_solved()
    groups: dict[str, list[str]] = defaultdict(list)

    for path in sorted(ASM.glob("*.s")):
        for name, insns in read_functions(path).items():
            if ns.solved and name in solved:
                continue
            # The delay slot is the instruction *after* the jr.
            if len(insns) < 2:
                groups["(no return)"].append(name)
                continue
            (jop, jops), (dop, dops) = insns[-2], insns[-1]
            if jop != "jr" or jops != "$ra":
                groups["(no trailing return)"].append(name)
                continue
            groups[f"{dop} {dops}"].append(name)

    total = sum(len(v) for v in groups.values())
    done = sum(1 for v in groups.values() for n in v if n in solved)

    print(f"{total} functions, {done} solved, {total - done} left")
    print(f"{len(groups)} distinct delay slots\n")

    if ns.group:
        names = sorted(groups.get(ns.group, []))
        print(f'"{ns.group}": {len(names)} functions')
        for name in names[:ns.top]:
            print(f"  {'*' if name in solved else ' '} {name}")
        return 0

    ranked = sorted(groups.items(), key=lambda kv: -len(kv[1]))
    for slot, names in ranked[:ns.top]:
        mark = "*" if all(n in solved for n in names) else " "
        left = sum(1 for n in names if n not in solved)
        print(f"{mark} {len(names):4d} ({left} left)  {slot}")
        if len(names) <= 6:
            for name in sorted(names):
                print(f"         {'*' if name in solved else ' '} {name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
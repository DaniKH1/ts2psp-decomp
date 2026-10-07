"""How many functions are a straight word copy?

`func_00103E88` and `func_000EE56C` are the same rotation at two sizes - six words and
sixteen - and both needed the same three things: the source offset folded into the
pointer in asm, three named working registers, and the final load spelled out because
gcc notices it duplicates the previous one.

The census answers how many there are, and the answer is **two**.  That is the finding,
and it is the opposite of what the pair suggested: an earlier note in progress.md
called this a family, on the strength of having seen both in the shapes queue, and two
out of 7,497 is not a family.  A codebase that copies structures inline would have
hundreds.  This one almost never does - it copies through pointers, or field by field,
or through the generated copy constructors that `-ffunction-sections` left behind.

So the recipe is worth writing down twice over and there is nothing to generalise from
it.  The size histogram is still printed, because it is what says the rotation is in
groups of three: the number of stores at the tail with no load after them is the word
count mod 3.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM = ROOT / "asm" / "eboot"

ADJUST = re.compile(r"^addiu\s+\$a0,\s*\$a0,\s*(0x[0-9A-Fa-f]+|-?\d+)$")
LOAD = re.compile(r"^lw\s+\$(\w+),\s*(0x[0-9A-Fa-f]+)\(\$a0\)$")
STORE = re.compile(r"^sw\s+\$(\w+),\s*(0x[0-9A-Fa-f]+)\(\$a1\)$")


def instructions(name: str) -> list[str]:
    text = (ASM / f"{name}.s").read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
    out = []
    for line in chunk.splitlines():
        if "/*" not in line:
            continue
        instr = line.split("*/", 1)[-1].split(";", 1)[0].strip()
        if instr:
            out.append(instr)
    return out


def classify(body: list[str]) -> tuple[int, int] | None:
    """(source offset, word count) if this is a pure copy, else None."""
    if len(body) < 4:
        return None
    if not ADJUST.match(body[0]):
        return None
    offset = int(ADJUST.match(body[0]).group(1), 0)

    loads = 0
    highest = 0
    stores = 0
    # Everything after the adjustment has to be a load or a store, except the return.
    for instr in body[1:]:
        if instr.startswith("jr ") or instr == "nop":
            continue
        if instr.startswith("swc1") or instr.startswith("lw"):
            pass
        m = LOAD.match(instr)
        if m:
            loads += 1
            highest = max(highest, int(m.group(2), 0) + 4)
            continue
        m = STORE.match(instr)
        if m:
            stores += 1
            continue
        return None
    if loads == 0 or loads != stores:
        return None
    return offset, loads


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--show", metavar="FUNC", help="describe one function")
    ap.add_argument("--limit", type=int, default=25)
    args = ap.parse_args()

    if args.show:
        body = instructions(args.show)
        got = classify(body)
        print(f"{args.show}: {len(body)} instructions")
        print(f"  copy: {got}" if got else "  not a pure copy")
        for instr in body:
            print(f"    {instr}")
        return 0

    found: dict[str, tuple[int, int]] = {}
    for path in sorted(ASM.glob("*.s")):
        got = classify(instructions(path.stem))
        if got:
            found[path.stem] = got

    by_size = collections.Counter(words for _, words in found.values())
    by_offset = collections.Counter(off for off, _ in found.values())

    print(f"{len(found)} functions are a pure word copy\n")
    print(f"  {'words':>6}  {'bytes':>6}  how many")
    for words, count in sorted(by_size.items()):
        print(f"  {words:>6}  {words * 4:>6}  {count}")

    print("\n  source offset, most common first:")
    for off, count in by_offset.most_common(args.limit):
        print(f"    0x{off:04x}  {count:>4}")

    print("\n  the ones already transcribed:")
    done = set()
    matched = ROOT / "config" / "matched_c.txt"
    if matched.exists():
        done = {l.split("//")[0].strip()
                for l in matched.read_text(encoding="utf-8").splitlines()}
    for name in sorted(found):
        if name in done:
            print(f"    {name}  (0x{found[name][0]:x}, {found[name][1]} words)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
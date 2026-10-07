"""How many functions allocate a frame, save registers, and return doing nothing?

`func_00025594` is `addiu $sp, $sp, -0x30`, six stores of argument registers to the
frame, then `jr $ra` with `addiu $sp, $sp, 0x30` in the delay slot.  Nothing else.  The
body is empty and the prologue survived anyway.

That happens when a function's real body was optimised away but the frame had already
been built - a function whose only remaining code was a call that got inlined to
nothing, or a body that consisted entirely of stores the compiler proved dead.  The
argument registers being the ones saved is the tell: `$a2`, `$a3`, `$t0`-`$t3` are all
live across a call, so saving them means the function *was* going to make one.

So the count says how much dead scaffolding the build left behind, which is a fact
about the compiler's inlining decisions rather than about the game.  Zero would mean the
shape does not exist; a few dozen would mean it is a shape worth recognising, because a
frame with nothing in it looks alarming until you know what it is.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM = ROOT / "asm" / "eboot"

PROLOGUE = re.compile(r"^addiu\s+\$sp,\s*\$sp,\s*-0x([0-9A-Fa-f]+)$")
EPILOGUE = re.compile(r"^addiu\s+\$sp,\s*\$sp,\s*0x([0-9A-Fa-f]+)$")
SPILL = re.compile(r"^s[wh]\s+\$(\w+),\s*0x([0-9A-Fa-f]+)\(\$sp\)$")
RETURN = re.compile(r"^jr\s+\$ra$")

# The registers a callee has to save across a call, and which are argument registers
# in o32.  Saved argument registers are the tell that this function used to make a
# call.
CALLER_SAVED = {f"$t{i}" for i in range(10)} | {f"$a{i}" for i in range(4)}
CALLEE_SAVED = {f"$s{i}" for i in range(8)} | {"$sp", "$fp", "$ra", "$gp"}


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


def classify(body: list[str]) -> tuple[int, str, list[str]] | None:
    """(frame bytes, size, saved registers) if this is an empty-bodied frame."""
    if len(body) < 4:
        return None
    pro = PROLOGUE.match(body[0])
    if not pro:
        return None
    if not RETURN.match(body[-2]) or not EPILOGUE.match(body[-1]):
        return None
    if pro.group(1).upper().lstrip("0") != body[-1].split()[-1].lstrip("0x").upper():
        # Compare numerically: -0x30 and +0x30 have to be the same size.
        if int(pro.group(1), 16) != int(body[-1].split()[-1], 16):
            return None
    saved: list[str] = []
    for instr in body[1:-2]:
        m = SPILL.match(instr)
        if not m:
            return None
        saved.append(m.group(1))
    return int(pro.group(1), 16), len(body) * 4, saved


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--show", metavar="FUNC", help="describe one function")
    args = ap.parse_args()

    if args.show:
        body = instructions(args.show)
        got = classify(body)
        print(f"{args.show}: {len(body)} instructions")
        print(f"  {got}" if got else "  not an empty-bodied frame")
        for instr in body:
            print(f"    {instr}")
        return 0

    found: dict[str, tuple[int, int, list[str]]] = {}
    for path in sorted(ASM.glob("*.s")):
        got = classify(instructions(path.stem))
        if got:
            found[path.stem] = got

    print(f"{len(found)} functions allocate a frame, save registers, and return "
          f"doing nothing\n")
    if not found:
        print("  the shape does not exist in this build")
        return 0

    frames = collections.Counter(frame for frame, _, _ in found.values())
    kinds = collections.Counter()
    for _, _, saved in found.values():
        if set(saved) & CALLER_SAVED:
            kinds["saves argument registers - it used to make a call"] += 1
        if set(saved) & CALLEE_SAVED:
            kinds["saves $ra or a callee-saved register"] += 1

    print("  frame sizes:")
    for frame, count in sorted(frames.items()):
        print(f"    0x{frame:02x}  {count}")

    print("\n  what they save:")
    for note, count in kinds.most_common():
        print(f"    {count:>4}  {note}")

    print("\n  the functions:")
    for name in sorted(found, key=lambda n: -len(found[n][2])):
        frame, size, saved = found[name]
        print(f"    {name:<18} frame 0x{frame:02x}, {len(saved)} saved")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
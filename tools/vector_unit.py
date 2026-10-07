"""Who uses the PSP's vector unit, and for what.

The vector coprocessor is the one piece of this console's hardware nothing in the
project had touched until `syncSkeleton_27D0` came up in the shapes queue.  It turns
out to be used by exactly **32 of the 7,497 functions**, which is worth knowing for
two reasons:

* it is small enough to read.  32 functions is a bounded census, not a search, so the
  claim "the renderer and the skeleton code use it and nothing else does" is
  checkable rather than a first impression;
* the named ones are the interesting ones.  `renderMeshInstances_122C` has 67 vector
  instructions and 32 functions is the whole set, so the four translation units that
  own the vector code are all in the surviving symbol names from the original link.

The opcodes also say what it is being used *for*, and not for the usual reasons:

* `vrsq.s` - reciprocal square root, eight times.  That is distance attenuation in a
  lighting calculation, done four lanes at a time.  It is the classic reason to want
  a vector unit at all, and it is only worth having if you are shading many vertices
  per frame.
* `svl.q` / `svr.q` - the swapped forms of the vector load and store, which is how the
  unit transposes.  Five of each.  A transposition is what you need to multiply two
  matrices, and `vmmul.q` appears six times, so matrix multiplication is happening.
* `vscl.t` and `vmul.t` - the `.t` suffix is a transpose on read, which is what lets
  one operand feed three separate multiplications, as in the two `syncSkeleton`
  functions.

None of that needs the ISA reference to be useful: the opcode names say the intent and
the counts say how much of it there is.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM = ROOT / "asm" / "eboot"

# Every Allegrex vector instruction.  The scalar FPU uses `lw`/`sw`/`mtc1`, so there
# is no overlap to worry about; the prefixes are what the ISA calls the vector unit.
VECTOR_OPS = (
    "lv.s", "lv.q", "sv.s", "sv.q", "svl.q", "svr.q",
    "vmov.s", "vmov.q", "vzero.s", "vzero.q", "vone.s", "vone.q",
    "vmul.s", "vmul.q", "vmul.t", "vmmul.q", "vmsub.q",
    "vadd.s", "vadd.q", "vadd.t", "vsub.s", "vsub.q", "vsub.t",
    "vdiv.s", "vmax.s", "vmin.s", "vscl.s", "vscl.t",
    "vrcp.s", "vrsq.s", "vdp4.s", "vdet.s",
    "vcft.s", "vcvt.s", "vcfs.s", "vcfy.s", "vflame.s", "vflamo.s",
)

# What each opcode is *for*, where that is not obvious from the name.  Kept short:
# this is a census, and the reasoning belongs in progress.md.
WHY = {
    "vrsq.s": "reciprocal square root - distance attenuation in lighting",
    "svl.q": "swapped load: a transpose on the way in",
    "svr.q": "swapped store: a transpose on the way out",
    "vmmul.q": "matrix multiply",
    "vscl.t": "scale by a broadcast scalar, read through the transpose file",
    "vmul.t": "multiply by a vector already transposed",
    "vadd.t": "add a vector already transposed",
}


def body(name: str) -> list[str]:
    text = (ASM / f"{name}.s").read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
    out = []
    for line in chunk.splitlines():
        if "/*" not in line:
            continue
        instr = line.split("*/", 1)[-1].split(";", 1)[0].strip()
        if instr:
            out.append(instr.split()[0])
    return out


def scan() -> list[tuple[str, list[str]]]:
    hits = []
    for path in sorted(ASM.glob("*.s")):
        ops = [o for o in body(path.stem) if o in VECTOR_OPS]
        if ops:
            hits.append((path.stem, ops))
    return hits


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--show", metavar="FUNC",
                    help="list this function's vector instructions")
    ap.add_argument("--why", action="store_true",
                    help="only the opcodes whose purpose is worth stating")
    args = ap.parse_args()

    if args.show:
        text = (ASM / f"{args.show}.s").read_text(encoding="utf-8",
                                                  errors="replace")
        chunk = text.split(f"glabel {args.show}", 1)[-1].split("endlabel")[0]
        print(f"{args.show}:")
        for line in chunk.splitlines():
            if "/*" not in line:
                continue
            instr = line.split("*/", 1)[-1].split(";", 1)[0].strip()
            if instr and instr.split()[0] in VECTOR_OPS:
                print(f"    {instr}")
        return 0

    hits = scan()
    total = sum(len(o) for _, o in hits)
    print(f"{len(hits)} functions use the vector unit, {total} vector instructions\n")

    tally = collections.Counter(op for _, ops in hits for op in ops)

    if args.why:
        for op, count in tally.most_common():
            if op in WHY:
                print(f"  {count:>4}x  {op:<10} {WHY[op]}")
        return 0

    print(f"  {'count':>5}  function")
    for name, ops in sorted(hits, key=lambda kv: -len(kv[1])):
        tag = "" if name.startswith("func_") else "   <- named"
        print(f"  {len(ops):>5}  {name}{tag}")

    print("\n  opcodes, most used first:")
    for op, count in tally.most_common():
        note = WHY.get(op, "")
        print(f"    {count:>4}x  {op:<10} {note}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
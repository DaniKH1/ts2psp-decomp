"""Which functions in the module are byte-identical to each other?

`func_000E4970` and `func_000F7D38` are the same five instructions:

    sw    $zero, 0xC($a0)
    sw    $zero, 0x10($a0)
    sw    $zero, 0x14($a0)
    jr    $ra
    move  $v0, $zero

Two entry points, one body.  That is unusual in hand-written code and ordinary in
generated or templated code, and the count is the interesting part: a handful of pairs
is what a large codebase does by accident, and hundreds would mean the module has a
code generator in it.

This is deliberately not the same question `tools/copy_family.py` asks.  That one looks
for *repeated sequences inside* one function, which is about the compiler's
strength-reduction.  This one compares whole function bodies, which is about the
program's authorship.

It also excludes two things that would otherwise dominate the answer:

  * the 223 PSP import stubs, which are all `jr $ra` because they are linker-generated
    placeholders and the names were lost, so their sameness means nothing;
  * zero-length and 4-byte bodies, where a single instruction has too little entropy to
    be evidence of anything.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config" / "matched_c.txt"

LABEL = re.compile(r"^glabel\s+(\S+)")
END = re.compile(r"^endlabel")
SIZE = re.compile(r"\.size\s+\S+,\s*(-?0x[0-9A-Fa-f]+|-?\d+)", re.M)


def sizes() -> dict[str, int]:
    """Function name -> byte size, from the symbol table split.py maintains."""
    out: dict[str, int] = {}
    if not CONFIG.exists():
        return out
    for line in CONFIG.read_text(encoding="utf-8").splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue
        try:
            out[parts[0]] = int(parts[1], 0)
        except ValueError:
            continue
    return out


# spimdisasm writes each instruction as
#
#     /* <section addr> <virtual addr> <instruction word> */  <mnemonic> ...
#
# so the original bytes are in the third hex field of the comment.  Taking them from
# there rather than re-assembling the mnemonics is the whole point: the comparison has
# to be on the bytes the module actually contains, not on this project's spelling of
# them.
WORD = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s*\*/")


def bodies(asm_dir: pathlib.Path) -> dict[str, bytes]:
    """Function name -> its raw bytes, read out of the .s files' instruction words."""
    out: dict[str, bytes] = {}
    for path in sorted(asm_dir.glob("*.s")):
        text = path.read_text(encoding="utf-8", errors="replace")
        name = path.stem
        chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel", 1)[0]
        words = [m.group(1) for line in chunk.splitlines()
                 if (m := WORD.search(line))]
        out[name] = bytes.fromhex("".join(words)) if words else b""
    return out


def is_stub(name: str) -> bool:
    """The linker-generated import placeholders.

    All 223 PSP imports are named `stub` by the original link and all of them are
    `jr $ra`, so any group containing them measures the linker rather than the
    programmer.  They are excluded from the counts and counted separately.
    """
    return name.startswith("stub_") or name.startswith("_imp") or "_stub" in name


def instructions_of(path: pathlib.Path, name: str) -> list[str]:
    """The mnemonics spimdisasm wrote, one per line.

    Reading the disassembly rather than decoding the raw words is deliberate.  The
    words in these files are printed byte-swapped (`jr $ra` comes out as `0800E003`),
    so any decoder has to know that, and hand-decoding o32 encodings is exactly the
    kind of thing that goes quietly wrong.  spimdisasm already did it.
    """
    text = path.read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel", 1)[0]
    out = []
    for line in chunk.splitlines():
        if not WORD.search(line):
            continue
        out.append(line.split("*/", 1)[1].strip() if "*/" in line else "")
    return out


def describe(asm_dir: pathlib.Path, name: str) -> str:
    """What an 8-byte body does, in words.

    An 8-byte body is `jr $ra` plus one instruction, so the second instruction is the
    whole of the function's behaviour.  Naming the cases rather than reporting the raw
    word matters: without it the census reads as a hundred opaque hex constants.
    """
    body = instructions_of(asm_dir / f"{name}.s", name)
    if len(body) != 2 or not body[0].startswith("jr"):
        return f"is not a return: {' / '.join(body)}"
    last = " ".join(body[1].split())
    if last == "nop":
        return "returns nothing (void)"
    if last in ("or $v0, $zero, $zero", "move $v0, $zero"):
        return "returns 0"
    if last.startswith("ori $v0, $zero,"):
        return f"returns {last.rsplit(',', 1)[1].strip()}"
    if last.startswith(("or $v0, $a", "move $v0, $a", "addiu $v0, $a")):
        return f"returns {last.split(',')[1].strip()} unchanged"
    return f"returns something built by: {last}"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--min-size", type=lambda s: int(s, 0), default=8,
                    help="ignore bodies smaller than this (default 8)")
    ap.add_argument("--show", metavar="N", default="40",
                    help="how many duplicate groups to print (default 40)")
    ap.add_argument("--include-stubs", action="store_true",
                    help="also count the linker-generated import placeholders")
    ap.add_argument("--body", metavar="FUNC", help="print one function's instructions")
    args = ap.parse_args()

    asm_dir = ROOT / "asm" / "eboot"
    if not asm_dir.is_dir():
        print(f"no {asm_dir}; run tools/split.py first", file=sys.stderr)
        return 1

    raw = bodies(asm_dir)
    total = len(raw)

    if args.body:
        print(f"{args.body}: {len(raw[args.body])} bytes")
        for line in instructions_of(asm_dir / f"{args.body}.s", args.body):
            print(f"    {line}")
        return 0

    groups: dict[str, list[str]] = collections.defaultdict(list)
    skipped_small = 0
    stubs = 0
    for name, data in raw.items():
        if not data:
            continue
        if is_stub(name):
            stubs += 1
            if not args.include_stubs:
                continue
        if len(data) < args.min_size:
            skipped_small += 1
            continue
        groups[hashlib.sha1(data).hexdigest()].append(name)

    dupes = {h: names for h, names in groups.items() if len(names) > 1}
    in_dupes = sum(len(v) for v in dupes.values())

    print(f"{len(dupes)} distinct bodies are shared by more than one function; "
          f"{in_dupes} of {total} functions are byte-identical to a sibling")
    if skipped_small:
        print(f"  ({skipped_small} bodies under {args.min_size} bytes ignored)")
    if stubs and not args.include_stubs:
        print(f"  ({stubs} linker-generated import stubs excluded)")
    if not dupes:
        print("  no duplicates: every function body in the module is unique")
        return 0

    # Largest groups first: the biggest duplication is the most informative.
    ordered = sorted(dupes.values(), key=lambda v: (-len(v), v[0]))
    sizes = collections.Counter(len(v) for v in ordered)
    print("\n  group sizes:")
    for n, count in sorted(sizes.items()):
        print(f"    {count:>4} groups of {n}")

    print(f"\n  the {min(int(args.show), len(ordered))} largest groups:")
    for names in ordered[: int(args.show)]:
        data = raw[names[0]]
        shown = names if len(names) <= 5 else names[:4] + [f"...+{len(names) - 4}"]
        print(f"    {len(data):>6} bytes x{len(names):<4} {', '.join(shown)}")

    # The eight-byte groups are the interesting ones for a different reason: a body
    # that is exactly `jr $ra` plus one more instruction is a function whose entire
    # content is a return.  Counting those by what they return says more about the
    # module than any other group here.
    print("\n  bodies of exactly 8 bytes - a function that is one instruction wide:")
    returns: dict[str, list[str]] = collections.defaultdict(list)
    for names in groups.values():
        if len(raw[names[0]]) != 8:
            continue
        returns[describe(asm_dir, names[0])].extend(names)
    for what, names in sorted(returns.items(), key=lambda kv: -len(kv[1])):
        print(f"    {len(names):>4}  {what}")
    single_return = sum(len(v) for v in returns.values())
    print(f"    ----\n    {single_return} functions in total, "
          f"{single_return * 100 / total:.1f}% of the module")

    # How much of the duplication has already been decompiled once?
    done = set()
    if CONFIG.exists():
        done = {line.split()[0] for line in
                CONFIG.read_text(encoding="utf-8").splitlines() if line.split()}
    in_dupes_done = [n for names in dupes.values() for n in names if n in done]
    print(f"\n  {len(in_dupes_done)} of the {in_dupes} duplicated functions are already "
          f"transcribed in C")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
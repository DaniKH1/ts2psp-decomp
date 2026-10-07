"""Do the callers of a group of functions read the whole return value, or one byte?

`func_00151240` and 63 siblings share twenty bytes that store a zero *byte* to the
stack and then load a *word* from the same address:

    addiu $sp, $sp, -0x10
    sb    $zero, 0x0($sp)
    lw    $v0, 0x0($sp)
    jr    $ra
    addiu $sp, $sp, 0x10

Three bytes above the one that was written are stack garbage, and the function returns
them as part of its result.  The natural explanation is that the original source
declared a `char` local, assigned zero to it, and returned it from something whose
result is only ever read one byte wide - which would make the wide load legal.

That is an inference.  This tool is what turns it into a measurement: for every branch to
any member of a duplicate group, it reports what the caller does next, and in
particular whether the result is ever narrowed before use.

The reading that supports the inference is `lbu`/`lb`/`sb` on `$v0`.  A caller that does
`sw $v0, k($sp)` or `addu $v1, $v0, $zero` is consuming all four bytes and would be
carrying garbage, so the count of those is the count of places the optimisation would
actually be visible.

**It follows `$v0` forward, not just the next instruction.**  The first version of this
tool looked only at the instruction after a `jal` and got the answer badly wrong: at
`func_00017FB0` that instruction is `sb $a1, 0x39B($sp)`, which is the `jal`'s own delay
slot storing an argument and has nothing to do with the result.  Reading it as a
one-byte use of the return value would have manufactured the very evidence the tool
exists to check.

So the scan now starts *after* the delay slot and walks forward until something writes
`$v0`, classifying what used it on the way.  That is what turned up the real pattern,
which is more interesting than the one the naive scan found - see below.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM = ROOT / "asm" / "eboot"

WORD = re.compile(r"/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s*\*/")

# Instructions that read only the low byte of their source register.
NARROW = {"lbu", "lb"}
# Instructions that consume all four bytes of a source register.
WIDE = {"sw", "addu", "addiu", "and", "or", "xori", "andi", "sll", "srl", "sra", "lw"}

# Instructions whose *first* operand is read, not written.  Missing these is the third
# version of the same bug: a `beqz $v0` after the call looks like a destination, so the
# value reads as discarded when in fact it is being tested - and a zero test is the one
# use that is sensitive to the three bytes above the low one, so getting it wrong here
# is exactly what would hide a real problem.
FIRST_OPERAND_IS_SOURCE = {
    "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz",
    "mult", "multu", "div", "divu", "madd", "maddu",
    "slt", "sltu", "slti", "sltiu", "teq", "tge", "tgeu", "tlt", "tltu", "tne",
    "mov.s", "movn", "movz", "c.eq.s", "c.lt.s", "c.le.s",
    "add.s", "sub.s", "mul.s", "div.s", "abs.s", "neg.s", "sqrt.s",
}

# The load-interlocked delay slot: the instruction after a branch always executes, so it
# is not part of "what happens next" for dataflow purposes.
DELAY_SLOT = {"j", "jal", "jr", "jalr", "beq", "bne", "bgez", "bltz", "bgtz", "blez"}


def uses_v0(instr: str) -> bool:
    """True if the instruction *reads* `$v0`.

    A store's first operand is its source, so `sw $v0, 0x2C($sp)` reads `$v0` and is a
    use - which is what made the previous version of this tool report `or $a0, $s0,
    $zero` as "using all four bytes" when it never mentions `$v0` at all.

    The other half is the branches and compares, whose first operand is also a source.
    Without them `beqz $v0` reads as a *destination* and the value looks discarded, when
    in fact the caller is testing it - and testing against zero is the one use that
    *can* see the three bytes above the low one, so this is the case where being wrong
    hides a real problem rather than a cosmetic one.
    """
    if "$v0" not in instr:
        return False
    op = instr.split()[0]
    first = instr.split(",")[0]
    if op in FIRST_OPERAND_IS_SOURCE:
        return True
    return "$v0" not in first


# A store of the tracked value to a stack slot, and the read of that slot.
STORE_V0 = re.compile(r"^sw\s+\$v0,\s*(0x[0-9A-Fa-f]+)\(\$sp\)$")


def reloads(instr: str, offset: str) -> bool:
    """True if this instruction reads back the word just stored at `offset($sp)`."""
    return bool(re.search(rf"^l[bwu]?[a-z]*\s+\$\w+,\s*{re.escape(offset)}\(\$sp\)$",
                          instr))


def writes_v0(instr: str) -> bool:
    """True if the instruction writes `$v0` - which kills the value being tracked."""
    parts = instr.replace(",", " ").split()
    if not parts:
        return False
    op = parts[0]
    if op in {"lw", "lwc1", "mflo", "mfhi", "lbu", "lb", "lhu", "lh", "lwl", "lwr",
              "mul", "mfc1", "mov.s", "trunc.w.s", "round.w.s", "ceil.w.s",
              "floor.w.s", "cvt.s.w"}:
        # Destination is the first operand.
        return "$v0" in instr.split(",")[0]
    if op in {"move", "or", "addu", "addiu", "subu", "and", "andi", "xori", "xor",
              "nor", "sll", "srl", "sra", "sllv", "srlv", "srav", "negu", "seqz",
              "snez", "slt", "sltu", "slti", "sltiu", "mul"}:
        return parts[1] == "$v0" if len(parts) > 1 else False
    return False


def classify(seq: list[str]) -> str:
    """How the caller used the value the call returned.

    The window is the instructions after the call's delay slot, up to and including the
    next write to `$v0`.  Two things count as using the value:

      * reading `$v0` directly, and
      * storing it to a stack slot and reading that slot back.

    The second is not a detail.  The dominant pattern in this group is
    `sw $v0, 0x2C($sp)` followed by `lb $a0, 0x2C($sp)` - a word-wide store and a
    one-byte reload of the same address, which is the caller doing byte-for-byte what
    the callee did.  Tracking only the register would call those 142 sites "discards the
    result", which is the opposite of the truth.
    """
    direct = [i for i in seq if uses_v0(i) and not writes_v0(i)]

    # A zero test on the whole register is the one use that can observe the bytes above
    # the low one, so it is reported on its own rather than lumped in with "other".
    for i in direct:
        if i.split()[0] in ("beqz", "bnez") and "$v0" in i:
            return "tests the whole register against zero"

    # Follow the value through memory: a `sw $v0, k($sp)` and a later load of `k($sp)`.
    for i, instr in enumerate(seq):
        m = STORE_V0.match(instr)
        if not m:
            continue
        offset = m.group(1)
        for later in seq[i + 1:]:
            if reloads(later, offset):
                op = later.split()[0]
                if op in ("lb", "lbu"):
                    return "stores it and reloads one byte"
                if op == "lw":
                    return "stores it and reloads all four bytes"
                return f"stores it and reloads with {op}"
            if writes_v0(later):
                break

    if not direct:
        return "discards the result"
    ops = {i.split()[0] for i in direct}
    if ops <= {"move", "or"} and all("$zero" in i for i in direct):
        return "copies it to another register"
    if ops & NARROW:
        return "narrows it to a byte"
    if ops & WIDE:
        return f"uses all four bytes ({', '.join(sorted(ops & WIDE))})"
    return f"other: {', '.join(sorted(ops))}"


def raw(path: pathlib.Path) -> list[tuple[str, str]]:
    """(virtual address, instruction text) for one function."""
    text = path.read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {path.stem}", 1)[-1].split("endlabel", 1)[0]
    out = []
    for line in chunk.splitlines():
        m = WORD.search(line)
        if m:
            out.append((m.group(2), line.split("*/", 1)[1].strip()))
    return out


def find_group(target: str) -> list[str]:
    """Every function whose body is byte-identical to `target`'s."""
    def words(path: pathlib.Path) -> str:
        text = path.read_text(encoding="utf-8", errors="replace")
        chunk = text.split(f"glabel {path.stem}", 1)[-1].split("endlabel", 1)[0]
        return "".join(m.group(3) for m in
                       (WORD.search(l) for l in chunk.splitlines()) if m)

    want = words(ASM / f"{target}.s")
    return [p.stem for p in sorted(ASM.glob("*.s")) if words(p) == want]


def all_groups(min_size: int = 8) -> list[tuple[bytes, list[str]]]:
    """Every byte-identical body shared by more than one function, biggest first."""
    words: dict[pathlib.Path, str] = {}
    for p in sorted(ASM.glob("*.s")):
        t = p.read_text(encoding="utf-8", errors="replace")
        c = t.split(f"glabel {p.stem}", 1)[-1].split("endlabel", 1)[0]
        words[p] = "".join(m.group(3) for m in
                           (WORD.search(l) for l in c.splitlines()) if m)
    groups: dict[str, list[str]] = collections.defaultdict(list)
    for p, w in words.items():
        if w and len(w) >= min_size * 2:
            groups[w].append(p.stem)
    out = [(w, names) for w, names in groups.items() if len(names) > 1]
    out.sort(key=lambda kv: (-len(kv[1]), kv[1][0]))
    return [(bytes.fromhex(w), names) for w, names in out]


def census(args) -> int:
    """Run the width question across every duplicated body at once."""
    groups = all_groups(args.min_size)
    print(f"{len(groups)} duplicated bodies; running the width question on each\n")
    rows = []
    for data, members in groups:
        where = report(members[0], quiet=True)
        rows.append((len(data), len(members), where))
    rows.sort(key=lambda r: -r[1])
    print(f"{'bytes':>6} {'group':>6}  what the callers do")
    for size, count, where in rows[: args.show]:
        print(f"{size:>6} x{count:<4}  {where}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("function", nargs="?",
                    help="any member of the duplicate group")
    ap.add_argument("--census", action="store_true",
                    help="run the width question over every duplicated body")
    ap.add_argument("--min-size", type=lambda s: int(s, 0), default=8,
                    help="with --census, ignore bodies smaller than this")
    ap.add_argument("--show", type=int, default=30, help="rows to print with --census")
    ap.add_argument("--examples", type=int, default=6,
                    help="how many narrow and wide call sites to show")
    args = ap.parse_args()

    if args.census:
        return census(args)

    if not args.function:
        ap.print_help()
        return 1
    if not (ASM / f"{args.function}.s").exists():
        print(f"no {args.function} in asm/eboot", file=sys.stderr)
        return 1

    members = find_group(args.function)
    print(f"{args.function}'s body is shared by {len(members)} functions\n")
    if len(members) > 1:
        shown = members[:5] + ([f"...+{len(members) - 5}"] if len(members) > 5 else [])
        print(f"  members: {', '.join(shown)}")

    pattern = re.compile(r"\b(?:jal|j)\s+(" + "|".join(re.escape(m) for m in members) + r")\b")
    return scan(pattern, members, args)


def scan(pattern: re.Pattern[str], members: list[str], args) -> str:
    """The width question for one group, as a one-line summary (or a full report)."""
    tally: collections.Counter[str] = collections.Counter()
    examples: dict[str, list[str]] = collections.defaultdict(list)
    for p in sorted(ASM.glob("*.s")):
        if p.stem in members:
            continue
        seq = [instr for _, instr in raw(p)]
        for i, instr in enumerate(seq):
            m = pattern.search(instr)
            if not m:
                continue
            # Skip the branch's own delay slot: it always runs and is unrelated to
            # what the call returned.  Reading it as a use of $v0 is exactly the bug
            # this tool had before.
            start = i + 2
            window: list[str] = []
            for later in seq[start:start + 12]:
                window.append(later)
                if writes_v0(later):
                    break
            what = classify(window)
            tally[what] += 1
            if len(examples[what]) < args.examples:
                shown = " ; ".join(window[:3]) if window else "(nothing follows)"
                examples[what].append(f"{p.stem}: {m.group(1)} then {shown}")

    if not sum(tally.values()):
        if getattr(args, "quiet", False):
            return "no in-module branches"
        print("  no in-module branches to this group at all")
        return ""

    # If the callee never writes `$v0` there is no return value to be narrow about,
    # and every "discards the result" below is counting nothing.  Saying that up front is
    # the difference between a census and a list of coincidences: the 704-byte group
    # reports "32 discards the result", which sounds like a fact about 32 callers and is
    # in fact a fact about none of them.
    callee = [instr for _, instr in raw(ASM / f"{members[0]}.s")]
    produces = any(writes_v0(i) or i.startswith(("mtc1", "lwc1")) for i in callee)
    # Whether a byte-width write leaves the bytes above it stale.  This is a proxy, not a
    # proof - it looks for `sb` anywhere in the body - but it is the case that has
    # produced every interesting result so far, and being explicit about it beats
    # warning about zero tests on functions that return a whole word.
    any_partial = any(i.startswith("sb") for i in callee)
    label = "" if produces else "  [callee never writes $v0 - no return value]"

    summary = ", ".join(f"{c} {w}" for w, c in tally.most_common(3))
    if getattr(args, "quiet", False):
        return (summary or "no use of $v0") + label

    print(f"  {sum(tally.values())} in-module branches; what each caller does with `$v0`:")
    for what, count in tally.most_common():
        print(f"    {count:>4}  {what}")
    if not produces:
        print("\n  The callee never writes `$v0`, so there is no return value here at all.")
        print("  Whatever it is left in is not this function's doing, and every count")
        print("  above is a coincidence rather than a fact about the callers.")

    print("\n  the width question:")
    n_narrow = sum(c for w, c in tally.items() if "one byte" in w)
    n_wide = sum(c for w, c in tally.items() if w.startswith("uses all four"))
    n_drop = sum(c for w, c in tally.items() if w == "discards the result")
    n_test = sum(c for w, c in tally.items() if w.startswith("tests the whole"))
    print(f"    {n_narrow} call sites keep one byte of it")
    print(f"    {n_wide} call sites use all four bytes")
    print(f"    {n_test} call sites test the whole register against zero")
    print(f"    {n_drop} call sites ignore it")
    if n_wide == 0 and n_narrow:
        print("\n  No call site uses the whole word, so the three bytes above the one the")
        print("  callee wrote are never observed in-module.  That is what makes the wide")
        print("  load safe rather than a bug - and it is also why it is worth measuring")
        print("  rather than asserting.")
    if n_test and any_partial:
        print("\n  The zero tests would see any garbage in the upper bytes, since `beqz`")
        print("  looks at all thirty-two.  They are safe only if the low byte is guaranteed")
        print("  non-zero, which is a stronger requirement than 'nobody reads the value'.")
    elif n_test:
        print("\n  The zero tests are safe here: the callee writes the whole word, not one")
        print("  byte of it, so there is nothing above the low byte to be wrong.")

    for what, lines in examples.items():
        print(f"\n  [{what}]")
        for line in lines:
            print(f"    {line}")
    return 0


def report(member: str, quiet: bool = False) -> str:
    """The width question for the group `member` belongs to."""
    args = argparse.Namespace(quiet=quiet, examples=6)
    members = find_group(member)
    pattern = re.compile(r"\b(?:jal|j)\s+(" +
                         "|".join(re.escape(m) for m in members) + r")\b")
    return scan(pattern, members, args)


if __name__ == "__main__":
    raise SystemExit(main())
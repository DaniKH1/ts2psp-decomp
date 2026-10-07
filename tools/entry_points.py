"""Which `glabel`s are actually function entry points?

spimdisasm splits a function at every `jr $ra`, which is right for a normal
function and wrong for anything the compiler laid out as a shared tail.  The
symptom is easy to recognise in the body and impossible to spot from a symbol
list: **the first instruction reads a register the ABI says the caller did not
have to preserve.**

In o32, `$t0`-`$t9` are caller-saved.  A function that reads one before writing it
is reading whatever the caller happened to leave there - so either the label is not
an entry point, or it is one half of a tail call.  `$s0`-`$s7` are callee-saved and
must be restored before use, so reading one at entry is the same story with a
different excuse.

func_0014EAAC is one: `lw $t5, 0x8($t0)` as its first instruction, and nothing in
`asm/eboot` branches to it.

This is a census, not a verdict.  A label flagged here may still be a real function
reached through a jump table - those exist in the 223 PSP import stubs and possibly
elsewhere.  What it does establish is the *upper bound* on how many of the 7,503
labels can be transcribed as functions with an o32 signature, which is a number the
decompilation work depends on and has never had.
"""
from __future__ import annotations

import argparse
import pathlib
import re

ASM = pathlib.Path(__file__).resolve().parent.parent / "asm" / "eboot"

# Registers an o32 entry point may read without having written them: the incoming
# argument registers, the return register, the stack pointer, the globals pointer,
# and the constant zero.
ARG_REGS = {f"$a{i}" for i in range(4)} | {f"$v{i}" for i in range(2)}
FLOAT_REGS = {f"$f{i}" for i in range(12, 20)}
ENTRY_OK = ARG_REGS | FLOAT_REGS | {"$zero", "$ra", "$sp", "$gp", "$fp"}

# Caller-saved and callee-saved temporaries: fine to use, not fine to *read* first.
SUSPECT = {f"$t{i}" for i in range(10)} | {f"$s{i}" for i in range(8)}

# Everything an instruction can name as a destination.  Keeping this complete is what
# makes "first instruction reads X" reliable rather than approximate.
DEFINES = re.compile(
    r"^\s*(?:"
    r"add|addi|addiu|addu|and|andi|or|ori|xor|xori|sub|subu|"
    r"lui|sll|srl|sra|sllv|srlv|srav|"
    r"lb|lbu|lh|lhu|lw|lwc1|ld|lwu|"
    r"sb|sh|sw|swc1|sd|"
    r"move|li|la|not|neg|"
    r"mult|multu|div|divu|mfhi|mflo|"
    r"lui"
    r")\s+\$(\w+),"
)

OPERANDS = re.compile(r"\$([a-z0-9]+)")
REG_TOKEN = re.compile(r"\$(zero|at|v[01]|a[0-3]|t[0-9]|s[0-9]|k[01]|gp|sp|s[89]|fp|ra"
                       r"|f1[2-9]|f2[0-9]|f3[01]|hi|lo)")

# Operations with no destination: stores, and the branches.  For these the first
# operand is a *source*, not a destination.
NO_DEST = {"sw", "sb", "sh", "sdc1", "sd", "jr", "jalr", "j", "jal", "b", "beq", "bne",
           "bgez", "bltz", "bgtz", "blez", "beqz", "bnez", "beql", "bnel", "bgezal",
           "bltzal", "break"}


def instructions(name: str) -> list[str]:
    """The instruction lines of one glabel, without the leading comment block."""
    path = ASM / f"{name}.s"
    text = path.read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
    lines = []
    for line in chunk.splitlines():
        if "/*" not in line:
            continue
        # Strip the leading `/* addr word word *\/` and any trailing relocation note.
        body = line.split("*/", 1)[-1]
        body = body.split(";", 1)[0]
        lines.append(body.strip())
    return lines


def sources_of(instr: str) -> list[str]:
    """Every register the instruction reads, in order, with `($reg)` counted.

    An earlier version of this only looked at the second comma-separated operand,
    which misses the register inside every `0x8($t0)` addressing mode - and those
    are the ones this whole census is looking for.
    """
    parts = instr.split(None, 1)
    if len(parts) != 2:
        return []
    op = parts[0]
    operands = parts[1].replace("(", " ").replace(")", " ")
    regs = [f"${m}" for m in OPERANDS.findall(operands)]
    if op in NO_DEST or not regs:
        return regs
    return regs[1:]


def first_read_of_suspect(name: str) -> tuple[str, str] | None:
    """The first instruction and the suspect register it reads, if any."""
    body = instructions(name)
    if not body:
        return None
    first = body[0]
    for src in sources_of(first):
        if src in SUSPECT:
            return first, src
    return None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--show", metavar="FUNC",
                    help="print this function's first instruction and why it is flagged")
    ap.add_argument("--limit", type=int, default=40,
                    help="how many flagged labels to list (default 40)")
    args = ap.parse_args()

    names = sorted(p.stem for p in ASM.glob("*.s"))

    if args.show:
        body = instructions(args.show)
        if not body:
            print(f"{args.show}: empty body")
            return 1
        hit = first_read_of_suspect(args.show)
        print(f"{args.show}: {len(body)} instructions")
        print(f"  first: {body[0]}")
        if hit:
            print(f"  flagged: reads {hit[1]}, which the caller need not have preserved")
        else:
            print("  not flagged")
        return 0

    flagged: list[tuple[str, str, str]] = []
    for name in names:
        hit = first_read_of_suspect(name)
        if hit:
            flagged.append((name, hit[0], hit[1]))

    print(f"{len(names)} labels")
    print(f"{len(flagged)} start by reading a register the caller need not have "
          f"preserved\n")
    by_reg: dict[str, int] = {}
    for _, _, reg in flagged:
        by_reg[reg] = by_reg.get(reg, 0) + 1
    print("  register read first:")
    for reg, count in sorted(by_reg.items(), key=lambda kv: -kv[1]):
        print(f"    {reg:<6} {count}")
    print()
    print(f"  first {args.limit}:")
    for name, instr, reg in flagged[:args.limit]:
        print(f"    {name.replace('func_', '')}  {reg:<5} {instr}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
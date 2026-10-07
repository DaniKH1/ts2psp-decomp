#!/usr/bin/env python3
"""Find the smallest functions that contain a loop.

A backward branch is the signature: its target is below its own address, so
control can return to an earlier instruction.  Those are the functions where
psp-gcc has to choose a block order, which is the one thing pinning a register
cannot fix - so they are what is left to work out.

    python tools/find_loops.py              # the smallest, by byte count
    python tools/find_loops.py --top 30
    python tools/find_loops.py --size 40    # only loops in functions <= 40 bytes
    python tools/find_loops.py --show func_00112233
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from paths import ROOT  # noqa: E402

ASM = ROOT / "asm" / "eboot"
MATCHED = ROOT / "config" / "matched_c.txt"

LABEL = re.compile(r"^glabel (\w+)")
# The address each instruction sits at.  A comment is three fields - the section
# offset, the module address, the encoding - and it is the *middle* one we want:
#     /* DA0 00000D2C 1F00C010 */  beqz  $a2, .Leboot_00000DAC
ADDR = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s")
# `b*`/`j`/`beq`-style mnemonics, i.e. anything with a branch in the name.
BRANCH = re.compile(r"^(b|beq|bne|bgez|bgtz|blez|bltz|bc|bv|j|jr|jal|bal)")


def load_solved() -> set[str]:
    if not MATCHED.exists():
        return set()
    return {
        line.split()[0]
        for line in MATCHED.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.startswith("//")
    }


def read_function(path: Path, name: str) -> tuple[list[tuple[int, str, str]], set[str]]:
    """One function as (address, mnemonic, operands), plus the labels it defines.

    `asm/` writes branch targets as `.Leboot_XXXXXXXX` labels rather than as
    displacements - the displacement form only appears in `disasm_range.py`, which
    disassembles on the fly.  The address is in the label's own name, so it can be
    read straight out of it; the set of labels defined here is what stops a
    reference to a label in another function from being resolved by accident.
    """
    out: list[tuple[int, str, str]] = []
    labels: set[str] = set()
    current = None
    for raw in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = LABEL.match(raw)
        if m:
            if current is not None and current != name:
                break                 # next label: this function ended
            current = m.group(1)
            continue
        if current != name:
            continue
        md = re.match(r"^\s*(\.\w+):", raw)
        if md:
            labels.add(md.group(1))
            continue
        ma = ADDR.match(raw)
        mi = re.search(r"/\*[^*]*\*/\s+(\S+)\s*(.*)$", raw)
        if ma and mi:
            out.append((int(ma.group(1), 16), mi.group(1),
                        " ".join(mi.group(2).split())))
    return out, labels


LOCAL_LABEL = re.compile(r"^\.Leboot_([0-9A-Fa-f]{8})$")


def branch_target(here: int, operands: str) -> tuple[str | None, int | None]:
    """(label, address) for a branch, either of which may be None.

    `beqz $a2, .Leboot_00000DAC` writes the whole operand list, so the target is
    the last comma-separated field and not the first - reading the operands as
    though they were the target is the easy mistake here, and it silently
    classifies every conditional branch as "target unknown".

    Returns the label text separately so the caller can insist it is defined in
    this function; a reference to a label defined elsewhere is a jump table or a
    tail call, not a loop.
    """
    ops = operands.strip()
    fields = [f.strip() for f in ops.split(",")]
    target = fields[-1]

    if LOCAL_LABEL.match(target):
        return target, int(LOCAL_LABEL.match(target).group(1), 16)

    if target.startswith("."):
        # a displacement form, as disasm_range.py prints it
        m = re.match(r"^\.\s*\+\s*4\s*\+\s*\((0x[0-9A-Fa-f]+)\s*<<\s*2\)$", target)
        if m:
            return None, here + 4 + int(m.group(1), 16) * 4
        return None, None

    m = re.match(r"^(0x[0-9A-Fa-f]+)$", target)
    if m:
        return None, int(m.group(1), 16)
    return None, None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--top", type=int, default=20)
    ap.add_argument("--size", type=int, default=0,
                    help="only report loops in functions of at most N bytes")
    ap.add_argument("--show", help="disassemble one function")
    ns = ap.parse_args()

    if ns.show:
        insns, _ = read_function(ASM / f"{ns.show}.s", ns.show)
        for a, m, o in insns:
            print(f"  0x{a:08x}: {m:<8s} {o}")
        return 0

    solved = load_solved()
    found: list[tuple[str, int, list[int]]] = []

    for path in sorted(ASM.glob("*.s")):
        text = path.read_text(encoding="utf-8", errors="replace")
        if "glabel" not in text:
            continue
        for name in re.findall(r"^glabel (\w+)", text, re.MULTILINE):
            if name in solved or name.startswith("collision_"):
                continue
            insns, labels = read_function(path, name)
            if len(insns) < 5:
                continue
            size = (insns[-1][0] - insns[0][0]) + 4
            if ns.size and size > ns.size:
                continue
            backwards = []
            for addr, mnem, ops in insns:
                if not BRANCH.match(mnem) or mnem in ("jr", "jalr", "jal", "bal", "j"):
                    continue
                label, target = branch_target(addr, ops)
                if target is None or target > addr:
                    continue
                if label is not None and label not in labels:
                    continue
                backwards.append(addr)
            if backwards:
                found.append((name, size, backwards))

    found.sort(key=lambda t: t[1])
    print(f"{len(found)} unsolved functions contain a backward branch")
    if ns.size:
        print(f"  (of which {sum(1 for f in found if f[1] <= ns.size)} are "
              f"<= {ns.size} bytes)")
    print()
    for name, size, sites in found[:ns.top]:
        where = ", ".join(f"0x{a:06x}" for a in sites)
        print(f"  {size:5d} bytes  {len(sites)} branch(es) at {where}  {name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
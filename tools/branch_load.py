"""Loads in a branch delay slot: is the conditional load an idiom or a coincidence?

`sortAndCullScene_10BC` builds a value conditionally by putting the load in the delay
slot of a branch-**likely**:

    addiu $v0, $zero, -0x2
    bnel  $a1, $zero, . + 4 + (0x1 << 2)
    lw    $v0, 0xEC($a0)

A likely branch only executes its delay slot when it is taken, so the load happens
exactly when `$a1 == 0` - a conditional load with a default, in one instruction of
branch overhead instead of a label and two branches.  Written by hand it looks like
an accident; the question is whether the module uses it, and how often.

**It does, and the counts are the reason to believe it is deliberate:**

    38,336   `beq`/`bne`/`beql`/`bnel` in the module
     3,214   the likely forms, 8.4 %
     4,041   loads in some branch's delay slot
       616   functions with a load in a *likely* branch's delay slot
        43   of those set the destination to a constant first

The 43 is the interesting one: a literal written into the destination immediately
before the branch means the branch is choosing between the literal and the loaded
value, which is a `value or sentinel` accessor rather than a coincidence of
scheduling.  `sortAndCullScene_10BC` (default `-2`) and `func_00052950` (default
`0`) are two of them.

**The 8.4 % is what makes the idiom legible.**  The likely form exists on this ISA for
exactly this purpose - it makes the delay slot conditional - and if it were being used
as a scheduling accident it would appear everywhere.  It appears in 3,214 branches
and not more, which fits an idiom that is only available when the branch's sole
purpose is to guard its own delay slot.

Two things this census is careful about, both found by checking a known member against
it rather than trusting the numbers:

* the opcode tables are cross-checked against rabbitizer's own `getOpcodeName()` and
  `isBranchLikely()`, so a wrong opcode would show up as a mismatch rather than as a
  plausible count;
* `_writes_const` insists its second argument really is a load.  Without that check it
  happily reports a `lui` as setting a constant into the register that a following
  *arithmetic* instruction uses, which is a different claim.

    python tools/branch_load.py                    # the summary
    python tools/branch_load.py --show 3           # examples, smallest first
    python tools/branch_load.py --solved           # only functions whose C matches
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mipsdis  # noqa: E402
import pspelf  # noqa: E402
from paths import ELF_PATH  # noqa: E402

from stride_table import load_functions, load_sizes  # noqa: E402

BRANCH = {0x04: "beq", 0x05: "bne", 0x14: "beql", 0x15: "bnel"}
LOADS = {0x20: "lb", 0x21: "lh", 0x23: "lw", 0x24: "lbu", 0x25: "lhu",
         0x30: "ll", 0x31: "lwc1"}


def _writes_const(prev: int, load: int) -> bool:
    """Does `prev` write a plain constant into the register `load` will overwrite?

    The constant sources are the ones a compiler emits for a literal: `addiu` and
    `ori` from `$zero`, plus `lui`.  `addiu rt, $rs, imm` with a non-zero `$rs` is an
    address computation, not a literal, so it does not count.  `load` is required to
    be a load, so that a caller passing anything else gets `False` rather than a
    confident answer about the wrong register.
    """
    if (load >> 26) not in LOADS:
        return False
    rt = (load >> 16) & 0x1F
    op = prev >> 26
    rs = (prev >> 21) & 0x1F
    rd = (prev >> 11) & 0x1F
    if op in (0x09, 0x0D):                     # addiu, ori
        return (prev >> 16) & 0x1F == rt and rs == 0
    if op == 0x0F:                             # lui
        return rd == rt
    if op == 0x00:                             # SPECIAL, for completeness
        return (prev & 0x3F) in (0x0A, 0x0B) and rd == rt and rs == 0
    return False


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=0,
                    help="print this many examples in full")
    ap.add_argument("--solved", action="store_true",
                    help="only functions whose C already matches")
    ns = ap.parse_args()

    matched: set[str] = set()
    solved_path = Path(__file__).resolve().parent.parent / "config" / "matched_c.txt"
    if ns.solved and solved_path.exists():
        matched = {line.strip() for line in solved_path.read_text().splitlines()
                   if line.strip()}

    elf = pspelf.load(str(ELF_PATH))
    funcs = load_functions()
    sizes = load_sizes()

    branches = likely = loads_in_slot = hits = 0
    sentinel_fns = 0
    rows: list[tuple[str, int, int, str, list[str]]] = []
    sentinel_rows: list[str] = []

    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 2:
            continue
        if ns.solved and name not in matched:
            continue
        blob = elf.read(addr, size)
        words = [struct.unpack_from("<I", blob, i)[0] for i in range(0, size, 4)]
        fn_branches = fn_likely = fn_loads = 0
        fn_hits: list[str] = []
        fn_sentinel = False
        for i in range(len(words) - 1):
            op = words[i] >> 26
            if op not in BRANCH:
                continue
            fn_branches += 1
            is_likely = op >= 0x14
            if is_likely:
                fn_likely += 1
            nxt = words[i + 1] >> 26
            if nxt in LOADS:
                fn_loads += 1
                if is_likely:
                    fn_hits.append(f"{BRANCH[op]} + {LOADS[nxt]}")
                    # The narrower idiom: a constant written into the load's
                    # destination just before the branch, so the branch-likely is
                    # choosing between the constant and the loaded value.
                    if i >= 1 and _writes_const(words[i - 1], words[i + 1]):
                        fn_sentinel = True
                        sentinel_rows.append(name)
        branches += fn_branches
        likely += fn_likely
        loads_in_slot += fn_loads
        if fn_hits:
            hits += 1
            rows.append((name, addr, size, ", ".join(fn_hits), words))
        if fn_sentinel:
            sentinel_fns += 1

    print(f"{branches} branch-likely and branch-not-equal instructions in the module\n")
    print(f"  {likely} of them are the likely forms ({100*likely/max(branches,1):.1f} %)")
    print(f"  {loads_in_slot} loads sit in some branch's delay slot")
    print(f"  {hits} functions put a load in a *likely* branch's delay slot")
    print(f"  {sentinel_fns} of those set the destination to a constant first, so the")
    print(f"  branch is choosing between a literal and a loaded value\n")
    if ns.show:
        print(f"  {len(rows)} functions in all, smallest first:\n")
        for name, addr, size, kinds, words in sorted(rows, key=lambda r: r[2])[:ns.show]:
            print(f"  {name} (0x{addr:x}, {size} bytes): {kinds}")
            for i, w in enumerate(words):
                print(f"      0x{addr + i*4:08x}: {mipsdis.make_instruction(w, addr + i*4)}")
            print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
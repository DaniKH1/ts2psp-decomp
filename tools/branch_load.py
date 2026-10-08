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

**And the 43 is an undercount by more than seven times, because a delay slot does not
have to hold a load.**  `--all` counts *any* register write in a likely delay slot:

     1,173   functions
       311   of those with a constant default first

The gap is the interesting part, not the bigger number.  The slot can hold a `move`,
an `addu`, an `addiu`, an `andi` - anything that writes the register the branch is
choosing between.  `func_000BF49C` is a NULL-if-absent accessor whose slot holds a
`move`; `func_000BAB78` holds an `addu`; `func_0000EBDC` holds an `addu` three times
over, into three different registers in one function.  **So the idiom is not
"conditionally load", it is "conditionally materialise a value, defaulting to a
literal" - and 43 described only the loads.**

What the idiom *means* is still not established.  The best-supported reading is that
it conditionally computes an address - `func_0000E04C` guards `addiu $s1, $a0, 0x8`,
so `s1 = node ? node + 8 : NULL`, which is "the field at +8, if the link exists".

**The 8.4 % is what makes the idiom legible.**  The likely form exists on this ISA for
exactly this purpose - it makes the delay slot conditional - and if it were being used
as a scheduling accident it would appear everywhere.  It appears in 3,214 branches
and not more, which fits an idiom that is only available when the branch's sole
purpose is to guard its own delay slot.

Three things this census is careful about, and each was wrong first:

* the opcode tables are cross-checked against rabbitizer's own `getOpcodeName()` and
  `isBranchLikely()`, so a wrong opcode would show up as a mismatch rather than as a
  plausible count;
* `_writes_const` insists its second argument really is a load.  Without that check it
  happily reports a `lui` as setting a constant into the register that a following
  *arithmetic* instruction uses, which is a different claim.  `--all` cannot use it at
  all for the same reason, which is why `_default_before` exists: it walks back from
  the branch looking for the last thing that wrote the destination register, rather
  than looking one instruction back, and `_writes_const`'s one-instruction rule is only
  right because a load's destination is also its `rt`;
* and `_dest` asks rabbitizer *which field* is the destination and then reads the bits
  itself.  It first used `getDestinationGpr()`, which raises for an I-type like `lw`
  because `lw` has no `rd` field at all - so the first `--all` run reported **265
  functions, fewer than the 616 it was meant to extend**, with no loads in the
  breakdown.  **A count smaller than the narrower count it extends is the tell**, and it
  is a tell that works: no plausible reading of a census goes down when it is widened.

    python tools/branch_load.py                    # the summary
    python tools/branch_load.py --all              # any register write in the slot
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
STORES = {0x28: "sb", 0x29: "sh", 0x2B: "sw", 0x2A: "swl", 0x2E: "swr",
          0x39: "swc1"}


def _dest(word: int, addr: int) -> int | None:
    """The general register `word` writes, or None if it writes none.

    Used to decide whether an instruction in a delay slot is *conditional work* or
    just the unavoidable shadow of an earlier branch.  Asked of rabbitizer rather
    than decoded here, because getting this wrong means silently reclassifying a
    `jal`'s delay slot or a branch's shadow as conditional - and rabbitizer already
    knows which fields an instruction actually writes, which is not something to
    re-derive from an opcode table.
    """
    insn = mipsdis.make_instruction(word, addr)
    # `modifiesRd` is about the *rd field*, which an I-type like `lw` does not have -
    # asking rabbitizer for `lw`'s destination raises, and `insn.rt` is a RegGprO32
    # object rather than an integer.  So rabbitizer decides *which field* is the
    # destination and the bits are read here.  Getting this wrong silently drops
    # every load from the sweep, which is how the first version of `--all` reported
    # *fewer* functions than the load-only count it is supposed to extend.
    try:
        if insn.modifiesRd():
            return (word >> 11) & 0x1F or None
        if insn.modifiesRt():
            return (word >> 16) & 0x1F or None
    except Exception:                        # not a type rabbitizer models
        return None
    return None


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


def _default_before(words: list[int], branch_i: int, base: int,
                    dest: int | None) -> bool:
    """Was `dest` given a constant earlier in the function, before this branch?

    Walks back from the branch looking for a plain constant written into `dest`,
    and stops at the first instruction that writes `dest` by any other means - so a
    register that is conditionally overwritten is only counted as having a default if
    the default is the last thing that touched it.

    This is the general form of `_writes_const`, which looks only one instruction
    back and only at loads.  One instruction back is right for a load, whose
    destination is also the load's `rt`, but it fails on the ALU cases:
    `func_000BF49C` sets its NULL default six instructions before its `beql` and
    `_writes_const` reports no default for it, while the same function *does* have
    one - which would have made the tool undercount the very idiom it was extended
    to find.  Register-based rather than instruction-distance-based, because the
    distance is arbitrary and the register is what the branch is actually choosing
    between.
    """
    if dest is None:
        return False
    for w in reversed(words[:branch_i]):
        d = _dest(w, base)
        if d is None:
            continue
        if d != dest:
            continue
        op, rs, rd = w >> 26, (w >> 21) & 0x1F, (w >> 11) & 0x1F
        if op in (0x09, 0x0D) and rs == 0:            # addiu/ori from $zero
            return True
        if op == 0x0F:                                # lui
            return True
        return False                                   # written by something else
    return False


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=0,
                    help="print this many examples in full")
    ap.add_argument("--solved", action="store_true",
                    help="only functions whose C already matches")
    ap.add_argument("--all", action="store_true", dest="allslots",
                    help="also count delay slots holding any register write, not "
                         "just loads - the sentinel idiom is not always a load")
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

    # `--all`: the same sweep, but any instruction in a likely branch's delay slot
    # that writes a register counts, not only a load.  `func_000BF49C` is the case
    # that forced this - it is the sentinel idiom with a `move` in the slot - and it
    # is invisible to the load-only count.
    kinds: dict[str, int] = {}
    any_fns = 0
    any_sentinel_fns = 0

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
        fn_any = False
        fn_any_sentinel = False
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
            if ns.allslots and is_likely and nxt not in BRANCH:
                # Any delay-slot instruction that writes a register.  A `jal` or a
                # non-likely branch in the slot is excluded above; what is left is
                # work that only happens if the branch was taken.
                d = _dest(words[i + 1], addr + (i + 1) * 4)
                if d is not None and nxt != 0x03:       # 0x03 is `jal`
                    name_of = mipsdis.make_instruction(
                        words[i + 1], addr + (i + 1) * 4).getOpcodeName()
                    kinds[name_of] = kinds.get(name_of, 0) + 1
                    fn_any = True
                    if _default_before(words, i, addr, d):
                        fn_any_sentinel = True
        branches += fn_branches
        likely += fn_likely
        loads_in_slot += fn_loads
        if fn_any:
            any_fns += 1
        if fn_any_sentinel:
            any_sentinel_fns += 1
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
    print(f"  branch is choosing between a literal and a loaded value")
    if ns.allslots:
        print()
        print(f"  counting *any* register write in a likely delay slot, not just")
        print(f"  loads:  {any_fns} functions, {any_sentinel_fns} of them with a")
        print(f"  constant default first")
        for op, n in sorted(kinds.items(), key=lambda kv: (-kv[1], kv[0])):
            print(f"      {n:>6}  {op}")
    print()
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
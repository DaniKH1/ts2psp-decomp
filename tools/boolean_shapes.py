"""Two shapes the flag and boolean accessors take, counted.

Both were found by hand-transcribing a function that no tool claimed, which has
happened three times in this project now.  Both are narrow, both are checkable
against a known member, and both are reported with the count they came from.

**Shape 1: `sltiu rt, rs, 1` followed by `andi rt, rt, 0xFF`.**
`sltiu` against an immediate of 1 produces exactly 0 or 1, so the `andi` cannot change
the result.  `func_000BF49C` and `func_000FBFD4` both have it, 0xBF apart in address
and nothing else in common.

**Shape 2: a flag setter whose mask fits a halfword, so `ori rt, rs, MASK` with
`rt == rs`.**  `tools/flag_accessors.py` has `get`, `set` and `clear` and all three
build the mask with `lui`, because every mask it had found was above bit 15.  Bit 2
fits an immediate, so `ori $a1, $a1, 0x4` is a setter in four instructions instead of
five - and the census cannot see it.

    **Shape 3: a flag bit written from a boolean argument - clear then set, in one
function.**  `func_00012F14` clears bit 31 of the word at offset 0x64 with
`lui 0x8000` + `addiu -1` + `and` (which is 0x7FFFFFFF) and then ORs in
`(arg & 1) << 31`.  `tools/flag_accessors.py` finds the *getter* for the same bit at the
same offset - `func_00012F3C`, forty bytes away - and reports the pair as one orphan,
because `set`, `clear` and `combined` all describe a single operation and this is two.

The match is: a `lui` + `addiu -1` pair forming a clear mask, an `and` through it, and
an `or` whose destination is that `and`'s destination.  That is narrow on purpose - the
function also contains a *dead* `andi 0xFF` before its real `andi 0x1`, so a looser
test keyed on "masks then stores" would match every accessor twice.

    python tools/boolean_shapes.py              # all three counts
    python tools/boolean_shapes.py --show 3     # examples in full
    python tools/boolean_shapes.py --only sltiu # one shape

**The first version matched the REGIMM sub-opcode field against `9`, which is what the
standard MIPS table says `sltiu` is.  **In this assembler's encoding `sltiu $a0, $a0,
1` is 0x2c840001, where bits 25-21 and bits 20-16 are both 4** - so there is no known
member that says which half holds the sub-opcode, and testing for 9 matches nothing in
the module.  It reported zero for a shape that is in 168 functions.

The first version of the `ori` filter required the register to be `$a0` and reported
**192 functions**, which were the float-constant idiom: `lui $a0, 0x3F7D` then `ori
$a0, $a0, 0x70A4` is 0x3F7D70A4 read as a float, and there are 192 of those alone.  The
register is not reliably `$a0` either - `func_001A9B78` loads the flags into `$a1`
because `$a0` is the object pointer - and the missing condition was that the value
being preserved must have come from a *load*, not from a `lui`.  That left 18.

**Counts are per function, not per occurrence**: 267 and 31 occurrences sit in 168 and
18 functions, so several functions repeat the shape.  A function that tests eight
fields for zero is one function eight times over, and quoting the occurrence count
would have made the idiom look twice as common as it is in the places a reader would
go looking.

**The calibration is worth keeping: both shapes were found by hand-transcribing two
functions, and those two are 1.2 % and 5.6 % of their totals.**  Transcribing finds the
instances; only counting says how many there are.
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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=0)
    ap.add_argument("--only", choices=("sltiu", "ori", "clear_set", "all"),
                    default="all")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs, sizes = load_functions(), load_sizes()

    want_sltiu = ns.only in ("sltiu", "all")
    want_ori = ns.only in ("ori", "all")
    want_clear_set = ns.only in ("clear_set", "all")

    sltiu: list[tuple[str, int, int]] = []
    ori_set: list[tuple[str, int, int]] = []
    clear_set: list[tuple[str, int, int]] = []

    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 3:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        for i in range(1, len(words) - 1):
            prev, a, b = words[i - 1], words[i], words[i + 1]
            # `sltiu rs, rt, 1` then `andi rd, rs, 0xFF`, the source register shared.
            # Matched on the immediate and the shared register rather than on the
            # sub-opcode field: in this assembler's encoding `sltiu $a0, $a0, 1` is
            # 0x2c840001, where bits 25-21 *and* bits 20-16 are both 4, so there is no
            # way to tell from a known member which half carries the sub-opcode.  The
            # test may therefore admit another REGIMM arithmetic form with an immediate
            # of 1; the count below is stated with that caveat rather than as exact.
            # (Testing bits 20-16 for the sub-opcode value 9 instead - which is what the
            # standard MIPS table says - matches nothing at all, and reported zero.)
            if (a >> 26 == 0x0B and (a & 0xFFFF) == 1
                    and b >> 26 == 0x0C and (b & 0xFFFF) == 0xFF
                    and ((b >> 21) & 0x1F) == (a >> 21) & 0x1F):
                sltiu.append((name, addr, i))
                continue
            # A flag setter: `ori rt, rs, MASK` with rt == rs, where the value being
            # preserved was *loaded from memory*, not built by a `lui`.  Without the
            # second condition this matches the float-constant idiom - `lui $a0, 0x3F7D`
            # then `ori $a0, $a0, 0x70A4` is 0x3F7D70A4 as a float - and there are 192
            # of those alone.  Note the register is not always `$a0`: `func_001A9B78`
            # loads the flags into `$a1` because `$a0` is the object pointer.
            rt = (a >> 16) & 0x1F
            if (a >> 26 == 0x0D and rt == (a >> 21) & 0x1F and rt != 0
                    and 0 < (a & 0xFFFF) < 0x8000
                    and prev >> 26 == 0x23 and (prev >> 16) & 0x1F == rt):
                ori_set.append((name, addr, i))

        # Shape 3: clear-then-set in one function.  A `lui` whose low half arrives as
        # `addiu -1`, an `and` through it, and an `or` into that same register.
        if want_clear_set:
            himask = {}
            for i, w in enumerate(words):
                if w >> 26 == 0x0F:
                    himask[(w >> 16) & 0x1F] = i
            for i, w in enumerate(words):
                if w >> 26 == 0x00 and (w & 0x3F) == 0x24:      # and rd, rs, rt
                    src = (w >> 16) & 0x1F
                    lo = himask.get(src)
                    if lo is None or lo >= i:
                        continue
                    # The `addiu -1` completes the mask, and it is not necessarily
                    # adjacent: `func_00012F14` puts an `andi` between it and the
                    # `and`.  Requiring adjacency was the second version and it
                    # matched nothing for the same reason the first did.
                    completed = any(
                        words[j] >> 26 == 0x09
                        and (words[j] >> 21) & 0x1F == src
                        and (words[j] >> 16) & 0x1F == src
                        and (words[j] & 0xFFFF) == 0xFFFF
                        for j in range(lo + 1, i))
                    if not completed:
                        continue
                    rd = (w >> 11) & 0x1F
                    # The `or`'s destination need not be the `and`'s: in
                    # `func_00012F14` the `and` clears into $a2 and the `or` writes a
                    # fresh $a1 that *reads* $a2, which is then stored back.  Testing
                    # that the `or` writes the same register was the first version and
                    # it matched nothing.
                    #
                    # **The search is bounded to eight instructions and to before any
                    # branch**, because "some `or` later in the function" is not a
                    # claim: `func_00012360` is 372 bytes and contains two of these
                    # masks back to back, so an unbounded search pairs the first with
                    # an unrelated `or` hundreds of instructions away.  It still matches
                    # that function, and on this evidence correctly - the second `lui
                    # 0x8000` and `and` show it is doing the same thing again - but the
                    # bound is what makes the count mean clear-then-set rather than
                    # clear-and-something-later.
                    limit = min(len(words), i + 9)
                    for later in words[i + 2:limit]:
                        op = later >> 26
                        if op in (0x04, 0x05, 0x14, 0x15, 0x03):
                            break
                        if (later >> 26 == 0x00 and (later & 0x3F) == 0x25
                                and rd in ((later >> 21) & 0x1F,
                                           (later >> 16) & 0x1F)):
                            clear_set.append((name, addr, i))
                            break
                    break

    if want_sltiu:
        n = len({n for n, _, _ in sltiu})
        print(f"{n} functions contain `sltiu rs, rt, 1` immediately followed by a dead "
              f"`andi rd, rs, 0xFF`\n")
    if want_ori:
        n = len({n for n, _, _ in ori_set})
        print(f"{n} functions contain `ori rt, rs, MASK` with rt == rs, a flag setter "
              f"whose\nmask fits a halfword and so needs no `lui` and no scratch "
              f"register\n")
    if want_clear_set:
        n = len({n for n, _, _ in clear_set})
        print(f"{n} functions clear a flag bit with `lui` + `addiu -1` + `and` and then "
              f"OR a value\nback into the same register - clear and set in one "
              f"function, which `flag_accessors.py`\ndoes not model\n")

    if ns.show:
        shown = ((sltiu if want_sltiu else [])
                 + (ori_set if want_ori else [])
                 + (clear_set if want_clear_set else []))
        for name, addr, at in shown[:ns.show]:
            size = sizes.get(name)
            print(f"  {name} (0x{addr:x}, {size} bytes), match at +0x{at*4:x}:")
            for i in range(max(0, at - 2), min(size // 4, at + 4)):
                w = struct.unpack_from("<I", elf.read(addr + i * 4, 4), 0)[0]
                print(f"      {addr + i*4:#08x}: {mipsdis.make_instruction(w, addr + i*4)}")
            print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
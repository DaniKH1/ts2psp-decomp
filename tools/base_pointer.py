"""How often does a function form a base pointer and then run a loop-free copy?

Five functions in this tree do the same thing: materialise an offset once with
`addiu`, then store at 0, 4 and 8 - or 0, 4, 8 ... 0x3C - of the result, instead of
re-deriving the offset on every store.

    addiu $a3, $a0, 0x44
    sw   $a2, 0x0($a3)
    sw   $t0, 0x4($a3)
    sw   $a1, 0x8($a3)

The alternative is `sw $a2, 0x44($a0)` and so on, three instructions with three
offsets.  **Which one psp-gcc chooses is worth counting, because the answer decides
whether a function's length is a property of its data or of its codegen.**

The test is narrow and mechanical: a function must contain an `addiu` whose
destination is later used as the base of three or more stores at consecutive
multiples of four.  Both `sw` and `swc1` count, so `func_0018A650` - which copies
three floats - is found alongside `func_00102C84`, which copies three words.
Anything with a loop, a branch or a non-multiple-of-four stride in between does not
match, and there is no attempt to decide whether the copies are related to each other.

**617 functions qualify, and the five that prompted the tool are five of them** -
`func_00102D34` at fifteen stores, and `func_00102C84`, `func_00194ADC`,
`func_000E3C24` and `func_0018A650` at three each.  **So this is a habit of the whole
module and not a signature of a few related functions**, which is the claim the five
instances alone could not support.

    python tools/base_pointer.py              # the count
    python tools/base_pointer.py --show 4     # examples in full
    python tools/base_pointer.py --min 3      # require at least this many stores

**The first version of this counted 4, and none of the five were among them.**  It
required every store to follow the `addiu` with nothing in between, on the reasoning
that a straight line of stores is what makes the form worth noticing - and
`func_00194ADC` puts `lw $a1, 0x8($a1)` between its `addiu` and its first store, as
four of the five do.  It also insisted the run start at offset 0, which
`func_00102D34` cannot satisfy because its first store goes through the *pre-`addiu`*
register.  **Both were the test being more specific than the shape**, and a filter that
misses the examples that motivated it is a filter to distrust before it is trusted.
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

from branch_load import _dest  # noqa: E402
from stride_table import load_functions, load_sizes  # noqa: E402


def runs(words: list[int], base: int, kind: str = "store") -> list[tuple[int, int, int]]:
    """(at, register, count) for each base pointer followed by consecutive accesses.

    "Consecutive" means offsets stepping by four on the same register, each access
    coming after the `addiu` that formed the base.  The first access's offset is
    whatever it is - `func_00102D34` writes 0x0 through the *pre-addiu* register and
    starts its run at 4 - and the rest must follow from it.

    `kind` is "store" (`sw`, `swc1`) or "load" (`lw`, `lwc1`, `lbu`, `lhu`).
    **Both directions matter and the two counts differ**: `func_0018A650` forms
    `$a3 = $a0 + 0x1A0` and then reads three floats through it, so it is a load run
    and not a store run - and an earlier draft of `func_0018A650.c` cited it as store
    evidence, which was simply wrong.

    **The first version of this required every access to follow the `addiu` with
    nothing in between**, on the reasoning that a straight line is what makes the
    base-pointer form worth noticing.  That is wrong: `func_00194ADC` puts
    `lw $a1, 0x8($a1)` between its `addiu` and its first store, as four of the five
    functions that motivated the tool do.  It also insisted the run start at offset 0,
    which `func_00102D34` cannot satisfy.  **Both were the test being more specific
    than the shape**, and a filter that misses the examples that motivated it is a
    filter to distrust before it is trusted.
    """
    ops = (0x2B, 0x39) if kind == "store" else (0x23, 0x31, 0x24, 0x25, 0x30)
    out = []
    for i, w in enumerate(words):
        if w >> 26 != 0x09:
            continue
        rt = (w >> 16) & 0x1F
        if rt == 0:
            continue
        expect = None
        count = 0
        first = None
        for j in range(i + 1, len(words)):
            n = words[j]
            op = n >> 26
            if op in (0x04, 0x05, 0x14, 0x15, 0x03, 0x00) and (n & 0x3F) not in (0x08,):
                break                              # a branch or a jump
            hit = op in ops and ((n >> 21) & 0x1F) == rt
            if hit:
                off = n & 0xFFFF
                if off >= 0x8000:
                    break
                if expect is None:
                    expect = off
                elif off != expect:
                    break
                if first is None:
                    first = j
                expect += 4
                count += 1
                continue
            if _dest(n, base + j * 4) == rt:
                break                              # the base was overwritten
        if count:
            out.append((first, rt, count))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=0)
    ap.add_argument("--min", type=int, default=3)
    ap.add_argument("--limit", type=int, default=15)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    funcs, sizes = load_functions(), load_sizes()

    store_fns, load_fns, both = set(), set(), {}
    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size or size % 4 or size // 4 < 4:
            continue
        words = [struct.unpack_from("<I", elf.read(addr, size), i)[0]
                 for i in range(0, size, 4)]
        s = max((c for _, _, c in runs(words, addr, "store")), default=0)
        ld = max((c for _, _, c in runs(words, addr, "load")), default=0)
        if s >= ns.min:
            store_fns.add(name)
            best_run = max(s, ld)
        else:
            best_run = ld
        if ld >= ns.min:
            load_fns.add(name)
        if best_run >= ns.min:
            both[name] = (best_run, size, name, addr, words)

    rows = sorted(both.values(), key=lambda r: (-r[0], r[1]))
    print(f"{len(store_fns | load_fns)} functions form a base pointer and then run "
          f"{ns.min}+ consecutive accesses through it\n")
    print(f"  {len(store_fns)} of them run stores through it")
    print(f"  {len(load_fns)} of them run loads through it")
    print(f"  {len(store_fns & load_fns)} do both\n")
    print(f"  {'name':<24}{'bytes':>7}{'run':>6}")
    for best, size, name, addr, words in rows[:ns.limit]:
        print(f"  {name:<24}{size:>7}{best:>6}")
    if len(rows) > ns.limit:
        print(f"\n  ({len(rows) - ns.limit} more; --limit to see them)")

    if ns.show:
        print()
        for best, size, name, addr, words in sorted(both.values(),
                                                    key=lambda r: r[1])[:ns.show]:
            kind = "store" if name in store_fns else "load"
            start = min(a for a, _, c in runs(words, addr, kind) if c == best)
            print(f"  {name} (0x{addr:x}, {size} bytes), longest {kind} run {best}:")
            for i in range(max(0, start - 3), min(len(words), start + best + 2)):
                print(f"      {addr + i*4:#08x}: "
                      f"{mipsdis.make_instruction(words[i], addr + i*4)}")
            print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
#!/usr/bin/env python3
"""Decode the packed flag bytes at 0x001E1B98, and list the code that reads them.

The array is 128 bytes, one per object class, each packing several independent
booleans into one byte.  Two accessors were already known - `func_00140A58` masks
with 0x07 and `func_00140A74` with 0x04 - and a grep for `sym_001E1B98` finds
**twenty** functions touching it, so the full set of queried bits is worth
extracting rather than guessing at.

The table is strikingly regular, which is the real finding.  Grouping the runs:

    [0x42..0x47]  0x41  x6     0x40 | 0x01
    [0x48..0x5A]  0x01  x19
    [0x61..0x66]  0x42  x6     0x40 | 0x02
    [0x67..0x79]  0x02  x19

Nineteen classes with bit 0 set, nineteen with bit 1 set, and the **first six of
each run additionally carry bit 6**.  Six and six, and the pairing is exact.  That
is two properties of the engine's object hierarchy being encoded: a property shared
by a specific group of six, on top of one shared by a group of nineteen.

    python tools/flag_table.py            # the runs, and what each value means
    python tools/flag_table.py --states    # the four values & 0x07 can actually take
    python tools/flag_table.py --accessors  # the 20 readers and the bits they ask for
    python tools/flag_table.py --bits      # every bit, and how many classes have it
    python tools/flag_table.py --raw       # every entry, one per line
"""
from __future__ import annotations

import argparse
import re
import struct
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from paths import ELF_PATH, ROOT  # noqa: E402
import pspelf  # noqa: E402

# How far the table is known to run, and the symbol splat gave it.
TABLE_ADDR = 0x001E1B98
TABLE_ENTRIES = 128
SYMBOL = "sym_001E1B98"

# `andi $v0, $v0, 0x7` and friends - the bits the readers ask for.
MASK = re.compile(r"andi\s+\$v0,\s*\$v0,\s*(0x[0-9A-Fa-f]+|\d+)")


def runs(values: list[int]) -> list[tuple[int, int, int]]:
    """(start, length, value) for each maximal run of equal bytes."""
    out: list[tuple[int, int, int]] = []
    i = 0
    while i < len(values):
        j = i
        while j + 1 < len(values) and values[j + 1] == values[i]:
            j += 1
        out.append((i, j - i + 1, values[i]))
        i = j + 1
    return out


def bits_of(value: int) -> str:
    """`0x41` as `0,6` - the bits that are set, low to high."""
    return ",".join(str(b) for b in range(8) if value & (1 << b)) or "-"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--accessors", action="store_true",
                    help="list the functions that read the table and their masks")
    ap.add_argument("--bits", action="store_true",
                    help="one line per bit: how many classes have it set")
    ap.add_argument("--raw", action="store_true", help="every entry, one per line")
    ap.add_argument("--states", action="store_true",
                    help="the values `flags[i] & 0x07` can actually take")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    raw = elf.read(TABLE_ADDR, TABLE_ENTRIES)
    values = list(struct.unpack(f"<{TABLE_ENTRIES}b", raw))

    if ns.states:
        print("func_00140A58 returns `flags[index] & 0x07`.  Which values does that")
        print("actually produce?  The distribution is the finding, not the mask.\n")
        counts = Counter(v & 0x07 for v in values)
        for state in sorted(counts):
            entries = [i for i, v in enumerate(values) if (v & 0x07) == state]
            print(f"  state {state}: {counts[state]:3d} classes")
            shown = ", ".join(str(i) for i in entries[:20])
            more = " ..." if len(entries) > 20 else ""
            print(f"           indices {shown}{more}")
        print()
        unseen = [s for s in range(8) if s not in counts]
        print(f"  values never produced: "
              + (", ".join(str(s) for s in unseen) if unseen else "none"))
        if 3 in unseen and 5 in unseen and 6 in unseen and 7 in unseen:
            print("  -> bits 0 and 1 are never both set, and bit 2 never combines")
            print("     with either.  So this is a FOUR-state property, and the")
            print("     accessor returns a field rather than a flag.")
        return 0

    if ns.raw:
        for i, v in enumerate(values):
            print(f"  [{i:3d}] 0x{i:02x}  0x{v & 0xFF:02x}  bits {bits_of(v & 0xFF)}")
        return 0

    if ns.bits:
        print(f"{TABLE_ENTRIES} entries at 0x{TABLE_ADDR:08X}")
        counts = Counter()
        for v in values:
            for b in range(8):
                if v & (1 << b):
                    counts[b] += 1
        for b in range(8):
            set_in = [i for i, v in enumerate(values) if v & (1 << b)]
            print(f"  bit {b}: {counts[b]:3d} classes"
                  + (f"   e.g. {set_in[:12]}" if set_in else ""))
        return 0

    if ns.accessors:
        asm = ROOT / "asm" / "eboot"
        users = []
        for path in sorted(asm.glob("*.s")):
            text = path.read_text(encoding="utf-8", errors="replace")
            if SYMBOL not in text:
                continue
            name = path.stem
            body = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
            masks = [int(m, 0) for m in MASK.findall(body)]
            users.append((name, masks))
        print(f"{len(users)} functions reference {SYMBOL}\n")
        print(f"{'function':<20} masks asked for")
        for name, masks in users:
            if masks:
                shown = " ".join(f"0x{m:x}" for m in masks)
                bits = set()
                for m in masks:
                    bits |= {b for b in range(8) if m & (1 << b)}
                extra = f"   -> bits {sorted(bits)}"
            else:
                shown = "(writes, or reads without a mask)"
                extra = ""
            print(f"  {name:<20} {shown}{extra}")
        return 0

    print(f"{TABLE_ENTRIES} packed flag bytes at 0x{TABLE_ADDR:08X}")
    print(f"{'index':>12}  {'count':>5}  {'value':>5}  bits\n")
    for start, count, value in runs(values):
        v = value & 0xFF
        end = start + count - 1
        where = f"[{start:#04x}]" if count == 1 else f"[{start:#04x}..{end:#04x}]"
        print(f"  {where:>12}  {count:5d}  0x{v:02x}     {bits_of(v)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
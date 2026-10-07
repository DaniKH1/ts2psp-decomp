#!/usr/bin/env python3
"""Decode the packed flag bytes at 0x001E1B98, and list the code that reads them.

The array is 128 bytes, each packing several independent properties into one
byte, indexed by a value in a 128-wide domain taken from a data stream.  **What
that domain is remains open** - it is not ASCII (`--domain` tests that and shows
it is not), and an earlier version of this docstring claimed one entry per object
class, which had no evidence behind it.

Two accessors were known before this tool - `func_00140A58` masks with 0x07 and
`func_00140A74` with 0x04 - and a grep for `sym_001E1B98` finds **twenty**
functions touching it, so the full set of queried bits is worth extracting rather
than guessing at.

The table is strikingly regular, which is the real finding.  Grouping the runs:

    [0x42..0x47]  0x41  x6     0x40 | 0x01
    [0x48..0x5b]  0x01  x20
    [0x62..0x67]  0x42  x6     0x40 | 0x02
    [0x68..0x7b]  0x02  x20

Twenty entries with bit 0 set, twenty with bit 1 set, and the **first six of each
run additionally carry bit 6**.  Six and six, and the pairing is exact.  Two
properties of whatever this index space describes, encoded one byte each.

    python tools/flag_table.py            # the runs, and what each value means
    python tools/flag_table.py --states    # the four values & 0x07 can actually take
    python tools/flag_table.py --domain    # test the "indexed by ASCII" idea
    python tools/flag_table.py --accessors  # the 20 readers and the bits they ask for
    python tools/flag_table.py --bits      # every bit, and how many entries have it
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

# `andi $dst, $src, mask`.  The register is matched loosely - an earlier version
# only looked for `$v0`, which missed func_00143838, which masks with 0x1 in `$t0`.
# A preceding byte load into the same register is required too: `andi $reg, $reg,
# 0xff` is the usual way to write `& 0xFF` on something that is *not* the flag
# byte, and counting those claims every bit is queried everywhere.
ANDI = re.compile(r"andi\s+\$(\w+),\s*\$(\w+),\s*(0x[0-9A-Fa-f]+|\d+)")
LOAD_BYTE = re.compile(r"l[bhu]c?\s+\$(\w+),")

# A mask of 0xFF keeps every bit of a byte, so it says nothing about which bit is
# being asked for.  Reported separately rather than folded into the union.
UNINFORMATIVE = {0xFF}


def body_of(path: Path, name: str) -> str:
    """The instruction lines of one glabel block."""
    text = path.read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
    return "\n".join(line for line in chunk.splitlines() if "/*" in line)


def masks_on_loaded_byte(body: str) -> list[int]:
    """Masks applied to a value a byte load had written.

    Approximate, but far better than "every `andi` in the function": a mask counts
    only if the register it reads was the destination of an earlier byte load in
    the same block.  Still a heuristic - the load may be of something else - but it
    excludes the `& 0xFF` idiom, which was most of what the naive search found.
    """
    loaded: set[str] = set()
    found: list[int] = []
    for line in body.splitlines():
        m = LOAD_BYTE.search(line)
        if m:
            loaded.add(m.group(1))
        a = ANDI.search(line)
        if a and a.group(2) in loaded:
            found.append(int(a.group(3), 0))
    return found


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
    ap.add_argument("--domain", action="store_true",
                    help="test whether the index is an ASCII character code")
    ap.add_argument("--asked", action="store_true",
                    help="for each bit, which functions ask for it")
    ns = ap.parse_args()

    if ns.asked:
        asm = ROOT / "asm" / "eboot"
        askers: dict[int, list[str]] = {b: [] for b in range(8)}
        users = 0
        for path in sorted(asm.glob("*.s")):
            if SYMBOL not in path.read_text(encoding="utf-8", errors="replace"):
                continue
            users += 1
            masks = masks_on_loaded_byte(body_of(path, path.stem))
            bits: set[int] = set()
            for m in masks:
                if m in UNINFORMATIVE:
                    continue
                bits |= {b for b in range(8) if m & (1 << b)}
            for b in bits:
                askers[b].append(path.stem)
        print(f"{users} functions reference {SYMBOL}\n")
        for b in range(8):
            names = askers[b]
            print(f"  bit {b}: asked by {len(names)}")
            if not names:
                print("          nothing asks for it")
            else:
                print("          " + ", ".join(n.replace("func_", "") for n in names))
        never = [b for b in range(8) if not askers[b]]
        print()
        print("  bits nothing asks for: "
              + (", ".join(str(b) for b in never) if never else "none"))
        return 0

    if ns.domain:
        # `func_001434C0` walks a byte stream and looks up `flags[base + c]` for
        # each byte, which reads like a character-class table.  It is not.  Both
        # alignments are tested here and both fail: no digit has bit 3 set.
        #
        # The same function compares the byte against 0x2B and 0x2D, so those are
        # values in the index domain rather than '+' and '-'.  The index is a
        # 128-value token or enum space; what the tokens mean is not established,
        # and "one entry per object class" - which this tool's docstring used to
        # claim - has no evidence behind it either.
        elf = pspelf.load(str(ELF_PATH))
        for base, label in ((TABLE_ADDR + 1, "0x1E1B99, as the code computes it"),
                            (TABLE_ADDR, "0x1E1B98, as this table is indexed")):
            digits = all(elf.read(base + ord(ch), 1)[0] & 8 for ch in "0123456789")
            signs = any(elf.read(base + ord(ch), 1)[0] & 8 for ch in "+-")
            print(f"{label}: flags[0x{base:08X} + c]")
            print(f"  every digit has bit 3 set : {digits}")
            print(f"  a sign has bit 3 set      : {signs}")
            print(f"  -> character-class table  : "
                  f"{'yes' if digits and not signs else 'NO'}")
            print()
        return 0

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
            if SYMBOL not in path.read_text(encoding="utf-8", errors="replace"):
                continue
            users.append((path.stem, masks_on_loaded_byte(body_of(path, path.stem))))
        readers = [u for u in users if u[1]]
        print(f"{len(users)} functions reference {SYMBOL}; "
              f"{len(readers)} of them mask a byte they loaded\n")
        print(f"{'function':<20} masks   bits asked for")
        for name, masks in users:
            if not masks:
                print(f"  {name:<20} -      (no mask on a loaded byte)")
                continue
            bits: set[int] = set()
            for m in masks:
                if m not in UNINFORMATIVE:
                    bits |= {b for b in range(8) if m & (1 << b)}
            shown = " ".join(f"0x{m:x}" for m in masks)
            note = "" if 0xFF not in masks else "   (0xFF keeps everything)"
            print(f"  {name:<20} {shown}{note}")
            print(f"  {'':<20} -> bits {sorted(bits)}")
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
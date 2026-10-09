"""Find the module's vtables and measure how widely each method is shared.

The eleven-record family at a 0xD8 stride was found by hand.  This finds the
rest of them, and answers the question that family raised: how many classes
inherit a given method.

A candidate is a run of *eight-byte* entries in .rodata or .data, each one an
adjustment word followed by a function pointer:

    +0x00  i32  adjustment    (0 for every entry in this module)
    +0x04  i32  function pointer

Two things follow from that and both were got wrong the first time.

**The entries are eight bytes, not four.**  Treating them as four-byte
pointers makes a real 27-entry table look like 54 slots, half of them
structural padding, and a run of 26 pointers among 54 words fails any
sensible density test - which is why scanning for four-byte pointers
finds almost nothing once the padding is excluded.

**The adjustment word is the multiple-inheritance thunk.**  It is the same
`{i16 adjust; void (*fn)()} ` structure that `func_0019D11C` and
`func_000BE138` read out of the +0xD0 thunk arrays, and it is zero for
every entry measured here, which is why these tables behave as plain
vtables.

Testing membership in the function set - rather than an address range - is
also the point: an address-range test accepts small integers as code
addresses.

Usage:
    python tools/vtables.py                # the census
    python tools/vtables.py --wide 14      # the most shared methods
    python tools/vtables.py --list 0x1EAA78  # dump one table entry by entry
"""

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pspelf
from paths import ELF_PATH

MIN_PTRS = 4
MIN_ENTRIES = 4
MAX_ENTRIES = 8192
MIN_PTR_FRACTION = 0.6
ENTRY = 8

# An adjustment is a byte offset applied to `this`, so it has to be small.
# Without this bound a plain data table whose words happen to alternate
# data and code reads as a thunk table with million-byte offsets, which is
# what happened when the adjustment word was accepted unfiltered.
MAX_ADJUST = 0x2000

# Tables in this module are laid out back to back, so a run of conforming
# entries is usually several tables and not one.  The largest run found
# spans 0x1EA3E8..0x1EBD88 - 820 entries - and it contains both the
# three-record family at sym_001EA3E8 and the eleven-record family at
# sym_001EAA78, which are 0x690 bytes apart and adjacent.  Nothing in the
# bytes marks where one table ends and the next begins, so a run is NOT a
# table and the run count is NOT a class count.
LIKELY_MERGED = 32


def load_symbols():
    """address -> name, and the set of addresses that begin a function."""
    labels, funcs = {}, set()
    for line in open("config/eboot.symbol_addrs.txt", encoding="utf-8"):
        if "=" not in line:
            continue
        name = line.split("=", 1)[0].strip()
        value = line.split("=")[1].split(";", 1)[0].strip()
        try:
            addr = int(value, 0)
        except ValueError:
            continue
        labels[addr] = name
        if "type:func" in line:
            funcs.add(addr)
    # Address 0 is the module entry point, so the symbol map calls it a
    # function.  A null word in a table is a null pointer, not an entry
    # pointing at it, and counting it makes every table look as though it
    # shared a method - which is how this script first reported a
    # function present in all 289 of them.
    funcs.discard(0)
    return labels, funcs


def find_tables(elf, funcs):
    """Runs of eight-byte {adjust, fnptr} entries in read-only and writable data.

    A table may begin with a zero header - the eleven-record family has an
    eight-byte one, so 8 + 26 * 8 == 216 exactly - so a zero entry is
    tolerated at the front and does not break the run.

    The adjustment word is taken as it stands, so long as it is small
    enough to be a byte offset - sym_001E5DD0 carries -4 in every entry
    and is a genuine multiple-inheritance thunk table.  Requiring it to
    be zero, as an earlier version did, discarded precisely those.
    """
    def signed(a):
        return a - 0x10000 if a & 0x8000 else a

    def plausible(a):
        return a == 0 or abs(signed(a)) <= MAX_ADJUST

    secs = {s.name: s for s in elf.sections}
    tables = []
    for sec in (secs[".rodata"], secs[".data"]):
        raw = elf.read(sec.addr, sec.size)
        words = [struct.unpack("<I", raw[j:j + 4])[0] for j in range(0, len(raw), 4)]
        nent = len(words) // 2      # an entry is two words, not ENTRY bytes of them
        i = 0
        while i < nent:
            adj, f = words[2 * i], words[2 * i + 1]
            if f != 0 and (f not in funcs or not plausible(adj)):
                i += 1
                continue
            j, nptr, nonzero_adj, adjusts = i, 0, 0, set()
            while j < nent:
                a, b = words[2 * j], words[2 * j + 1]
                if b == 0 and a == 0:
                    j += 1              # header or trailing nulls
                    continue
                if b not in funcs or not plausible(a):
                    break
                nptr += 1
                adjusts.add(a)
                if a:
                    nonzero_adj += 1
                j += 1
            span = j - i
            if (nptr >= MIN_PTRS and MIN_ENTRIES <= span <= MAX_ENTRIES
                    and nptr >= span * MIN_PTR_FRACTION):
                tables.append((sec.addr + i * ENTRY, span, words[2 * i:2 * j], sec.name,
                               nonzero_adj, sorted(adjusts - {0})))
            i = j if j > i else i + 1
    return tables


def function_size(name):
    p = Path("asm/eboot") / (name + ".s")
    if not p.exists():
        return None
    import re
    m = re.search(r"^nonmatching\s+\S+,\s*(0x[0-9A-Fa-f]+|\d+)",
                  p.read_text(encoding="utf-8"), re.M)
    return int(m.group(1), 0) if m else None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--wide", type=int, default=12,
                    help="how many of the most shared methods to show")
    ap.add_argument("--list", metavar="ADDR",
                    help="dump the table at ADDR slot by slot and exit")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    labels, funcs = load_symbols()

    if ns.list:
        addr = int(ns.list, 0)
        entries = 27
        for k in range(entries):
            d = elf.read(addr + k * ENTRY, ENTRY)
            adj, w = struct.unpack("<II", d)
            note = labels.get(w, "")
            size = function_size(note) if note.startswith("func_") else None
            print("  [%2d] +0x%03X  adj %-10d 0x%08X  %-16s %s"
                  % (k, k * ENTRY, adj, w, note, "size %d" % size if size else ""))
        return 0

    tables = find_tables(elf, funcs)

    # A table counts once per distinct address however many entries repeat it.
    # Counting per entry makes a class that inherits the same base twice look
    # like two classes, and the tell is that the maximum exceeds the total.
    use = defaultdict(set)
    slots_of = defaultdict(set)
    for addr, span, words, _sec, _nz, _adj in tables:
        for k in range(0, len(words), ENTRY):
            w = words[k + 1]
            if w in funcs:
                use[w].add(addr)
                slots_of[w].add(k // ENTRY)

    widest = max(len(v) for v in use.values()) if use else 0
    assert widest <= len(tables), (
        "internal error: %d tables found but one function claims %d"
        % (len(tables), widest))

    sizes = sorted(t[1] for t in tables)
    merged = [t for t in tables if t[1] > LIKELY_MERGED]
    print("candidate table runs: %d  (8-byte entries)" % len(tables))
    bysec = defaultdict(int)
    for t in tables:
        bysec[t[3]] += 1
    for sec, count in sorted(bysec.items()):
        print("   in %-9s %d" % (sec, count))
    print("  entries: min %d, median %d, max %d"
          % (sizes[0], sizes[len(sizes) // 2], sizes[-1]))
    print("  distinct functions used as entries: %d" % len(use))
    print("  widest single membership: %d of %d runs" % (widest, len(tables)))

    print()
    print("adjustment words, which are the multiple-inheritance offsets:")
    hist = defaultdict(int)
    for t in tables:
        for a in t[5]:
            hist[a] += 1
    if not hist:
        print("   none: every entry in every run has adjustment 0, so these")
        print("   tables have the layout of thunk arrays and behave as plain")
        print("   vtables.")
    else:
        for a, c in sorted(hist.items()):
            signed = a - 0x10000 if a & 0x8000 else a
            print("   %6d (0x%04X = %+d)  in %d runs" % (a, a, signed, c))
        nz = [t for t in tables if t[4]]
        print("   %d of %d runs carry a non-zero adjustment" % (len(nz), len(tables)))
        for t in sorted(nz, key=lambda t: -t[4])[:8]:
            print("      0x%06X  %4d entries, adjustments %s"
                  % (t[0], t[1], [a - 0x10000 if a & 0x8000 else a for a in t[5]]))
    if merged:
        print()
        print("  %d run(s) longer than %d entries; each is probably several"
              % (len(merged), LIKELY_MERGED))
        print("  tables placed end to end.  Nothing in the bytes separates them,")
        print("  so the class count below is a LOWER BOUND and the membership")
        print("  of anything shared by adjacent tables is undercounted.")
        for t in sorted(merged, key=lambda t: -t[1])[:6]:
            print("     0x%06X  %4d entries" % (t[0], t[1]))

    hist = defaultdict(int)
    for v in use.values():
        hist[len(v)] += 1
    biggest = max(sizes) if sizes else 0
    if biggest > sum(sizes) * 0.5:
        print()
        print("  MEMBERSHIP CENSUS SUPPRESSED")
        print("  One run holds %d of %d entries, so nearly everything landed in a"
              % (biggest, sum(sizes)))
        print("  single block.  A membership count over one run is trivially 1 for")
        print("  every function and says nothing.  Separate the tables first; this")
        print("  happens whenever the adjustment filter is relaxed far enough to")
        print("  keep the real thunk tables.")
    else:
        print()
        print("functions appearing in exactly k tables:")
        for k in sorted(hist):
            if k >= 3:
                print("   k=%-3d %4d functions" % (k, hist[k]))

        print()
        print("most widely shared methods:")
        for w, v in sorted(use.items(), key=lambda kv: -len(kv[1]))[:ns.wide]:
            name = labels.get(w, "<unlabelled 0x%X>" % w)
            size = function_size(name) if name.startswith("func_") else None
            sl = sorted(slots_of[w])[:4]
            tag = "promoted" if (Path("src/eboot") / (name + ".c")).exists() else ""
            print("   %-16s %3d tables  size %-5s slots %-14s %s"
                  % (name, len(v), size, ",".join(str(x) for x in sl), tag))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
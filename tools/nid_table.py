#!/usr/bin/env python3
"""Report the PSP import table: the stubs, their libraries, and the NID words.

What this establishes, and what it does not.

The 26 `.sceStub.text.*` sections are **empty stubs** - every one of them is
`jr $ra` followed by `nop`:

    0x001b0128: jr    $ra
    0x001b012c: nop

That is not a mistake and not a stripped-out call.  It is a PSPLINK module before
loading: the loader matches each entry of `.rodata.sceNid` against the NIDs of the
loaded libraries and patches the corresponding stub with the real address.  In the
file on disc every stub is a placeholder that returns immediately.

So **the stub code carries no information at all**, and the only thing the sections
give is the 26 library names, which are already spelled out in the section names:
`sceUtility`, `sceAudio`, `sceMpeg`, `sceNet`, `sceCtrl` and the rest.  The useful
part of this script is therefore the correspondence between stub index, address,
library and NID word - so that whoever has a NID table can fill in the names.

**The entry size of `.rodata.sceNid` is unresolved, and the script says so rather
than guessing.**  There are 223 stubs and the section is 223 32-bit words, so one
word per stub fits exactly - but a PSP NID is 64 bits, which would mean 111 entries
and four bytes left over instead.  The obvious test does not settle it: PSP NIDs
have their top bit set, and neither reading satisfies that (103 of 223 under the
one-word reading, 53 of 111 under the two-word one - roughly half either way, so
the assumption itself is wrong, not the layout).

`--words` therefore says how many 32-bit words of the table to attribute to each
stub, and defaults to the one that divides evenly.  Whoever has a NID table can set
it to 2 and read the 64-bit values.

    python tools/nid_table.py             # the table
    python tools/nid_table.py --libs      # just the libraries and counts
    python tools/nid_table.py --words 2   # read the table as 64-bit NIDs
"""
from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from paths import ELF_PATH  # noqa: E402
import pspelf  # noqa: E402

NID = ".rodata.sceNid"
STUB_PREFIX = ".sceStub.text"

# `jr $ra` / `nop`, little-endian.
JR_RA = 0x03E00008
NOP = 0x00000000


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--words", type=int, default=1,
                    help="32-bit words of the table per stub (default 1)")
    ap.add_argument("--libs", action="store_true",
                    help="only the libraries and how many stubs each has")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))

    stubs = sorted(
        (s for s in elf.sections if s.name.startswith(STUB_PREFIX)),
        key=lambda s: s.addr,
    )
    if not stubs:
        print("no stub sections found", file=sys.stderr)
        return 1

    # Every stub, in address order, with the library it belongs to.
    entries: list[tuple[str, int, int]] = []       # library, address, offset in .text
    for sec in stubs:
        library = sec.name[len(STUB_PREFIX) + 1:]
        for i in range(sec.size // 8):
            entries.append((library, sec.addr + i * 8, i))

    # Confirm they really are empty, rather than trusting the pattern.
    nonempty = []
    for _lib, addr, _off in entries:
        code = struct.unpack("<2I", elf.read(addr, 8))
        if code != (JR_RA, NOP):
            nonempty.append((addr, code))

    table = elf.section(NID)
    words = list(struct.unpack(f"<{table.size // 4}I",
                               elf.read(table.addr, table.size)))

    if ns.libs:
        counts: dict[str, int] = {}
        for lib, _a, _o in entries:
            counts[lib] = counts.get(lib, 0) + 1
        print(f"{len(counts)} libraries, {len(entries)} stubs")
        for lib in sorted(counts):
            print(f"  {counts[lib]:4d}  {lib}")
        return 0

    per = ns.words
    print(f"{len(entries)} stubs in {len(stubs)} sections; "
          f"{NID} is {table.size} bytes = {len(words)} words")
    print(f"all stubs are `jr $ra` / `nop`: "
          f"{'yes' if not nonempty else f'NO - {len(nonempty)} differ'}")
    if nonempty:
        for addr, code in nonempty[:5]:
            print(f"    0x{addr:08x}: {' '.join(f'{w:08x}' for w in code)}")
    print()
    print(f"At {per} word(s) per stub the section covers "
          f"{len(words) // per} of {len(entries)} stubs"
          + (f", with {len(words) % per} words left over.\n"
             if len(words) % per else " exactly.\n"))
    print("A PSP NID is 64 bits, so 2 words per stub is the other reading; the data")
    print("does not settle which.  See the module docstring.")
    print()

    print(f"{'#':>4}  {'library':<24} {'stub':<12}  NID words")
    for i, (lib, addr, _off) in enumerate(entries):
        chunk = words[i * per:(i + 1) * per]
        if not chunk:
            break
        nid = " ".join(f"{w:08x}" for w in chunk)
        print(f"{i:4d}  {lib:<24} 0x{addr:08x}    {nid}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
"""Recover the string literals in `.rodata`.

The module is stripped, but its string literals survived, and every one of them
is a `R_MIPS_32` relocation target, which means each has a known address.  Naming
them turns thousands of `%hi(sym_XXXXXXXX)` references in the generated
assembly into readable text.

    python tools/rodata_strings.py            # summary
    python tools/rodata_strings.py --at 0x1CFB50
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import pspelf  # noqa: E402

MIN_LENGTH = 4
# Names have to be valid C identifiers and stable, so anything that is not a
# conservative word character becomes an escape.
UNSAFE = re.compile(r"[^A-Za-z0-9]+")


def printable(data: bytes) -> bool:
    return all(0x20 <= b < 0x7F for b in data)


def c_identifier(text: str) -> str:
    ident = UNSAFE.sub("_", text).strip("_")
    if not ident:
        ident = "str"
    if ident[0].isdigit():
        ident = "_" + ident
    return ident[:48]


def find_strings(elf, section_name: str = ".rodata",
                 min_length: int = MIN_LENGTH) -> list[tuple[int, str]]:
    sec = elf.section(section_name)
    if sec is None:
        return []
    out: list[tuple[int, str]] = []
    data = sec.data
    i = 0
    while i < len(data):
        if not (0x20 <= data[i] < 0x7F):
            i += 1
            continue
        j = i
        while j < len(data) and 0x20 <= data[j] < 0x7F:
            j += 1
        # A literal is terminated by a NUL, so a run that runs into the next
        # character is not one.
        if j < len(data) and data[j] == 0 and j - i >= min_length:
            out.append((sec.addr + i, data[i:j].decode("ascii")))
        i = j + 1 if j < len(data) and data[j] == 0 else j + 1
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--section", default=".rodata")
    ap.add_argument("--at", type=lambda s: int(s, 0),
                    help="show the string at this address")
    ap.add_argument("--grep")
    ap.add_argument("--limit", type=int, default=60)
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    strings = find_strings(elf, ns.section)

    if ns.at is not None:
        for addr, text in strings:
            if addr <= ns.at < addr + len(text) + 1:
                print(f"{addr:#x} {text!r}")
        return 0

    sel = strings
    if ns.grep:
        sel = [(a, t) for a, t in strings if ns.grep.lower() in t.lower()]
    print(f"{len(strings)} strings in {ns.section}"
          f"{f', {len(sel)} matching' if ns.grep else ''}\n")
    for addr, text in sel[:ns.limit]:
        print(f"  {addr:#010x}  str_{c_identifier(text):<44} {text!r}")
    if len(sel) > ns.limit:
        print(f"  ... {len(sel) - ns.limit} more")
    return 0


if __name__ == "__main__":
    sys.exit(main())
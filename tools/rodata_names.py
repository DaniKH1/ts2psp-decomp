"""Read the string literals whose original names survived, and check the bytes there.

3,532 of the surviving names land in `.rodata`.  That is worth checking properly: if
the name is a flattened copy of the string's contents, the two should agree, and if
they do then the naming is not a recovery problem at all -- the strings are simply
readable and the names are a cross-check on the reading.

If they do *not* agree, that is more interesting still: it would mean the symbol names
and the stored bytes come from different string literals, which is the sort of thing
that tells you something about how the original was built.
"""
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(r"F:\Github Repositories\ts2psp decomp")
sys.path.insert(0, str(ROOT / "tools"))
import pspelf  # noqa: E402
from paths import ELF_PATH  # noqa: E402

ENTRY = re.compile(r"^(\S+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*(.*)$")
OURS = ("func_", "sym_", "D_", "L_", "l_", "d_", "__", "R_MIPS", "j_")


def classes() -> int:
    """The C++ class::method names, which have to be read out of the bytes.

    The symbol name flattens `::` to `_`, so searching the names for `::` finds
    nothing - `str_BoidBehavior_avoidWalls` is the flattened form of the stored
    `BoidBehavior::avoidWalls`.  The string is where the qualification is.
    """
    import collections

    elf = pspelf.load(str(ELF_PATH))
    text = (ROOT / "config" / "eboot.symbol_addrs.txt").read_text(
        encoding="utf-8", errors="replace")

    strings: list[str] = []
    for line in text.splitlines():
        m = ENTRY.match(line.strip())
        if not m:
            continue
        name, addr = m.group(1), int(m.group(2), 0)
        if name.startswith(OURS):
            continue
        sec = elf.section_of(addr)
        if sec is None or sec.name != ".rodata":
            continue
        try:
            raw = elf.read(addr, 120).split(b"\x00")[0]
        except Exception:
            continue
        strings.append(raw.decode("latin-1"))

    qualified = [s for s in strings if "::" in s]
    by_class = collections.Counter(s.split("::")[0] for s in qualified)
    print(f"{len(strings)} named strings in .rodata")
    print(f"{len(qualified)} contain '::', over {len(by_class)} classes\n")
    for cls, count in by_class.most_common(40):
        members = sorted({s.split("::", 1)[1] for s in qualified
                          if s.startswith(cls + "::")})
        shown = ", ".join(members[:5])
        more = f" +{len(members) - 5}" if len(members) > 5 else ""
        print(f"  {count:>2}x  {cls}::{shown}{more}")
    return 0


def paths() -> int:
    """Names whose *contents* are a flattened source path.

    The linker flattens a path into an identifier: separators and the extension
    become underscores and the case goes to lower, but the words survive in order.
    `/c/AD/clean/sims_psp/src/elem/bent/circular.h` reads back as
    `str_c_ad_clean_sims_psp_src_elem_bent_circular_h`.

    Recognising one needs two things the name alone does not give: the drive-letter
    prefix (`_c_`), and a run of at least four lowercase words.  The stored string is
    the better test where it exists, so this reads the bytes and looks for the `/`
    that a real path keeps and the flattening does not.
    """
    import collections

    elf = pspelf.load(str(ELF_PATH))
    text = (ROOT / "config" / "eboot.symbol_addrs.txt").read_text(
        encoding="utf-8", errors="replace")

    hits = []
    for line in text.splitlines():
        m = ENTRY.match(line.strip())
        if not m:
            continue
        name, addr = m.group(1), int(m.group(2), 0)
        if name.startswith(OURS):
            continue
        sec = elf.section_of(addr)
        if sec is None or sec.name != ".rodata":
            continue
        try:
            raw = elf.read(addr, 160).split(b"\x00")[0].decode("latin-1")
        except Exception:
            continue
        if "/" in raw:
            hits.append((addr, raw))
        # A flattened build path keeps a drive letter and the extension, both of
        # which become part of the identifier: `str_c_ad_clean_sims_psp_src_elem_...`.
        elif re.match(r"^str_[a-z]_[a-z]+(_[a-z0-9]+)+_(h|c|cpp|inc)$", name):
            hits.append((addr, raw))

    # A sentence with five lowercase words in it is not a path.  An earlier version
    # of this matched on word count alone and returned 221 "paths", most of which
    # were ordinary error messages - so the test is the separator, and a name only
    # counts if it ends in a source extension.
    print(f"{len(hits)} strings contain a path separator, or are flattened "
          f"build paths\n")
    seen = set()
    for addr, raw in hits:
        if raw in seen:
            continue
        seen.add(raw)
        print(f"  0x{addr:08x}  {raw}")
    return 0


def main() -> int:
    if "--classes" in sys.argv:
        return classes()
    if "--paths" in sys.argv:
        return paths()
    elf = pspelf.load(str(ELF_PATH))
    text = (ROOT / "config" / "eboot.symbol_addrs.txt").read_text(
        encoding="utf-8", errors="replace")

    rodata = []
    for line in text.splitlines():
        m = ENTRY.match(line.strip())
        if not m:
            continue
        name, addr = m.group(1), int(m.group(2), 0)
        if name.startswith(OURS):
            continue
        sec = elf.section_of(addr)
        if sec is not None and sec.name == ".rodata":
            rodata.append((name, addr))

    print(f"{len(rodata)} original names land in .rodata\n")
    agree = disagree = unreadable = 0
    shown = 0
    for name, addr in rodata:
        try:
            data = elf.read(addr, 80)
        except Exception:
            unreadable += 1
            continue
        raw = data.split(b"\x00")[0]
        # A flattened name and a stored string differ in case and in punctuation.
        squash = lambda b: re.sub(rb"[^A-Za-z0-9]", b"", b).lower()  # noqa: E731
        a, b = squash(name.encode()), squash(raw)
        if a.startswith(b"str"):
            a = a[3:]
        if a and (a == b or a.startswith(b[:8]) or b.startswith(a[:8])):
            agree += 1
        else:
            disagree += 1
        if shown < 20:
            shown += 1
            print(f"  0x{addr:08x}  {name[:40]:<40} | {raw[:40]!r}")
    print(f"\n  name and bytes agree:      {agree}")
    print(f"  name and bytes disagree:   {disagree}")
    print(f"  unreadable:                {unreadable}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
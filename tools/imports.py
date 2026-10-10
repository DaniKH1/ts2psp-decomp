"""Recover import names for the 223 function stubs in the image.

The retail ELF stores imports as bare 32-bit NIDs (.rodata.sceNid) plus
per-library stub tables - no name table survives, because names only ever
existed in the SDK that linked the game.  Two sources rebuild them:

  1. pspsdk records the name of every import in its assembly stub macros,
     with the NID spelled out next to it:

         IMPORT_FUNC "InitForKernel",0x1D3256BA,sceKernelRegisterChunk

     a join on (library, NID) - authoritative where it covers;
  2. the NID itself is the first 4 bytes of SHA-1(function name), read
     little-endian (salted on later firmwares, but userland import NIDs
     of this era are plain) - so names pspsdk never stubs can still be
     confirmed by hashing candidates from its headers;
  3. config/nid-extra.txt: the handful of records neither source covers,
     curated from pspdev/psplibdoc (offline, provenance in the file).

    python tools/imports.py                 # recover -> config/imports.txt
    python tools/imports.py --sdk <dir>      # name source
                                             #   (default: C:/pspdev-src/pspsdk)

Import table layout decoded from the image itself (26 entries, stride 20,
in .lib.stub, each pointing into .rodata.sceResident / .rodata.sceNid):

    u32 libname      pointer to the ASCII library name
    u32 version      library version, e.g. 0x40010011
    u32 count_and_sz (nid_count << 16) | 5   (5 = struct size in words)
    u32 nidtable     pointer to nid_count NIDs in .rodata.sceNid
    u32 stubstart    first 8-byte stub; stub i is at stubstart + i*8
"""

from __future__ import annotations

import argparse
import hashlib
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402

OUT = ROOT / "config" / "imports.txt"
EXTRA = ROOT / "config" / "nid-extra.txt"
DEFAULT_SDK = Path(r"C:\pspdev-src\pspsdk")

LIB_STUB_STRIDE = 20
STUB_SIZE = 8

# IMPORT_FUNC "lib",0xNID,name  and  IMPORT_FUNC_WITH_ALIAS ...,name,alias
IMPORT_RE = re.compile(
    r'IMPORT_FUNC(?:_WITH_ALIAS)?\s+"([^"]+)"\s*,\s*0x([0-9A-Fa-f]{8})'
    r'\s*,\s*([A-Za-z_][A-Za-z0-9_]*)(?:\s*,\s*([A-Za-z_][A-Za-z0-9_]*))?'
)


def read_imports(mod: pspelf.Module) -> list[tuple[int, str, int, int]]:
    """(stub_addr, libname, nid, index) for every import, image order."""
    if ".lib.stub" not in mod.sections:
        raise SystemExit("error: no .lib.stub section in the image")
    addr, size, _ = mod.sections[".lib.stub"]
    if size % LIB_STUB_STRIDE:
        raise SystemExit(f"error: .lib.stub size 0x{size:X} not a multiple "
                         f"of {LIB_STUB_STRIDE}")

    imports: list[tuple[int, str, int, int]] = []
    for i in range(size // LIB_STUB_STRIDE):
        e = mod.read(addr + i * LIB_STUB_STRIDE, LIB_STUB_STRIDE)
        word = [int.from_bytes(e[j:j + 4], "little") for j in range(0, 20, 4)]
        name_ptr, _version, count_sz, nid_ptr, stub = word
        count = count_sz >> 16

        name = mod.read(name_ptr, 64).split(b"\0")[0].decode("ascii")
        # cross-check: the library's stub section must be count * 8 long
        sec = mod.containing_section(stub)
        if sec is None or not sec.startswith(".sceStub.text."):
            raise SystemExit(f"error: library {name}: stub 0x{stub:08X} "
                             f"not in a .sceStub.text.* section")
        sec_size = mod.sections[sec][1]
        if sec_size != count * STUB_SIZE:
            raise SystemExit(f"error: library {name}: section {sec} is "
                             f"0x{sec_size:X}, table says {count} stubs")

        for j in range(count):
            nid = int.from_bytes(mod.read(nid_ptr + j * 4, 4), "little")
            imports.append((stub + j * STUB_SIZE, name, nid, j))
    return imports


def psp_pairs(sdk: Path) -> tuple[dict[tuple[str, int], list[str]],
                                  dict[int, list[str]]]:
    """(lib, NID) -> [names], and NID -> [names], from pspsdk .S files."""
    if not sdk.is_dir():
        raise SystemExit(f"error: {sdk} is not a directory - clone "
                         f"pspdev/pspsdk there or pass --sdk")
    by_lib: dict[tuple[str, int], list[str]] = {}
    by_nid: dict[int, list[str]] = {}
    files = sorted(sdk.rglob("*.S")) + sorted(sdk.rglob("*.s"))
    if not files:
        raise SystemExit(f"error: no .S files under {sdk}")
    for path in files:
        text = path.read_text(encoding="utf-8", errors="ignore")
        for m in IMPORT_RE.finditer(text):
            lib, hexnid, name, alias = m.groups()
            nid = int(hexnid, 16)
            for who in (name, alias):
                if not who:
                    continue
                by_lib.setdefault((lib, nid), [])
                if who not in by_lib[(lib, nid)]:
                    by_lib[(lib, nid)].append(who)
                by_nid.setdefault(nid, [])
                if who not in by_nid[nid]:
                    by_nid[nid].append(who)
    return by_lib, by_nid


def extra_records(path: Path) -> dict[tuple[str, int], list[str]]:
    """Curated (lib, NID) -> [name] lines from config/nid-extra.txt."""
    out: dict[tuple[str, int], list[str]] = {}
    if not path.is_file():
        return out
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        lib, hexnid, name = line.split()[:3]
        out.setdefault((lib, int(hexnid, 16)), []).append(name)
    return out


def sha1_nid(name: str) -> int:
    """The NID scheme: first 4 bytes of SHA-1(name), little-endian."""
    return int.from_bytes(hashlib.sha1(name.encode()).digest()[:4], "little")


def candidate_names(sdk: Path) -> set[str]:
    """Identifiers declared in pspsdk headers - the fallback name pool
    for NIDs pspsdk's import macros never mention (Sony-only functions).
    A SHA-1 collision between a wrong candidate and a retail NID costs
    nothing at this scale (~2^-32 per pair).
    """
    names: set[str] = set()
    ident = re.compile(r"[A-Za-z_][A-Za-z0-9_]{2,}")
    for path in sdk.rglob("*.h"):
        try:
            text = path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        names.update(ident.findall(text))
    return names


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    ap.add_argument("--sdk", type=Path, default=DEFAULT_SDK,
                    help="pspsdk checkout used as name source")
    ap.add_argument("--report", action="store_true",
                    help="print statistics and exit, no file")
    args = ap.parse_args()

    with pspelf.Module() as mod:
        imports = read_imports(mod)

    by_lib, by_nid = psp_pairs(args.sdk)
    libs = sorted({lib for _, lib, _, _ in imports})
    covered_libs = sum(1 for lib in libs
                       if any(k[0] == lib for k in by_lib))
    print(f"{len(imports)} imports across {len(libs)} libraries "
          f"({covered_libs} with pspsdk stub records)")
    print(f"{sum(len(v) for v in by_lib.values())} name pairs in "
          f"{len({p for p, _ in by_lib})} (lib, NID) records from {args.sdk}")

    # Sanity check of the hash scheme itself: recorded pairs from the
    # unobfuscated era must satisfy SHA-1(name)[:4] LE == NID (salted
    # later-firmware kernel NIDs legitimately fail it).
    pair_total = sum(len(v) for v in by_lib.values())
    agree = sum(1 for (lib, nid), names in by_lib.items()
                if sha1_nid(names[0]) == nid)
    print(f"SHA-1(name) agrees with {agree}/{pair_total} pspsdk records")

    # Curated records (psplibdoc) merge into the same tables.
    extra = extra_records(EXTRA)
    for (lib, nid), names in extra.items():
        for name in names:
            slot = by_lib.setdefault((lib, nid), [])
            if name not in slot:
                slot.append(name)
            nslot = by_nid.setdefault(nid, [])
            if name not in nslot:
                nslot.append(name)
    if extra:
        print(f"{len(extra)} curated records from "
              f"{EXTRA.relative_to(ROOT)}")

    # Match: (library, NID) first - the precise key; fall back to the bare
    # NID when the image's library spelling differs from pspsdk's (driver
    # vs user variant) as long as the name set is unambiguous.
    name_of: dict[int, str] = {}
    ambiguous = 0
    unmatched: list[tuple[str, int]] = []
    for _addr, lib, nid, _i in imports:
        if nid in name_of:
            continue
        names = by_lib.get((lib, nid)) or by_nid.get(nid)
        if not names:
            unmatched.append((lib, nid))
            continue
        if len(names) > 1:
            ambiguous += 1
        name_of[nid] = names[0]

    hit = len(name_of)
    print(f"recovered {hit}/{len(imports)} names "
          f"({ambiguous} with pspsdk-recorded aliases, first kept)")

    # Fill the leftovers by hashing the header-declared name pool: the
    # pair records only cover what homebrew links against, but the hash
    # confirms any name pspsdk at least mentions.
    filled: list[tuple[str, int, str]] = []
    if unmatched:
        pool = candidate_names(args.sdk)
        hash_index: dict[int, str] = {}
        for name in sorted(pool):          # sorted: deterministic winner
            hash_index.setdefault(sha1_nid(name), name)
        rest: list[tuple[str, int]] = []
        for lib, nid in unmatched:
            name = hash_index.get(nid)
            if name:
                name_of[nid] = name
                filled.append((lib, nid, name))
            else:
                rest.append((lib, nid))
        unmatched = rest
        if filled:
            print(f"SHA-1 matched {len(filled)} leftovers from "
                  f"{len(pool)} header names:")
            for lib, nid, name in filled:
                print(f"  {lib:<20} 0x{nid:08X} {name}")

    if unmatched:
        print(f"{len(unmatched)} unmatched:")
        for lib, nid in unmatched:
            print(f"  {lib:<20} 0x{nid:08X}")

    if args.report:
        return 0
    hit = len(name_of)   # recount: the SHA-1 fill may have added more
    if not hit:
        print("error: no names recovered at all - is --sdk pspsdk?",
              file=sys.stderr)
        return 1

    lines = ["# stub_addr   library             nid        name  "
             "(generated by tools/imports.py; ? = not recovered)\n"]
    recovered = 0
    for addr, lib, nid, _index in imports:
        name = name_of.get(nid, "")
        if name:
            recovered += 1
        lines.append(f"0x{addr:08X} {lib:<19} 0x{nid:08X} {name or '?'}\n")

    OUT.write_text("".join(lines), encoding="utf-8")
    print(f"wrote {OUT.relative_to(ROOT)}: {recovered}/{len(imports)} "
          f"names recovered")
    return 0


if __name__ == "__main__":
    sys.exit(main())

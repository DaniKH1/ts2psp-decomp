"""Extract the boot executable from the disc image.

`PSP_GAME/SYSDIR/BOOT.BIN` is the module in plain ELF form; `EBOOT.BIN`
beside it is the same thing behind the PSP's encryption wrapper, so only
the plain one is worth pulling.  Nothing extracted here is committed -
`disks/` is in .gitignore and the ISO stays where it is.
"""

import hashlib
import sys
from pathlib import Path

import pycdlib

ISO = Path(r"F:\Sims 2 PSP Decomp\pgs-si2.iso")
OUT_DIR = Path(__file__).resolve().parent.parent / "disks" / "pgs-si2"

# Both files under SYSDIR are pulled: BOOT.BIN is the plain ELF, EBOOT.BIN
# is the same module behind the ~PSP wrapper (and may differ in what it
# carries inside - the symbol table turned out to live there).
ISO_PATHS = ("/PSP_GAME/SYSDIR/BOOT.BIN", "/PSP_GAME/SYSDIR/EBOOT.BIN")

MAGICS = {b"\x7fELF": "ELF", b"~PSP": "~PSP"}


def main() -> int:
    OUT_DIR.mkdir(parents=True, exist_ok=True)

    iso = pycdlib.PyCdlib()
    iso.open(str(ISO))
    for iso_path in ISO_PATHS:
        out = OUT_DIR / Path(iso_path).name
        iso.get_file_from_iso(local_path=str(out), iso_path=iso_path)
        data = out.read_bytes()
        kind = MAGICS.get(data[:4], f"unknown (0x{data[:4].hex()})")
        sha1 = hashlib.sha1(data).hexdigest()
        print(f"extracted {iso_path} -> {out}")
        print(f"  {len(data)} bytes, {kind}, sha1 {sha1}")
    iso.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Check that a clone is ready to work on, and say what is missing if not.

The repository deliberately ships two things it cannot legally or sensibly
include: the decrypted module image (Maxis/EA material) and the psp toolchain
(a third-party build of 125 MB).  Everything else - the assembly, the C, the
symbol map, the reports - is here.  This script is the missing half of that
arrangement: it says which of the two you already have, which you do not, and
exactly how to point at the one you are missing.

    python tools/setup.py            # report
    python tools/setup.py --check    # exit non-zero if anything is missing

Nothing is downloaded.  A disc image and a compiler are the user's to supply,
and a script that fetched them would be the wrong place to put that choice.
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import paths  # noqa: E402

# The toolchain builds this project; splat and spimdisasm produced the asm that
# is already committed, so they are only needed to regenerate it.
TOOLCHAIN_TOOLS = ["psp-gcc", "psp-as", "psp-ld"]

README = """\
The two things this repository does not contain, and how to supply them.

  1. The module image - the reference every comparison is made against.

     Expected at:  {elf}
     {elf_state}

     This is the decrypted executable out of a retail disc of The Sims 2 for
     PSP.  It is EA's copyrighted material and cannot be redistributed, so
     `tools/setup.py` will not fetch it and `.gitignore` keeps it out.  Extract
     it from your own copy of the disc:

         EBOOT.BIN          the executable, as it appears on the disc
         pgs-si2/EBOOT.dec  the same file after decryption

     `tools/iso9660.py` and `tools/split.py` do the extraction and decryption;
     progress.md records the original sha1 so you can confirm you have the
     right build.

  2. The toolchain - psp-gcc, psp-as, psp-ld.

     Expected at:  {pspdev}
     {pspdev_state}

     Also not committed, and also third-party.  A Windows build of psp-dev is
     published by the pspdev project; the 125 MB zip goes to `bin/`, which is
     ignored.

  Once you have one or both of them, point at them:

      set TS2PSP_ELF=F:\\path\\to\\EBOOT.dec
      set TS2PSP_PSPDEV=F:\\path\\to\\pspdev\\bin

  (On POSIX: export TS2PSP_ELF=... and export TS2PSP_PSPDEV=...)

  Then check with:

      python tools/setup.py --check
      python tools/check_image.py
"""


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument(
        "--check",
        action="store_true",
        help="exit non-zero if the module image or the toolchain is missing",
    )
    args = ap.parse_args()

    elf_ok = paths.elf_present()
    tc_ok = paths.toolchain_present()

    missing_tools = [t for t in TOOLCHAIN_TOOLS if not (paths.PSP_BIN / t).is_file()]

    print(paths.describe())
    print()

    if not tc_ok:
        if missing_tools:
            print(f"toolchain incomplete: {', '.join(missing_tools)} not found")
        else:
            print("toolchain looks complete")
    if not elf_ok:
        print("module image not found")

    if args.check and not (elf_ok and tc_ok):
        print()
        print(README.format(
            elf=paths.ELF_PATH,
            elf_state="found" if elf_ok else "MISSING",
            pspdev=paths.PSP_BIN,
            pspdev_state="found" if tc_ok else "MISSING",
        ))
        return 1

    if elf_ok and tc_ok:
        print("\nready: run `python tools/split.py --build` to regenerate and compare")
        return 0
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
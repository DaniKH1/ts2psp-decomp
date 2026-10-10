"""Where the PSP toolchain lives and how to invoke it.

The toolchain is built from source under MSYS2 into $PSPDEV (C:\\pspdev by
default) - see README "Toolchain".  The binaries are MSYS-hosted, so they need
C:\\msys64\\usr\\bin on PATH for msys-2.0.dll, and they insist on POSIX paths
for temporary files: a Windows TEMP is not reachable through their /cygdrive
view, which is why TMPDIR is forced to /tmp.
"""

from __future__ import annotations

import os
import shutil
import sys
from pathlib import Path

PSPDEV = Path(os.environ.get("PSPDEV", r"C:\pspdev"))
MSYS_BIN = Path(os.environ.get("MSYS_BIN", r"C:\msys64\usr\bin"))
PSP_BIN = PSPDEV / "bin"

# The retail compiler flags, as identified from the ELF header and section
# layout (README "Compiler"): psp-gcc, -G0 (no small data - not one
# R_MIPS_GPREL16 relocation in the image), eabi, allegrex, non-PIC absolute
# addressing, per-TU function sections.
CPPFLAGS = ["-Iinclude"]
CFLAGS = [
    "-G0", "-mabi=eabi", "-march=allegrex",
    "-fno-pic", "-fno-common", "-ffunction-sections", "-fdata-sections",
    "-fno-strict-aliasing",
]
CXXFLAGS = CFLAGS + ["-fno-exceptions", "-fno-rtti", "-std=gnu++98"]
# Kept out of CFLAGS: tools/verify_c.py may override it per candidate.
OPTFLAGS = ["-O2"]


def tool(name: str) -> str:
    return str(PSP_BIN / f"{name}.exe")


def have_toolchain() -> bool:
    return (PSP_BIN / "psp-gcc.exe").exists()


def require_toolchain() -> None:
    if not have_toolchain():
        sys.exit(f"psp-gcc not found in {PSP_BIN} - the toolchain build is "
                 f"still in progress (see README 'Toolchain')")


def env() -> dict:
    e = dict(os.environ)
    e["PSPDEV"] = str(PSPDEV)
    e["PATH"] = os.pathsep.join([str(MSYS_BIN), str(PSP_BIN), e.get("PATH", "")])
    e["TMPDIR"] = "/tmp"
    e["TMP"] = "/tmp"
    e["TEMP"] = "/tmp"
    return e

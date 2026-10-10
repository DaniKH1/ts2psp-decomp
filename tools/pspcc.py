"""Where the PSP toolchains live and how to invoke them.

Two lanes are tried per candidate (README "Compiler era"):

  * C:\\pspdev    - GCC 15.2.0, built from source under MSYS2 (modern lane);
  * C:\\pspdev46  - GCC 4.6.4 (era-adjacent lane, closest to the retail
                    2007 build; only offered once its psp-gcc is installed).

A lane counts as available when its bin/psp-gcc.exe exists.  Both are
MSYS-hosted, so they need C:\\msys64\\usr\\bin on PATH for msys-2.0.dll,
and they insist on POSIX paths for temporary files: a Windows TEMP is not
reachable through their /cygdrive view, which is why TMPDIR is forced to
/tmp.
"""

from __future__ import annotations

import os
import sys
from pathlib import Path

# (pspdev root, lane label); PSPDEV / PSPDEV46 env vars override the roots.
LANES: list[tuple[Path, str]] = [
    (Path(os.environ.get("PSPDEV", r"C:\pspdev")), "gcc15"),
    (Path(os.environ.get("PSPDEV46", r"C:\pspdev46")), "gcc46"),
]
MSYS_BIN = Path(os.environ.get("MSYS_BIN", r"C:\msys64\usr\bin"))

# The retail compiler flags, as identified from the ELF header and section
# layout (README "Compiler"): psp-gcc, -G0 (no small data - not one
# R_MIPS_GPREL16 relocation in the image), eabi, allegrex, non-PIC absolute
# addressing, per-TU function sections.  Two more flags are derived from
# observed retail code (README "Compiler era"):
#   -mpreferred-stack-boundary=4  every retail frame is 16-byte aligned
#                                 (e.g. func_0004AB18: sp-32, ra at 16);
#   -fno-optimize-sibling-calls   retail contains 559 call-wrappers that
#                                 keep frame+jal+jr and zero true sibling
#                                 calls (j in tail position).
CPPFLAGS = ["-Iinclude"]
CFLAGS = [
    "-G0", "-mabi=eabi", "-march=allegrex",
    "-fno-pic", "-fno-common", "-ffunction-sections", "-fdata-sections",
    "-fno-strict-aliasing",
    "-mpreferred-stack-boundary=4", "-fno-optimize-sibling-calls",
]
CXXFLAGS = CFLAGS + ["-fno-exceptions", "-fno-rtti", "-std=gnu++98"]
# Kept out of CFLAGS: tools/verify_c.py may override it per candidate.
OPTFLAGS = ["-O2"]


def lanes() -> list[tuple[Path, str]]:
    """(root, label) for every lane whose psp-gcc is installed."""
    return [(root, label) for root, label in LANES
            if (root / "bin" / "psp-gcc.exe").exists()]


def tool(name: str, root: Path | None = None) -> str:
    base = root if root is not None else LANES[0][0]
    return str(base / "bin" / f"{name}.exe")


def have_toolchain() -> bool:
    return bool(lanes())


def require_toolchain() -> None:
    if not have_toolchain():
        roots = ", ".join(str(r) for r, _ in LANES)
        sys.exit(f"no psp-gcc found ({roots}) - the toolchain build is "
                 f"still in progress (see README 'Toolchain')")


def env(root: Path | None = None) -> dict:
    base = root if root is not None else LANES[0][0]
    e = dict(os.environ)
    e["PSPDEV"] = str(base)
    e["PATH"] = os.pathsep.join(
        [str(MSYS_BIN), str(base / "bin"), e.get("PATH", "")])
    e["TMPDIR"] = "/tmp"
    e["TMP"] = "/tmp"
    e["TEMP"] = "/tmp"
    return e

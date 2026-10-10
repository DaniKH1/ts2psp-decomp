"""Where the toolchains live and how to invoke them.

Lanes are tried per candidate (README "Compiler era"), era order:

  * C:\\pspdev33 - GCC 3.3 with the original 2005 allegrex port (the Sony
                   official-SDK compiler family: psp-gcc 1.x = GCC 3.3.x).
                   Built `all-gcc` only under MSYS2: it ships no
                   binutils/newlib of its own and *borrows* another lane's
                   `psp-as`/`psp-ld`/`psp-objcopy`, since instruction
                   encoding is ISA-determined and identical across
                   binutils versions - keeping the assembler fixed
                   isolates the cc1 version as the single variable;
  * C:\\pspdev    - GCC 15.2.0, built from source under MSYS2 (modern lane);
  * C:\\pspdev46  - GCC 4.6.4 (oldest upstream allegrex branch).

A lane counts as available when its bin/psp-gcc.exe exists.  All are
MSYS-hosted, so they need C:\\msys64\\usr\\bin on PATH for msys-2.0.dll,
and they insist on POSIX paths for temporary files: a Windows TEMP is not
reachable through their /cygdrive view, which is why TMPDIR is forced to
/tmp.
"""

from __future__ import annotations

import os
import sys
from pathlib import Path

# (pspdev root, lane label); PSPDEV / PSPDEV46 / PSPDEV33 env vars override.
LANES: list[tuple[Path, str]] = [
    (Path(os.environ.get("PSPDEV33", r"C:\pspdev33")), "gcc33"),
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

# Flags a lane's driver does not know (old drivers hard-error on unknown
# -m options).  Filled empirically per lane; see tools/verify_c.py runs.
LANE_CFLAGS_DROP: dict[str, tuple[str, ...]] = {
    # GCC 3.3.6's driver does not recognize this later GCC option.
    "gcc33": ("-mpreferred-stack-boundary=4",),
}


def lane_label(root: Path | None = None) -> str:
    base = root if root is not None else LANES[0][0]
    for r, label in LANES:
        if r == base:
            return label
    return ""


def cflags(root: Path | None = None) -> list[str]:
    """CFLAGS minus what this lane's compiler cannot parse."""
    drop = LANE_CFLAGS_DROP.get(lane_label(root), ())
    return [f for f in CFLAGS if f not in drop]


def lanes() -> list[tuple[Path, str]]:
    """(root, label) for every lane whose psp-gcc is installed."""
    return [(root, label) for root, label in LANES
            if (root / "bin" / "psp-gcc.exe").exists()]


def tool(name: str, root: Path | None = None) -> str:
    base = root if root is not None else LANES[0][0]
    exe = base / "bin" / f"{name}.exe"
    if exe.exists():
        return str(exe)
    # gcc-only lanes (gcc33 ships no binutils of its own) borrow another
    # lane's tool: instruction encoding is identical across binutils
    # versions, so this keeps the cc1 version as the only variable.
    for alt, _ in LANES:
        cand = alt / "bin" / f"{name}.exe"
        if cand.exists():
            return str(cand)
    return str(exe)  # nothing found; the caller surfaces the error


def have_toolchain() -> bool:
    return bool(lanes())


def require_toolchain() -> None:
    if not have_toolchain():
        roots = ", ".join(str(r) for r, _ in LANES)
        sys.exit(f"no psp-gcc found ({roots}) - the toolchain build is "
                 f"still in progress (see README 'Toolchain')")


def env(root: Path | None = None) -> dict:
    base = root if root is not None else LANES[0][0]
    # GCC 3.3's specs call the assembler as plain `as`; its borrowed psp-as
    # alias must precede MSYS's host assembler, while MSYS stays on PATH for
    # the runtime DLLs.  The newer drivers locate their target tools directly.
    if lane_label(base) == "gcc33":
        bins = [str(base / "bin"), str(MSYS_BIN)]
    else:
        bins = [str(MSYS_BIN), str(base / "bin")]
    if not (base / "bin" / "psp-as.exe").exists():
        # gcc-only lane: the driver must still find an assembler, so put the
        # first lane that has one on PATH after the lane's own bin.
        for alt, _ in LANES:
            if (alt / "bin" / "psp-as.exe").exists():
                bins.append(str(alt / "bin"))
                break
    e = dict(os.environ)
    e["PSPDEV"] = str(base)
    e["PATH"] = os.pathsep.join(bins + [e.get("PATH", "")])
    e["TMPDIR"] = "/tmp"
    e["TMP"] = "/tmp"
    e["TEMP"] = "/tmp"
    return e

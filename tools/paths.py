#!/usr/bin/env python3
"""Where the target module image and the psp toolchain live.

The repository contains neither, on purpose.  The decrypted executable is
Maxis/EA material and the psp-gcc toolchain is a third-party build of well over
a hundred megabytes; `.gitignore` keeps both out.  A clone therefore has to be
*pointed* at its own copy rather than have the files dropped in, so both paths
are resolved here and every tool asks this module instead of guessing.

    TS2PSP_ELF      path to the decrypted module image
                    (disks/pgs-si2/EBOOT.dec by default)
    TS2PSP_PSPDEV   path to the psp toolchain's bin directory
                    (bin/pspdev/bin by default)
    TS2PSP_MSYS     path to the MSYS2/Cygwin bin directory, needed only to
                    host the psp toolchain
                    (C:/msys64/usr/bin by default)

The first two fall back to their in-tree location, so a working copy that *does*
have them in place needs no environment at all.  `tools/setup.py` reports what
is missing.

If exporting variables is inconvenient, an optional `local_paths.txt` at the
repository root does the same job and is gitignored:

    # local_paths.txt - this machine only, not committed
    TS2PSP_ELF=D:/decomp/disks/pgs-si2/EBOOT.dec
    TS2PSP_PSPDEV=D:/decomp/bin/pspdev/bin

Blank lines and `#` comments are ignored.  The environment wins over the file.
"""
from __future__ import annotations

import os
from pathlib import Path

# Repository root, i.e. the parent of `tools/`.
ROOT = Path(__file__).resolve().parent.parent

LOCAL_PATHS_FILE = ROOT / "local_paths.txt"


def _from_file(name: str) -> str | None:
    """Read one `NAME=value` line out of `local_paths.txt`, if it is there."""
    try:
        text = LOCAL_PATHS_FILE.read_text(encoding="utf-8")
    except OSError:
        return None
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        key, sep, value = line.partition("=")
        if sep and key.strip() == name:
            return value.strip()
    return None


def _resolve(env_var: str, default: Path) -> Path:
    """Where one input lives: environment, then `local_paths.txt`, then default.

    Resolved to an absolute path so the result does not depend on the working
    directory a tool happens to be invoked from.
    """
    value = os.environ.get(env_var) or _from_file(env_var)
    return Path(value).resolve() if value else default


# The decrypted module image: the reference every comparison is made against.
ELF_PATH = _resolve("TS2PSP_ELF", ROOT / "disks" / "pgs-si2" / "EBOOT.dec")

# The psp toolchain's bin directory: psp-gcc, psp-as, psp-ld.
PSP_BIN = _resolve("TS2PSP_PSPDEV", ROOT / "bin" / "pspdev" / "bin")

# The MSYS2/Cygwin bin directory, which hosts the psp toolchain.  The Windows
# build of psp-dev is a Cygwin build and its temporary files have to be reachable
# through its POSIX view, so a Windows TEMP override is not enough.
MSYS_BIN = _resolve("TS2PSP_MSYS", Path("C:/msys64/usr/bin"))


def find_tool(name: str) -> Path | None:
    """Locate a toolchain binary, allowing for Windows' `.exe` suffix.

    The psp-dev Windows build installs `psp-gcc.exe`; a POSIX build installs
    `psp-gcc`.  Anything that checks the toolchain has to accept both, or it
    reports a complete install as missing.
    """
    for candidate in (name, f"{name}.exe"):
        path = PSP_BIN / candidate
        if path.is_file():
            return path
    return None


def elf_present() -> bool:
    """True if the module image is where the tools expect it."""
    return ELF_PATH.is_file()


def toolchain_present() -> bool:
    """True if the toolchain looks complete enough to build with."""
    return find_tool("psp-gcc") is not None and find_tool("psp-ld") is not None


def describe() -> str:
    """A short report of both paths and whether they are usable."""
    lines = [
        f"module image : {ELF_PATH}"
        + ("" if elf_present() else "   [MISSING]"),
        f"toolchain    : {PSP_BIN}"
        + ("" if toolchain_present() else "   [MISSING]"),
        f"msys2 host   : {MSYS_BIN}"
        + ("" if (MSYS_BIN / "gcc.exe").is_file() else "   [MISSING]"),
    ]
    return "\n".join(lines)
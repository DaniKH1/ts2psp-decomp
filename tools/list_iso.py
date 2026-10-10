"""List the directory tree of the disc image, without extracting anything.

The ISO itself must never be committed; this only prints what is on it.
"""

import sys
from pathlib import Path

import pycdlib

ISO = Path(r"F:\Sims 2 PSP Decomp\pgs-si2.iso")


def walk(iso: pycdlib.PyCdlib, iso_path: str, depth: int = 0, max_depth: int = 3) -> None:
    for child in iso.list_children(iso_path=iso_path):
        ident = child.file_identifier
        name = (ident() if callable(ident) else ident).decode("utf-8", "replace")
        if name in (".", ".."):
            continue
        if child.is_dir():
            print("  " * depth + name + "/")
            if depth < max_depth:
                walk(iso, iso_path.rstrip("/") + "/" + name, depth + 1, max_depth)
        else:
            print("  " * depth + f"{name} ({child.data_length} bytes)")


def main() -> int:
    iso = pycdlib.PyCdlib()
    iso.open(str(ISO))
    walk(iso, "/")
    iso.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())

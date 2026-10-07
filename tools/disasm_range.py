"""Disassemble a window of the EBOOT with its relocation entries annotated."""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import mipsdis  # noqa: E402
import pspelf  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--elf", default=str(ELF_PATH))
    ap.add_argument("--start", required=True, type=lambda s: int(s, 0))
    ap.add_argument("--count", type=int, default=24)
    ap.add_argument("--bytes", action="store_true",
                    help="show raw words instead of disassembly")
    ns = ap.parse_args()

    elf = pspelf.load(ns.elf)
    relocs = {r.offset: r for r in elf.relocs()}
    for i in range(ns.count):
        off = ns.start + i * 4
        word = struct.unpack("<I", elf.read(off, 4))[0]
        rel = relocs.get(off)
        tag = ""
        if rel is not None:
            tag = f"  ; {rel.type_name} -> {rel.target:#x}"
        text = f"{word:08x}" if ns.bytes else str(mipsdis.make_instruction(word, off))
        print(f"{off:#010x}: {text}{tag}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

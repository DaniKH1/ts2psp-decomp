"""Realign the built module's second loadable segment to the original layout.

PSPLINK writes the data segment at an 8-byte aligned *file* offset, so the
original has four padding bytes between the end of `.dtors` and the start of
`.cplinit`.  Reproducing the addresses alone (`AT(ADDR)` in the linker script)
does not reproduce that padding: GNU ld lays the segment out at whatever offset
`p_align` implies, and `-z max-page-size=4` - which segment 0 needs so that it
starts at file offset 0x74, right after the ELF header - pins `p_align` to 4.

`max-page-size` is global, so no linker flag can give the two segments the
different alignments the original has (4 and 8).  The fix is therefore a
post-link pass on the ELF, which is entirely mechanical:

* insert `p_align` padding bytes in front of the segment,
* fix `p_offset` on the segment and `p_align` itself,
* shift `sh_offset` of every section at or past an insertion point, and
  `e_shoff` with them.

Nothing in a PSP module is addressed by file offset at runtime - the loader maps
`p_offset` -> `p_vaddr` and relocations work off the address - so this only
changes the bytes of the image on disk, which is exactly what has to match.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402

PT_LOAD = 1


def load_segments(elf) -> list[dict]:
    return [s for s in elf.segments if s["type"] in (PT_LOAD, "PT_LOAD")]


def plan_insertions(want: list[dict], got: list[dict]) -> list[dict]:
    """Where padding has to go: one entry per segment that needs realignment.

    Offsets are the *pre-insertion* offsets of the built file; `shift` is how
    far everything from there on has to move.
    """
    out: list[dict] = []
    shift = 0
    for index, (w, g) in enumerate(zip(want, got)):
        pad = w["offset"] - (g["offset"] + shift)
        align = w["align"]
        # The original's own offset is already `align`-aligned, so it only needs
        # rounding up when the built file would otherwise run past it.
        if pad < 0:
            pad = -((-pad) // align) * align
        if pad <= 0 and g["align"] == align:
            continue
        out.append({"index": index, "at": g["offset"] + shift,
                    "pad": max(pad, 0), "align": align, "shift": shift})
        shift += max(pad, 0)
    return out


def shifted(offset: int, plan: list[dict]) -> int:
    """Apply every insertion at or before `offset`."""
    move = 0
    for step in plan:
        if offset >= step["at"]:
            move += step["pad"]
    return offset + move


def fix_headers(raw: bytearray, plan: list[dict]) -> None:
    """Update the program and section headers in place."""
    e_phoff, = struct.unpack_from("<I", raw, 0x1C)
    e_shoff, = struct.unpack_from("<I", raw, 0x20)
    e_phentsize, e_phnum = struct.unpack_from("<HH", raw, 0x2A)
    e_shentsize, e_shnum = struct.unpack_from("<HH", raw, 0x2E)

    align_for = {step["index"]: step["align"] for step in plan}

    for i in range(e_phnum):
        at = e_phoff + i * e_phentsize
        if struct.unpack_from("<I", raw, at)[0] not in (PT_LOAD, "PT_LOAD"):
            continue
        p_offset, = struct.unpack_from("<I", raw, at + 4)
        new = shifted(p_offset, plan)
        if new != p_offset:
            struct.pack_into("<I", raw, at + 4, new)
        if i in align_for:
            # Elf32_Phdr: type 0, offset 4, vaddr 8, paddr 12, filesz 16,
            # memsz 20, flags 24, align 28.
            struct.pack_into("<I", raw, at + 28, align_for[i])

    if e_shoff and e_shnum:
        # The table itself moves with the insert, so it has to be walked at its
        # *new* offset - patching it in place at the old one corrupts the ELF.
        e_shoff = shifted(e_shoff, plan)
        struct.pack_into("<I", raw, 0x20, e_shoff)
        for i in range(e_shnum):
            at = e_shoff + i * e_shentsize
            sh_offset, = struct.unpack_from("<I", raw, at + 0x10)
            struct.pack_into("<I", raw, at + 0x10, shifted(sh_offset, plan))


def realign(target: str, built: str) -> int:
    want = load_segments(pspelf.load(target))
    got = load_segments(pspelf.load(built))
    if len(want) != len(got):
        print(f"segment count differs ({len(got)} vs {len(want)}), skipping",
              file=sys.stderr)
        return 0

    plan = plan_insertions(want, got)
    if not plan:
        print("segment layout already matches the original")
        return 0

    raw = bytearray(Path(built).read_bytes())
    # Back to front, so the offsets earlier steps recorded stay valid.
    for step in sorted(plan, key=lambda s: -s["at"]):
        if not step["pad"]:
            continue
        at = step["at"]
        raw[at:at] = b"\x00" * step["pad"]
        print(f"seg{step['index']}: {step['pad']} padding byte(s) at {at:#x}, "
              f"p_align -> {step['align']:#x}")

    fix_headers(raw, plan)
    Path(built).write_bytes(bytes(raw))
    print(f"wrote {built} ({len(raw)} bytes)")
    return sum(step["pad"] for step in plan)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ns = ap.parse_args()
    realign(ns.target, ns.built)
    return 0


if __name__ == "__main__":
    sys.exit(main())

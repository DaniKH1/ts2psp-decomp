"""Minimal ISO9660 / UDF reader for PSP UMD images.

The Sims 2 PSP image ("pgs-si2.iso") carries a hybrid ISO9660 + UDF
filesystem.  Sony's own tooling (and psp-gcc's linker script for UMD
images) walks the UDF side, but the ISO9660 directory records describe the
same extent, so a plain ISO9660 walk is enough to enumerate and extract the
payload without any third party dependency.
"""

from __future__ import annotations

import argparse
import os
import sys
from dataclasses import dataclass


@dataclass
class DirRecord:
    length: int
    ext_attr_length: int
    extent: int
    data_length: int
    flags: int
    name: str

    @property
    def is_dir(self) -> bool:
        return bool(self.flags & 0x02)


class Iso9660:
    def __init__(self, path: str):
        self.path = path
        self.f = open(path, "rb")
        self.size = os.path.getsize(path)
        # PSP UMD images are always 2048 byte sectors; the descriptor block
        # size is read from the PVD itself once it has been located.
        self.sector_size = 2048
        pvd = self._find_pvd()
        self.block_size = pvd[128]
        root = self._parse_record(pvd[156:156 + 34])
        self.root_extent = root.extent
        self.root_length = root.data_length

    # -- low level ---------------------------------------------------
    def _find_pvd(self) -> bytes:
        self.f.seek(16 * self.sector_size)
        for _ in range(32):
            block = self.f.read(self.sector_size)
            if len(block) < 2048:
                break
            if block[1:6] != b"CD001":
                continue
            if block[0] == 1:
                return block
        raise ValueError("no primary volume descriptor found")

    @staticmethod
    def _parse_record(raw: bytes) -> DirRecord:
        length = raw[0]
        extent = int.from_bytes(raw[2:6], "little")
        data_length = int.from_bytes(raw[10:14], "little")
        flags = raw[25]
        name_len = raw[32]
        name = raw[33:33 + name_len].decode("latin-1")
        return DirRecord(length, raw[1], extent, data_length, flags, name)

    def _read_dir(self, extent: int, length: int) -> list[DirRecord]:
        self.f.seek(extent * self.sector_size)
        data = self.f.read(length)
        out: list[DirRecord] = []
        off = 0
        while off < len(data):
            rec_len = data[off]
            if rec_len == 0:
                # Records never straddle a sector boundary.
                off = (off // self.sector_size + 1) * self.sector_size
                if off >= len(data):
                    break
                continue
            raw = data[off:off + rec_len]
            if len(raw) < 33:
                break
            out.append(self._parse_record(raw))
            off += rec_len
        return out

    # -- public ------------------------------------------------------
    def listdir(self, path: str = "/") -> list[tuple[str, int, int, bool]]:
        """Return [(name, extent, length, is_dir)] for `path`."""
        if path in ("/", ""):
            extent, length = self.root_extent, self.root_length
        else:
            cur = (self.root_extent, self.root_length)
            for part in [p for p in path.split("/") if p]:
                found = None
                for rec in self._read_dir(*cur):
                    if rec.name.split(";")[0].lower() == part.lower():
                        found = rec
                        break
                if found is None:
                    raise FileNotFoundError(path)
                cur = (found.extent, found.data_length)
            extent, length = cur
        out = []
        for rec in self._read_dir(extent, length):
            if rec.name in ("\x00", "\x01"):
                continue
            name = rec.name.split(";")[0]
            if name in (".", ".."):
                continue
            out.append((name, rec.extent, rec.data_length, rec.is_dir))
        return sorted(out, key=lambda t: (not t[3], t[0].lower()))

    def read(self, extent: int, length: int) -> bytes:
        self.f.seek(extent * self.sector_size)
        return self.f.read(length)

    def find(self, path: str) -> tuple[int, int]:
        parts = [p for p in path.split("/") if p]
        extent, length = self.root_extent, self.root_length
        for part in parts:
            found = None
            for rec in self._read_dir(extent, length):
                if rec.name.split(";")[0].lower() == part.lower():
                    found = rec
                    break
            if found is None:
                raise FileNotFoundError(path)
            extent, length = found.extent, found.data_length
        return extent, length

    def extract(self, path: str, dest: str) -> None:
        os.makedirs(dest, exist_ok=True)
        parts = [p for p in path.split("/") if p]
        cur = ""
        for part in parts[:-1]:
            cur = os.path.join(cur, part)
            os.makedirs(os.path.join(dest, cur), exist_ok=True)
        extent, length = self.find(path)
        out = os.path.join(dest, *parts)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        remaining = length
        self.f.seek(extent * self.sector_size)
        with open(out, "wb") as g:
            while remaining > 0:
                chunk = self.f.read(min(remaining, 1 << 20))
                if not chunk:
                    break
                g.write(chunk)
                remaining -= len(chunk)
        print(f"extracted {out} ({length} bytes)")

    def walk(self, path: str = "/"):
        for name, extent, length, is_dir in self.listdir(path):
            full = f"{path.rstrip('/')}/{name}"
            yield full, extent, length, is_dir
            if is_dir:
                yield from self.walk(full)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("iso")
    ap.add_argument("cmd", nargs="?", default="ls",
                    choices=["ls", "extract", "info"])
    ap.add_argument("args", nargs="*")
    ns = ap.parse_args()
    iso = Iso9660(ns.iso)
    if ns.cmd == "info":
        print(f"block size      : {iso.block_size}")
        print(f"root extent     : {iso.root_extent}")
        print(f"root length     : {iso.root_length}")
        print(f"image size      : {iso.size}")
    elif ns.cmd == "ls":
        target = ns.args[0] if ns.args else "/"
        for full, extent, length, is_dir in iso.walk(target):
            kind = "d" if is_dir else "-"
            print(f"{kind} {length:>12} {extent:>8} {full}")
    else:
        iso.extract(ns.args[0], ns.args[1])
    return 0


if __name__ == "__main__":
    sys.exit(main())

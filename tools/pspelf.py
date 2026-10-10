"""Read-only access to the retail module image (BOOT.BIN).

The symbol table was stripped, so everything else in this repository is built
on two facts this module exposes directly: the section table (addresses and
sizes) and the raw bytes at a virtual address.  Nothing here writes.
"""

from __future__ import annotations

from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
ELF_PATH = ROOT / "disks" / "pgs-si2" / "BOOT.BIN"


class Module:
    """The retail ELF: section map + byte access."""

    def __init__(self, path: Path = ELF_PATH):
        self.path = Path(path)
        self._file = open(path, "rb")
        self._elf = ELFFile(self._file)
        self.entry = self._elf.header["e_entry"]
        # name -> (addr, size, flags); only SHF_ALLOC sections carry load
        # addresses, the rest (.rel*, .symtab, ...) are build-time metadata.
        self.sections: dict[str, tuple[int, int, int]] = {}
        for sec in self._elf.iter_sections():
            flags = sec["sh_flags"]
            if flags & 0x2:  # SHF_ALLOC
                self.sections[sec.name] = (sec["sh_addr"], sec["sh_size"], flags)
        # Sorted list of (addr, end, name) over every allocated section, for
        # "does this address point at real image data?" queries.
        self._ranges = sorted(
            (addr, addr + size, name)
            for name, (addr, size, _flags) in self.sections.items()
            if size > 0
        )

    def close(self) -> None:
        self._file.close()

    def __enter__(self) -> "Module":
        return self

    def __exit__(self, *exc) -> None:
        self.close()

    def read(self, addr: int, size: int) -> bytes:
        """Raw bytes of [addr, addr+size) as the loader sees them."""
        for seg in self._elf.iter_segments():
            h = seg.header
            if h["p_type"] == "PT_LOAD" and h["p_vaddr"] <= addr < h["p_vaddr"] + h["p_filesz"]:
                off = h["p_offset"] + (addr - h["p_vaddr"])
                self._file.seek(off)
                return self._file.read(size)
        raise ValueError(f"address 0x{addr:08X} is not in any PT_LOAD segment")

    def containing_section(self, addr: int) -> str | None:
        """Name of the allocated section containing `addr`, else None."""
        for start, end, name in self._ranges:
            if start <= addr < end:
                return name
        return None

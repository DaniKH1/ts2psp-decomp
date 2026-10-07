"""ELF access layer for the The Sims 2 PSP executable.

The retail EBOOT.BIN is a Sony-signed (`~PSP` header) and encrypted PRX; once
decrypted it is a plain 32-bit little endian MIPS (N32/EABI32) ELF whose
sections carry the original `gcc -ffunction-sections` names
(`.text.collision`, `.text.drawing`, ...).  Those names plus the surviving
`.rel.*` tables are the backbone of the decompilation: every function and data
object can be recovered even though the symbol table was stripped.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field

from elftools.elf.elffile import ELFFile

SHT_NULL = 0
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8
SHT_REL = 9
SHT_MIPS_RELA = 0x700000A0

SHF_WRITE = 0x1
SHF_ALLOC = 0x2
SHF_EXECINSTR = 0x4

R_MIPS_NONE = 0
R_MIPS_16 = 1
R_MIPS_32 = 2
R_MIPS_REL32 = 3
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6
R_MIPS_GPREL16 = 7
R_MIPS_LITERAL = 8
R_MIPS_GOT16 = 9
R_MIPS_PC16 = 10
R_MIPS_CALL16 = 11
R_MIPS_GPREL32 = 12

RELOC_NAMES = {
    R_MIPS_NONE: "R_MIPS_NONE",
    R_MIPS_16: "R_MIPS_16",
    R_MIPS_32: "R_MIPS_32",
    R_MIPS_REL32: "R_MIPS_REL32",
    R_MIPS_26: "R_MIPS_26",
    R_MIPS_HI16: "R_MIPS_HI16",
    R_MIPS_LO16: "R_MIPS_LO16",
    R_MIPS_GPREL16: "R_MIPS_GPREL16",
    R_MIPS_LITERAL: "R_MIPS_LITERAL",
    R_MIPS_GOT16: "R_MIPS_GOT16",
    R_MIPS_PC16: "R_MIPS_PC16",
    R_MIPS_CALL16: "R_MIPS_CALL16",
    R_MIPS_GPREL32: "R_MIPS_GPREL32",
}


@dataclass
class Section:
    index: int
    name: str
    type: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    addralign: int
    entsize: int
    data: bytes = b""

    @property
    def is_text(self) -> bool:
        return bool(self.flags & SHF_EXECINSTR) and self.type == SHT_PROGBITS

    @property
    def is_data(self) -> bool:
        return self.type == SHT_PROGBITS and not (self.flags & SHF_EXECINSTR)

    @property
    def is_bss(self) -> bool:
        return self.type == SHT_NOBITS

    @property
    def is_rel(self) -> bool:
        return self.type in (SHT_REL, SHT_RELA, SHT_MIPS_RELA)

    def contains_addr(self, addr: int) -> bool:
        return self.addr <= addr < self.addr + self.size

    def __repr__(self) -> str:
        return (f"<Section {self.name} addr={self.addr:#x} "
                f"off={self.offset:#x} size={self.size:#x}>")


@dataclass
class Reloc:
    """A single relocation entry (`SHT_MIPS_RELA`: offset + info + addend)."""

    section: str          # name of the section the relocation applies to
    offset: int           # vaddr of the word being patched
    type: int
    symbol: int
    addend: int
    target: int = 0       # resolved target vaddr (addend for RELA)
    resolved: bool = False  # True once the target has been recovered

    @property
    def type_name(self) -> str:
        return RELOC_NAMES.get(self.type, f"R_MIPS_{self.type}")


# Opcodes whose `%lo` operand is a memory offset, or an `addiu` that adds to
# the register the matching `lui` wrote into.
LO_USING_OPCODES = frozenset(
    [0x09,             # addiu $rt, $rs, lo
     0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B,
     # integer loads/stores, including the unaligned lwl/lwr/swl/swr pair
     0x31, 0x33, 0x35, 0x39, 0x3B, 0x3D])               # cop1 loads/stores


def lo_base_register(word: int) -> int | None:
    """Register a `%lo` immediate is added to, or None if not a `%lo` user."""
    opcode = word >> 26
    if opcode in LO_USING_OPCODES:
        return (word >> 21) & 0x1F
    return None


@dataclass
class Elf:
    path: str
    header: dict
    size: int
    segments: list = field(default_factory=list)
    sections: list = field(default_factory=list)
    # offset -> (hi16 offset, lo16 offset) for %hi/%lo relocation pairs
    hilo_pairs: dict = field(default_factory=dict)

    # -- section helpers ------------------------------------------------
    def section(self, name: str) -> Section | None:
        for sec in self.sections:
            if sec.name == name:
                return sec
        return None

    def text_sections(self) -> list[Section]:
        return [s for s in self.sections if s.is_text and s.size > 0]

    def data_sections(self) -> list[Section]:
        return [s for s in self.sections
                if s.is_data and s.size > 0 and s.flags & SHF_ALLOC]

    def section_of(self, addr: int) -> Section | None:
        for sec in self.sections:
            if sec.size and sec.addr <= addr < sec.addr + sec.size:
                return sec
        return None

    def read(self, addr: int, size: int) -> bytes:
        sec = self.section_of(addr)
        if sec is None:
            return b""
        off = addr - sec.addr
        return sec.data[off:off + size]

    # -- relocations ----------------------------------------------------
    def relocs(self) -> list[Reloc]:
        """Every relocation, resolved to the vaddr it points at.

        PSPLINK keeps the reference *in place* (SHT_MIPS_REL, 8 byte entries,
        no addend), so the target has to be recovered from the patched word.
        `R_MIPS_32` stores the address directly, `R_MIPS_26` holds the indexed
        target in its low 26 bits and `R_MIPS_HI16`/`R_MIPS_LO16` form a 32 bit
        value split across a `lui`/`lw`-`sw`-`addiu` pair.  The `lui` is *not*
        necessarily adjacent to its `%lo` (gcc happily interleaves several of
        them), so the pairs are matched up by register, exactly like a real
        MIPS linker does.
        """
        raw: list[Reloc] = []
        for sec in self.sections:
            if not sec.is_rel:
                continue
            target_name = sec.name[4:] if sec.name.startswith(".rel") else ""
            entries = []
            for off in range(0, len(sec.data) - 7, 8):
                r_offset, r_info = struct.unpack_from("<II", sec.data, off)
                entries.append((r_offset, r_info & 0xFF, r_info >> 8))
            # PSPLINK is inconsistent: `.rel.text`/`rel.rodata`/`rel.data` use
            # absolute addresses, while `.rel.cplinit` and friends count from
            # the start of the section they patch.  Detect which by looking at
            # the values themselves.
            target = self.section(target_name)
            base = 0
            if target is not None and target.size and entries:
                if min(o for o, _, _ in entries) < target.addr:
                    base = target.addr
            for r_offset, r_type, r_sym in entries:
                raw.append(Reloc(target_name, r_offset + base, r_type, r_sym,
                                 0, 0))

        pair_hilo(self, raw)
        for r in raw:
            if r.resolved:
                continue              # already resolved by the CSE pass
            if r.type in (R_MIPS_HI16, R_MIPS_LO16) \
                    and r.offset not in self.hilo_pairs:
                r.resolved = False
                continue
            r.target = self._resolve(r.offset, r.type)
            r.resolved = True
        return raw

    def _resolve(self, offset: int, r_type: int) -> int:
        """Recover the referenced address of one relocation."""
        if r_type in (R_MIPS_HI16, R_MIPS_LO16):
            pair = self.hilo_pairs.get(offset)
            if pair is None:
                return offset
            hi_off, lo_off = pair
            hi = self._word(hi_off) or 0
            lo = self._word(lo_off) or 0
            # The linker's `%hi` is the address rounded up to the next 64 KiB
            # boundary and `%lo` is the (negative) difference from it, so
            # recombining the two means sign extending the low half.
            low = lo & 0xFFFF
            if low & 0x8000:
                low -= 0x10000
            return (((hi & 0xFFFF) << 16) + low) & 0xFFFFFFFF
        word = self._word(offset)
        if word is None:
            return 0
        if r_type == R_MIPS_32:
            return word
        if r_type == R_MIPS_26:
            # `j`/`jal` are `opcode[31:26] | target[27:2]`; the top four bits
            # of the address come from the region the instruction sits in,
            # which for a module linked at base 0 is always zero.
            return (offset & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
        return 0

    def _word(self, addr: int) -> int | None:
        data = self.read(addr, 4)
        return struct.unpack("<I", data)[0] if len(data) == 4 else None


def pair_hilo(elf: "Elf", relocs: list[Reloc]) -> None:
    """Match every `%hi` relocation with the `%lo` that consumes it.

    Three things make adjacency the wrong rule:

    * gcc emits several `lui` instructions back to back, so the `%lo` is often
      several instructions away from its `%hi`;
    * after common subexpression elimination one `lui` feeds *several* `%lo`
      uses of the same register;
    * most confusingly, gcc sinks a `%hi` into the *delay slot* of a branch, so
      several `lui $a1, %hi(sym)` can appear before the single `addiu $a1, $a1,
      %lo(sym)` that completes them - and the ones in delay slots are not even
      executed on the path taken.

    So the pairing is done backwards: every `%lo` is matched to the closest
    preceding `%hi` for the register it is added to, and then each `%hi` is
    matched to the *first* `%lo` after it for the same register.  A `%hi` that
    is still unmatched at that point is one whose low half is never materialised
    in this object, which is genuinely unresolvable.
    """
    by_offset: dict[int, Reloc] = {}
    for r in relocs:
        by_offset.setdefault(r.offset, r)

    ordered = sorted(relocs, key=lambda x: x.offset)

    # Pass 1: for each register, remember the runs of %hi and resolve the %los.
    # Only %hi/%lo are handled here; R_MIPS_26/32 are resolved by the caller.
    hi_runs: dict[int, list[int]] = {}
    for r in ordered:
        if r.type not in (R_MIPS_HI16, R_MIPS_LO16):
            continue
        r.target = 0
        r.resolved = True
    for r in ordered:
        if r.type == R_MIPS_HI16:
            word = elf._word(r.offset)
            if word is not None and word >> 26 == 0x0F:   # lui $rt, %hi(sym)
                hi_runs.setdefault((word >> 16) & 0x1F, []).append(r.offset)
        elif r.type == R_MIPS_LO16:
            word = elf._word(r.offset)
            reg = lo_base_register(word) if word is not None else None
            run = hi_runs.get(reg) if reg is not None else None
            if not run:
                continue
            hi_off = run[-1]
            elf.hilo_pairs[r.offset] = (hi_off, r.offset)
            elf.hilo_pairs.setdefault(hi_off, (hi_off, r.offset))
            r_hi = by_offset.get(hi_off)
            if r_hi is not None:
                # both halves name the same symbol
                hi_word = elf._word(hi_off) or 0
                lo_word = elf._word(r.offset) or 0
                low = lo_word & 0xFFFF
                if low & 0x8000:
                    low -= 0x10000
                target = (((hi_word & 0xFFFF) << 16) + low) & 0xFFFFFFFF
                r.target = target
                r_hi.target = target
                r.resolved = True
                r_hi.resolved = True

    # Pass 2: a `%hi` that no `%lo` claimed is usually a *copy* of another one.
    # After common subexpression elimination gcc keeps several `lui` writing the
    # same constant, and only one of them ends up paired with the `%lo`; the
    # rest are dead as far as the relocation table is concerned but their value
    # is identical.  Matching them on their high half recovers it.
    claimed = {hi for hi, _ in elf.hilo_pairs.values()}
    unpaired = [r for r in ordered
                if r.type == R_MIPS_HI16 and r.offset not in claimed]
    if unpaired:
        # high half -> the pair a sibling %hi resolved to
        resolved: dict[int, int] = {}
        for r in ordered:
            if r.type == R_MIPS_HI16 and r.offset in claimed:
                word = elf._word(r.offset)
                if word is not None:
                    resolved.setdefault(word & 0xFFFF, r.target)
        for r in unpaired:
            word = elf._word(r.offset)
            if word is None:
                continue
            target = resolved.get(word & 0xFFFF)
            if target is not None:
                elf.hilo_pairs[r.offset] = (r.offset, r.offset)
                r.target = target
                r.resolved = True
                resolved.setdefault(word & 0xFFFF, target)

    # Pass 3: mark anything still unresolved.
    for r in ordered:
        if r.type in (R_MIPS_HI16, R_MIPS_LO16) \
                and r.offset not in elf.hilo_pairs:
            r.resolved = False


def load(path: str) -> Elf:
    with open(path, "rb") as f:
        raw = f.read()

    from io import BytesIO
    elf_file = ELFFile(BytesIO(raw))

    elf = Elf(path, dict(elf_file.header), len(raw))
    for seg in elf_file.iter_segments():
        h = seg.header
        elf.segments.append({
            "type": h["p_type"], "offset": h["p_offset"], "vaddr": h["p_vaddr"],
            "filesz": h["p_filesz"], "memsz": h["p_memsz"],
            "flags": h["p_flags"], "align": h["p_align"],
        })
    for i, sec in enumerate(elf_file.iter_sections()):
        h = sec.header
        stype = h["sh_type"]
        if isinstance(stype, str):
            stype = {
                "SHT_NULL": SHT_NULL, "SHT_PROGBITS": SHT_PROGBITS,
                "SHT_SYMTAB": SHT_SYMTAB, "SHT_STRTAB": SHT_STRTAB,
                "SHT_RELA": SHT_RELA, "SHT_NOBITS": SHT_NOBITS, "SHT_REL": SHT_REL,
            }.get(stype, 0)
        data = b"" if stype == SHT_NOBITS else raw[h["sh_offset"]:
                                                 h["sh_offset"] + h["sh_size"]]
        elf.sections.append(Section(
            index=i, name=sec.name, type=stype, flags=h["sh_flags"],
            addr=h["sh_addr"], offset=h["sh_offset"], size=h["sh_size"],
            link=h["sh_link"], info=h["sh_info"], addralign=h["sh_addralign"],
            entsize=h["sh_entsize"], data=data))
    return elf


if __name__ == "__main__":
    import sys
    elf = load(sys.argv[1])
    print(f"entry {elf.header['e_entry']:#x}  flags {elf.header['e_flags']:#x}")
    for sec in elf.sections:
        print(f"{sec.index:3} {sec.name:<32} {sec.addr:#010x} {sec.offset:#010x}"
              f" {sec.size:#010x} flags={sec.flags}")

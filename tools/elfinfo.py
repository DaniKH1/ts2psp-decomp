#!/usr/bin/env python3
"""Dump the ELF layout of a PSP executable (decrypted EBOOT.BIN).

Usage: python tools/elfinfo.py <elf> [--symbols out.txt]
"""

from __future__ import annotations

import argparse
import sys

from elftools.elf.elffile import ELFFile

SHT = {
    0: "NULL", 1: "PROGBITS", 2: "SYMTAB", 3: "STRTAB", 4: "RELA",
    5: "HASH", 6: "DYNAMIC", 7: "NOTE", 8: "NOBITS", 9: "REL",
    0x70000006: "MIPS_REGINFO", 0x7000000A: "MIPS_OPTIONS",
}
SHF = [(0x2, "ALLOC"), (0x4, "EXECINSTR"), (0x10, "MERGE"),
       (0x20, "STRINGS"), (0x40, "INFO_LINK"), (0x80, "LINK_ORDER"),
       (0x100, "OS_NONCONFORMING"), (0x200, "GROUP"), (0x400, "TLS")]
PT = {0: "NULL", 1: "LOAD", 2: "DYNAMIC", 3: "INTERP", 4: "NOTE",
      6: "PHDR", 0x70000000: "MIPS_REGINFO", 0x70000003: "MIPS_ABIFLAGS"}


def flags_str(value: int) -> str:
    return " ".join(n for b, n in SHF if value & b)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("elf")
    ap.add_argument("--symbols", help="write the symbol table here")
    ap.add_argument("--filter", help="only list symbols matching this substring")
    ns = ap.parse_args()

    with open(ns.elf, "rb") as f:
        elf = ELFFile(f)
        print(f"class      : {elf.elfclass}-bit")
        print(f"endian     : {elf.little_endian and 'little' or 'big'}")
        print(f"machine    : {elf.header['e_machine']}")
        print(f"type       : {elf.header['e_type']}")
        print(f"entry      : {elf.header['e_entry']:#010x}")
        print(f"flags      : {elf.header['e_flags']:#010x}")
        print(f"phnum      : {elf.num_segments()}   shnum: {elf.num_sections()}")
        print()
        print("== program headers ==")
        for i, seg in enumerate(elf.iter_segments()):
            h = seg.header
            ptype = h["p_type"]
            if not isinstance(ptype, str):
                ptype = PT.get(ptype, hex(ptype))
            print(f"{i:2} {ptype:<14}"
                  f" off={h['p_offset']:#010x} vaddr={h['p_vaddr']:#010x}"
                  f" filesz={h['p_filesz']:#010x} memsz={h['p_memsz']:#010x}"
                  f" flags={h['p_flags']} align={h['p_align']:#x}")
        print()
        print("== sections ==")
        print(f"{'idx':>3} {'name':<24} {'type':<12} {'addr':>10} {'off':>10}"
              f" {'size':>10} {'flags'}")
        for i, sec in enumerate(elf.iter_sections()):
            h = sec.header
            stype = h["sh_type"]
            if not isinstance(stype, str):
                stype = SHT.get(stype, hex(stype))
            print(f"{i:3} {sec.name:<24} {stype:<12}"
                  f" {h['sh_addr']:#010x} {h['sh_offset']:#010x}"
                  f" {h['sh_size']:#010x} {flags_str(h['sh_flags'])}")

        symtabs = [s for s in elf.iter_sections()
                   if s.header["sh_type"] == 2 and s.name in (".symtab", ".dynsym")]
        for st in symtabs:
            print()
            print(f"== symbols ({st.name}, {st.num_symbols()} entries) ==")
            rows = []
            for sym in st.iter_symbols():
                info = sym["st_info"]
                bind = info["bind"]
                typ = info["type"]
                rows.append((sym["st_value"], sym["st_size"], bind, typ,
                             sym.name))
            rows.sort(key=lambda r: (r[0], r[5]))
            for value, size, bind, typ, name in rows:
                if not name:
                    continue
                if ns.filter and ns.filter not in name:
                    continue
                print(f"{value:#010x} {size:>8} {bind:<7} {typ:<8} {name}")
            if ns.symbols:
                with open(ns.symbols, "w", encoding="utf-8") as g:
                    for value, size, bind, typ, name in rows:
                        if name:
                            g.write(f"{name} = 0x{value:08X}; // size {size}\n")
                print(f"wrote {ns.symbols}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

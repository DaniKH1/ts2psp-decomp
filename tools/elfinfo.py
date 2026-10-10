"""Report the structure of the extracted module image.

Prints the ELF facts this decompilation is built on: the target, the
segments the loader maps, every section with its address and size, and
what survived of the original symbol table.
"""

import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

BOOT = Path(__file__).resolve().parent.parent / "disks" / "pgs-si2" / "BOOT.BIN"

MACHINES = {8: "MIPS R3000"}


def main() -> int:
    with open(BOOT, "rb") as f:
        elf = ELFFile(f)
        header = elf.header
        print(f"e_machine  {MACHINES.get(header['e_machine'], header['e_machine'])}")
        print(f"e_entry    0x{header['e_entry']:08X}")
        print(f"e_type     {header['e_type']}")

        print("\nprogram headers (what the loader maps):")
        for seg in elf.iter_segments():
            h = seg.header
            print(
                f"  {h['p_type']:<10} vaddr 0x{h['p_vaddr']:08X}"
                f" filesz 0x{h['p_filesz']:08X} memsz 0x{h['p_memsz']:08X}"
                f" flags {h['p_flags']}"
            )

        print("\nsections:")
        total_code = 0
        for sec in elf.iter_sections():
            name = sec.name
            size = sec["sh_size"]
            addr = sec["sh_addr"]
            print(f"  {name:<20} addr 0x{addr:08X} size 0x{size:08X} ({size} bytes)")
            if name == ".text":
                total_code = size

        print(f"\n.text is {total_code} bytes = {total_code // 4} instructions")

        for sec in elf.iter_sections():
            if sec.name in (".symtab", ".dynsym"):
                count = sec.num_symbols()
                named = sum(1 for s in sec.iter_symbols() if s.name and not s.name.startswith("$"))
                funcs = sum(
                    1
                    for s in sec.iter_symbols()
                    if s["st_info"]["type"] == "STT_FUNC" and s["st_size"] > 0
                )
                print(f"\n{sec.name}: {count} symbols, {named} carry a name, {funcs} sized functions")
                sample = [
                    (s.name, s["st_value"], s["st_size"])
                    for s in sec.iter_symbols()
                    if s["st_info"]["type"] == "STT_FUNC" and s["st_size"] > 0 and s.name
                ][:15]
                for name, value, size in sample:
                    print(f"  {name:<40} 0x{value:08X} +0x{size:X}")

    return 0


if __name__ == "__main__":
    sys.exit(main())

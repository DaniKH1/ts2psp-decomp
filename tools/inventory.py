"""Enumerate every function in the module's code sections.

Function starts come from several independent sources, and each entry in
the output records which one found it:

* `section` - the start of a code section.  The original build used
  `-ffunction-sections`, so every translation unit became its own section
  and every section starts with a function.
* `call` - the target of a `jal` (jump-and-link) anywhere in the code.
  Addresses are absolute within the module (it links at 0), so the
  instruction word alone carries the target; no symbol table is needed.
* `jump` - the target of a bare `j`.  Either a tail call or a goto; a `j`
  to a label that is also the target of a conditional branch is a goto and
  is not a function start.
* `ctor` - an entry of the static constructor list in `.cplinit`.  Nothing
  in the code calls these; the runtime walks the list.
* `switch` - a relocated data word pointing into code, inside a function
  body that several such words point into: a jump table.  These are
  recorded for the record but are not function starts.
* `data` - any other relocated data word pointing into code: the
  constructor-ish / vtable case.  Ambiguous by construction and worth a
  per-function look during decompilation.

Function sizes are "distance to the next start within the same section" -
the same approximation splat uses, and the same caveat: a data island
inside a code section is attributed to the function before it.
"""

import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

BOOT = Path(__file__).resolve().parent.parent / "disks" / "pgs-si2" / "BOOT.BIN"
OUT = Path(__file__).resolve().parent.parent / "config" / "functions.txt"

SHF_EXECINSTR = 0x4
SHT_REL = 9
SHT_RELA = 4
SHT_MIPS_REL = 0x70000009
SHT_MIPS_RELA = 0x700000A0
# The PSP toolchain tags its relocation sections SHT_MIPS_RELA but emits
# 8-byte REL-style records (offset, info) with the addend in the target
# section, so both spellings are parsed the same way.
REL_TYPES = (SHT_REL, SHT_RELA, SHT_MIPS_REL, SHT_MIPS_RELA)
R_MIPS_32 = 2

# Import stubs are loader-patched placeholders, not functions of the game.
SKIP_PREFIX = ".sceStub"

J = 2
JAL = 3
BRANCH_OPS = {4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17}  # beq bne blez bgtz + likely
REGIMM_BRANCH_RTS = {0, 1, 2, 3, 16, 17, 18, 19}   # bltz bgez + likely + link

RANK = {"section": 6, "call": 5, "jump": 4, "ctor": 3, "data": 2, "switch": 1}


def section_prefix(name: str) -> str:
    """`.text.sortAndCullScene` -> `sortAndCullScene`, `.text` -> `func`."""
    if name == ".text":
        return "func"
    return name[len(".text."):] if name.startswith(".text.") else name


def main() -> int:
    elf = ELFFile(open(BOOT, "rb"))

    code = []
    for sec in elf.iter_sections():
        if sec["sh_flags"] & SHF_EXECINSTR and not sec.name.startswith(SKIP_PREFIX):
            addr, size = sec["sh_addr"], sec["sh_size"]
            code.append((sec.name, addr, addr + size, sec.data()))

    def region_of(addr: int):
        for name, start, end, _ in code:
            if start <= addr < end:
                return name
        return None

    starts = {}  # addr -> tier
    branch_targets = set()
    data_targets = []  # (addr, source-section-name)

    def add(addr: int, tier: str) -> None:
        if region_of(addr) is None:
            return
        if RANK[tier] > RANK.get(starts.get(addr, ""), 0):
            starts[addr] = tier

    for name, start, end, _ in code:
        add(start, "section")

    for name, start, end, data in code:
        for off in range(0, len(data) - 3, 4):
            word = int.from_bytes(data[off:off + 4], "little")
            addr = start + off
            op = word >> 26
            if op == JAL:
                add((word & 0x03FFFFFF) << 2, "call")
            elif op == J:
                add((word & 0x03FFFFFF) << 2, "jump")
            elif op == 1 and ((word >> 16) & 0x1F) in REGIMM_BRANCH_RTS:
                branch_targets.add(addr + 4 + (((word & 0xFFFF) ^ 0x8000) - 0x8000) * 4)
            elif op in BRANCH_OPS:
                branch_targets.add(addr + 4 + (((word & 0xFFFF) ^ 0x8000) - 0x8000) * 4)

    # a `j` to a conditional branch's label is a goto, not a tail call
    for addr, tier in list(starts.items()):
        if tier == "jump" and addr in branch_targets:
            del starts[addr]

    # relocated data words pointing into code
    data_relocs = 0
    for sec in elf.iter_sections():
        if sec["sh_type"] not in REL_TYPES:
            continue
        target = elf.get_section(sec["sh_info"])
        if target["sh_flags"] & SHF_EXECINSTR:
            continue  # code relocations are handled by decoding the code
        rdata = sec.data()
        tdata = target.data()
        entsize = sec["sh_entsize"] or 8
        for off in range(0, len(rdata) - entsize + 1, entsize):
            r_offset = int.from_bytes(rdata[off:off + 4], "little")
            r_info = int.from_bytes(rdata[off + 4:off + 8], "little")
            if r_info & 0xFF != R_MIPS_32:
                continue
            data_relocs += 1
            # r_offset is absolute in some tables and section-relative in
            # others (.cplinit uses the relative form); both appear here.
            at = r_offset if r_offset < target["sh_addr"] else r_offset - target["sh_addr"]
            value = int.from_bytes(tdata[at:at + 4], "little")
            if region_of(value) is None:
                continue
            if target.name == ".cplinit":
                tier = "ctor"
            else:
                tier = "data"
            add(value, tier)
            data_targets.append((value, target.name))

    # cluster analysis: several data words into one body => jump table
    call_spans = []
    for name, start, end, _ in code:
        here = sorted(a for a in starts if start <= a < end and starts[a] in ("section", "call", "jump", "ctor"))
        for i, addr in enumerate(here):
            nxt = here[i + 1] if i + 1 < len(here) else end
            call_spans.append((addr, nxt))
    per_span = {}
    for value, src in data_targets:
        if starts.get(value) != "data":
            continue
        for lo, hi in call_spans:
            if lo < value < hi:
                per_span[(lo, hi)] = per_span.get((lo, hi), 0) + 1
                break
    for addr, tier in list(starts.items()):
        if tier != "data":
            continue
        for (lo, hi), n in per_span.items():
            if lo < addr < hi and n >= 2:
                starts[addr] = "switch"
                break

    # per-section inventory with sizes
    lines = []
    summary = []
    for name, start, end, _ in code:
        here = sorted(a for a in starts if start <= a < end and starts[a] != "switch")
        tier_count = {}
        for i, addr in enumerate(here):
            size = (here[i + 1] if i + 1 < len(here) else end) - addr
            tier = starts[addr]
            tier_count[tier] = tier_count.get(tier, 0) + 1
            prefix = section_prefix(name)
            lines.append((addr, size, name, tier, f"{prefix}_{addr:08X}"))
        summary.append((name, len(here), tier_count))

    OUT.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT, "w", newline="\n") as f:
        f.write("# function inventory: address, size, section, tier, name\n")
        f.write("# tier: section = section start, call = jal target, jump = j target (tail call)\n")
        f.write("#       ctor = .cplinit entry, data = relocated word pointing into code\n")
        for addr, size, name, tier, sym in lines:
            f.write(f"0x{addr:08X} 0x{size:08X} {name:<26} {tier:<8} {sym}\n")

    total = len(lines)
    switches = sum(1 for t in starts.values() if t == "switch")
    print(f"{total} functions -> {OUT}")
    for name, count, tiers in summary:
        parts = ", ".join(f"{t} {n}" for t, n in sorted(tiers.items()))
        print(f"  {name:<26} {count:>6}  ({parts})")
    print(f"jump-table labels recorded but not counted: {switches}")
    print(f"relocated data words scanned: {data_relocs}, "
          f"conditional branch targets: {len(branch_targets)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

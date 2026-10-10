"""Generate config/pgs-si2.symbols.ld - absolute addresses for the verify link.

The retail symbol table is empty, so tools/verify_c.py links each candidate
against this file: every symbol the original code can reference is pinned to
the address it has in BOOT.BIN, which makes a candidate compile resolve to the
same instruction words as the retail build.

    python tools/linkerscript.py            # regenerate and report

Contents, in order:
  * section anchors      _sec_text_RENDER = 0x...;  _sec_text_RENDER_SIZE
  * import stubs         sceKernelCreateThread = 0x...;  (named 223/223 by
                        tools/imports.py: pspsdk stub records + SHA-1(NID)
                        + config/nid-extra.txt)
  * library anchors      _stub_sceAudio = 0x...;     (kept: the original code
                        also reaches stubs by address alone)
  * every inventory name func_000000B0 = 0x...;      (config/functions.txt)
  * data references      dword_001D1B00 = 0x...;     recovered by scanning the
                        original code for lui/addiu and load/store pairs that
                        resolve into an allocated section
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspelf  # noqa: E402

OUT = ROOT / "config" / "pgs-si2.symbols.ld"
FUNCTIONS = ROOT / "config" / "functions.txt"
IMPORTS = ROOT / "config" / "imports.txt"

# MIPS32 opcodes we care about while scanning code for address material.
OP_LUI = 0x0F
OP_ADDIU = 0x09
OP_ORI = 0x0D
OP_LOADSTORE = set(range(0x20, 0x2C)) | {0x2F} | set(range(0x30, 0x34)) | set(range(0x38, 0x3C))


def imm_signed(w: int) -> int:
    v = w & 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def imm_unsigned(w: int) -> int:
    return w & 0xFFFF


def rs(w: int) -> int:
    return (w >> 21) & 0x1F


def rt(w: int) -> int:
    return (w >> 16) & 0x1F


def load_inventory() -> list[tuple[int, int, str, str, str]]:
    """(addr, size, section, tier, name) for every inventory entry."""
    entries = []
    for line in FUNCTIONS.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        addr, size, section, tier, name = line.split()
        entries.append((int(addr, 16), int(size, 16), section, tier, name))
    return entries


def scan_addresses(code: bytes, base: int, mod: pspelf.Module,
                   named: set[int]) -> dict[int, list[int]]:
    """Address material inside one function.

    Tracks which registers hold a known upper half (lui) or full value
    (lui+addiu/ori) and harvests every value that lands in an allocated
    section - globals, string literals, jump tables, function pointers.
    Returns {address: [instruction offsets, ...]} relative to `base`.
    """
    hi: dict[int, int] = {}
    full: dict[int, int] = {}
    hits: dict[int, list[int]] = {}

    def harvest(value: int, off: int) -> None:
        if value in named:
            return  # an exact function start: already an inventory symbol
        section = mod.containing_section(value)
        if section is None:
            return
        if section == ".text" or section.startswith(".text."):
            # A code-region value is either an arithmetic constant or a
            # genuine mid-code anchor.  Constants are built from lui 0
            # (lui 0 + addiu 4 = 4) and can never reach 0x10000+, while a
            # tracked value in .text at >= 0x10000 required a nonzero
            # upper half - retail kept such a base separate from its
            # field offset when a link-time relocation targeted a symbol
            # at exactly that address (func_00049A90: base 0x743A0 is the
            # `jr ra` of an epilogue, accessed as *(u32*)(0x743A0 + 4)).
            # Exact function starts were already filtered above.
            if value < 0x10000:
                return
            hits.setdefault(value, []).append(off)
            return
        hits.setdefault(value, []).append(off)

    for off in range(0, len(code) - 3, 4):
        w = int.from_bytes(code[off:off + 4], "little")
        opcode = w >> 26

        if opcode == OP_LUI:
            hi[rt(w)] = imm_unsigned(w) << 16
            full.pop(rt(w), None)
            continue

        if opcode in (OP_ADDIU, OP_ORI):
            src, dst = rs(w), rt(w)
            base_val = full.get(src, hi.get(src))
            if base_val is not None:
                if opcode == OP_ADDIU:
                    value = base_val + imm_signed(w)
                else:
                    value = base_val | imm_unsigned(w)   # ORI is bitwise
                value &= 0xFFFFFFFF
                full[dst] = value
                harvest(value, off)
            else:
                full.pop(dst, None)
            hi.pop(dst, None)
            continue

        if opcode in OP_LOADSTORE:
            value = full.get(rs(w), hi.get(rs(w)))
            if value is not None:
                target = (value + imm_signed(w)) & 0xFFFFFFFF
                harvest(target, off)
            if opcode <= 0x2B:
                # integer loads overwrite rt with the loaded value
                hi.pop(rt(w), None)
                full.pop(rt(w), None)
            continue

        # The other I-types that write rt with a value we cannot follow.
        if opcode in (0x08, 0x0A, 0x0B, 0x0C, 0x0E):  # addi slti sltiu andi xori
            hi.pop(rt(w), None)
            full.pop(rt(w), None)
            continue

        if opcode == 0x00:  # R-type: writes rd
            funct = w & 0x1F
            rd = (w >> 11) & 0x1F
            if rd == 0:
                continue
            hi.pop(rd, None)
            full.pop(rd, None)
            # `addu/or rd, rs, $zero` and `sll rd, rt, 0` are moves: keep
            # tracking the value through them, pointers get moved a lot.
            src = None
            if funct in (0x21, 0x25):          # addu, or
                if rs(w) == 0:
                    src = rt(w)
                elif rt(w) == 0:
                    src = rs(w)
            elif funct == 0x00 and (w >> 6) & 0x1F == 0 and rt(w) != 0:
                src = rt(w)                    # sll rd, rt, 0
            if src:
                if src in full:
                    full[rd] = full[src]
                elif src in hi:
                    hi[rd] = hi[src]
            continue

        if opcode == 0x1F:  # SPECIAL3: EXT writes rt, BSHFL writes rd
            for reg in (rt(w), (w >> 11) & 0x1F):
                if reg != 0:
                    hi.pop(reg, None)
                    full.pop(reg, None)

    return {addr: [base + o for o in offs] for addr, offs in hits.items()}


def mangle(name: str) -> str:
    return re.sub(r"[^0-9A-Za-z_]", "_", name)


def main() -> int:
    mod = pspelf.Module()
    entries = load_inventory()

    # --- inventory symbols -------------------------------------------------
    named: set[int] = set()
    func_lines: list[str] = []
    seen_names: set[str] = set()
    for addr, size, section, tier, name in entries:
        if name in seen_names:
            print(f"error: duplicate symbol name {name}", file=sys.stderr)
            return 1
        seen_names.add(name)
        named.add(addr)
        func_lines.append((addr, f"{name} = 0x{addr:08X};"))

    # --- section anchors ---------------------------------------------------
    anchor_lines: list[str] = []
    stub_lines: list[str] = []
    for name, (addr, size, _flags) in sorted(mod.sections.items(), key=lambda kv: kv[1][0]):
        m = mangle(name).lstrip("_")
        anchor_lines.append((addr, f"_sec_{m}_START = 0x{addr:08X};"))
        anchor_lines.append((addr, f"_sec_{m}_SIZE  = 0x{size:08X};"))
        if name.startswith(".sceStub.text."):
            stub_lines.append((addr, f"_stub_{name[len('.sceStub.text.'):]}"
                                     f" = 0x{addr:08X};"))

    # --- named import stubs (tools/imports.py -> config/imports.txt) -------
    import_lines: list[tuple[int, str]] = []
    if IMPORTS.is_file():
        for raw in IMPORTS.read_text(encoding="utf-8").splitlines():
            raw = raw.strip()
            if not raw or raw.startswith("#"):
                continue
            parts = raw.split()
            if len(parts) != 4:
                print(f"error: malformed line in {IMPORTS.name}: {raw}",
                      file=sys.stderr)
                return 1
            addr_s, _lib, _nid, name = parts
            if name == "?":
                continue
            if name in seen_names:
                print(f"error: duplicate symbol name {name}", file=sys.stderr)
                return 1
            seen_names.add(name)
            addr = int(addr_s, 16)
            import_lines.append((addr, f"{name} = 0x{addr:08X};"))

    # --- data references recovered from the original code ------------------
    data_refs: dict[int, list[str]] = {}
    for addr, size, section, tier, name in entries:
        if size == 0:
            continue
        code = mod.read(addr, size)
        for target, offs in scan_addresses(code, addr, mod, named).items():
            refs = data_refs.setdefault(target, [])
            if name not in refs:
                refs.append(name)

    data_lines = []
    for target in sorted(data_refs):
        section = mod.containing_section(target)
        if section == ".text" or (section and section.startswith(".text.")):
            prefix = "code"       # mid-code anchor (relocation target)
        elif section and section.startswith(".sceStub.text."):
            prefix = "stub"      # an import reached by address, not by name
        elif target % 4 == 0:
            prefix = "dword"
        else:
            prefix = "byte"
        ref = ", ".join(data_refs[target][:3])
        more = "" if len(data_refs[target]) <= 3 else f" (+{len(data_refs[target]) - 3} more)"
        data_lines.append((target, f"{prefix}_{target:08X} = 0x{target:08X};"
                                   f"  /* ref: {ref}{more} */"))

    # --- write -------------------------------------------------------------
    header = f"""/* ==========================================================================
 *  The Sims 2 PSP - absolute linker symbols for the module image.
 *
 *  GENERATED BY tools/linkerscript.py - do not edit.
 *
 *  The retail ELF has an empty .symtab, so this file re-creates the address
 *  of every symbol a candidate C file can reference.  tools/verify_c.py links
 *  each candidate against it; with the same addresses, a correct C translation
 *  assembles to the very instruction words the retail build shipped.
 *
 *  {len(func_lines)} inventory symbols, {len(anchor_lines)} section anchors,
 *  {len(import_lines)} named import stubs, {len(stub_lines)} import-library
 *  anchors, {len(data_lines)} recovered data addresses (scanned from
 *  lui/addiu and load/store pairs in the original code - see "ref:" comments
 *  for which functions reach them).
 * ========================================================================== */


/* --- section anchors --- */
"""
    body = [header]
    for _, line in sorted(anchor_lines, key=lambda t: t[0]):
        body.append(line + "\n")
    body.append("\n/* --- import stub libraries (granularity: library, not function) --- */\n")
    for _, line in sorted(stub_lines, key=lambda t: t[0]):
        body.append(line + "\n")
    body.append("\n/* --- named import stubs (config/imports.txt) --- */\n")
    for _, line in sorted(import_lines, key=lambda t: t[0]):
        body.append(line + "\n")
    body.append("\n/* --- functions (config/functions.txt) --- */\n")
    for _, line in sorted(func_lines, key=lambda t: t[0]):
        body.append(line + "\n")
    body.append("\n/* --- data referenced by the code above --- */\n")
    for _, line in sorted(data_lines, key=lambda t: t[0]):
        body.append(line + "\n")

    OUT.write_text("".join(body), encoding="utf-8")
    mod.close()
    print(f"wrote {OUT.relative_to(ROOT)}: {len(func_lines)} functions, "
          f"{len(anchor_lines)} anchors, {len(import_lines)} imports, "
          f"{len(stub_lines)} stub libs, {len(data_lines)} data addresses")
    return 0


if __name__ == "__main__":
    sys.exit(main())

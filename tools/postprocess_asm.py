"""Post-process the asm splat produced so it assembles and links as one module.

Two rewrites are needed, both of them consequences of splat putting every
function in its own `.s` file while the original had them all in one `.text`:

* **cross-file labels become global.**  gas treats `.L`-prefixed symbols as
  file-local, so a branch from one function into another cannot resolve.

* **unpaired `%hi` becomes a plain immediate.**  `ld` pairs `R_MIPS_HI16` with
  `R_MIPS_LO16` inside a single object file.  When gcc sank a `%hi` into a delay
  slot and the matching `%lo` ended up in a different function, the pair is
  split across two objects and the link fails.  Since the target address is
  known, the `%hi` can simply be written as the constant it stands for.

Both rewrites are verified afterwards by comparing the assembled section bytes
against the original (see tools/build.py).

Run after splat and gen_linker_symbols.py:
    python tools/postprocess_asm.py
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

LABEL_DEF = re.compile(r"^(\s*)(?!glabel\b|alabel\b)(\.?[\w.$]+):",
                       re.MULTILINE)
HI_USE = re.compile(r"%hi\(([\w.$]+)\)")
LO_USE = re.compile(r"%lo\(([\w.$]+)\)")
ANY_USE = re.compile(r"(?:%hi|%lo)\(([\w.$]+)\)")
# Any branch or jump to a named label.
BRANCH_USE = re.compile(
    r"\b(?:jal|jalr|j|jralr|b|bal|beqz|bnez|beq|bne|blez|bgtz|bgez|bltz|"
    r"beql|bnel|blezl|bgtzl|bgezal|bltzal)\s+(\.?[\w.$]+)")


def load_symbols(path: Path) -> dict[str, int]:
    """`name = 0xVALUE;` assignments from a linker symbol file."""
    out: dict[str, int] = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("/*")[0].strip()
        if "=" not in line:
            continue
        name, _, value = line.partition("=")
        name = name.strip()
        value = value.strip().rstrip(";").strip()
        try:
            out[name] = int(value, 0)
        except ValueError:
            continue
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--asm", default=str(ROOT / "asm/eboot"))
    ap.add_argument("--symbols", default=str(ROOT / "config/eboot.symbols.ld"))
    ns = ap.parse_args()

    symbols = load_symbols(Path(ns.symbols))
    # Function labels are defined by their own object file, but their *values*
    # are only known from the splat symbol map - needed here to turn an unpaired
    # `%hi` into the constant it stands for.
    sym_addrs = Path(ns.asm).parent.parent / "config" / "eboot.symbol_addrs.txt"
    if sym_addrs.exists():
        for line in sym_addrs.read_text(encoding="utf-8").splitlines():
            line = line.split("//")[0].strip()
            if "=" not in line:
                continue
            name, _, value = line.partition("=")
            name = name.strip()
            value = value.strip().rstrip(";").strip()
            try:
                symbols.setdefault(name, int(value, 0))
            except ValueError:
                continue
    files = sorted(Path(ns.asm).rglob("*.s"))

    # First pass: where is every bare label defined, and which files use it?
    defined_at: dict[str, set[Path]] = {}
    texts: dict[Path, str] = {}
    for path in files:
        text = path.read_text(encoding="utf-8")
        texts[path] = text
        for _, name in LABEL_DEF.findall(text):
            defined_at.setdefault(name.strip(), set()).add(path)

    promoted = 0
    used_at: dict[str, set[Path]] = {}
    for path, text in texts.items():
        for name in BRANCH_USE.findall(text) + ANY_USE.findall(text):
            used_at.setdefault(name, set()).add(path)

    for path, text in texts.items():
        for _, name in LABEL_DEF.findall(text):
            name = name.strip()
            owners = defined_at.get(name, set())
            users = used_at.get(name, set())
            # defined here and referenced from somewhere else -> must be global
            if len(owners) == 1 and path in owners and users - owners:
                text = re.sub(rf"^(\s*){re.escape(name)}:",
                              rf"\1.globl {name}\n\1{name}:", text,
                              flags=re.MULTILINE)
                texts[path] = text
                promoted += 1

    # Second pass: `%hi(X)` with no `%lo(X)` in the same file.
    rewritten = 0
    for path, text in texts.items():
        los = set(LO_USE.findall(text))
        if not los and not HI_USE.search(text):
            continue

        def replace(match: re.Match) -> str:
            nonlocal rewritten
            name = match.group(1)
            if name in los or name not in symbols:
                return match.group(0)
            rewritten += 1
            # gas's `%hi` rounds the address up to the next 64 KiB boundary,
            # which is what the original `lui` encoded too.
            return f"0x{((symbols[name] + 0x8000) >> 16) & 0xFFFF:04X}"

        new = HI_USE.sub(replace, text)
        if new != text:
            texts[path] = new

    for path, text in texts.items():
        path.write_text(text, encoding="utf-8")
    print(f"promoted {promoted} cross-file labels, "
          f"rewrote {rewritten} unpaired %hi references")
    return 0


if __name__ == "__main__":
    sys.exit(main())

"""Report labels the generated asm references but never defines.

splat puts every function in its own `.s` file, which breaks as soon as a
function branches to a local label that lives in a *different* file: gas treats
`.L`-prefixed symbols as file-local, so the reference cannot be resolved.  This
tool lists every such symbol so the build can turn them global.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Symbols defined by the asm: labels, glabels and assembler directives that
# introduce a symbol.
DEFINES = re.compile(
    r"^\s*(?:glabel|alabel|\.globl|\.global)\s+(\S+)|^\s*(\.?[\w.$]+):",
    re.MULTILINE)
# Bare labels (not glabels) - these are what can go cross-file.
LABEL_DEF = re.compile(r"^\s*(?!glabel\b|alabel\b)(\.?[\w.$]+):",
                       re.MULTILINE)
# Symbols referenced by a branch, a jump or a %hi/%lo pair.
USES = re.compile(
    r"(?:%hi|%lo)\((\.?[\w.$]+)\)"
    r"|(?:\b(?:jal|jalr|j|b|beq|bne|beqz|bnez|blez|bgtz|bgez|bltz|"
    r"bgezal|bltzal))\s+(\.?[\w.$]+)")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--asm", default=str(ROOT / "asm/eboot"))
    ap.add_argument("--fix", action="store_true",
                    help="add `.globl` for every undefined label in place")
    ns = ap.parse_args()

    defined: set[str] = set()
    files = sorted(Path(ns.asm).rglob("*.s"))
    texts: dict[Path, str] = {}
    for path in files:
        text = path.read_text(encoding="utf-8")
        texts[path] = text
        for a, b in DEFINES.findall(text):
            defined.add((a or b).strip())

    # Symbols the linker will resolve from config/eboot.symbols.ld.
    ld = ROOT / "config/eboot.symbols.ld"
    if ld.exists():
        for line in ld.read_text(encoding="utf-8").splitlines():
            line = line.split("/*")[0].strip()
            if line and "=" in line:
                defined.add(line.split("=")[0].strip())

    # Labels defined exactly once but referenced from another file must be
    # made global: gas treats `.L`-prefixed symbols as file-local, and splat
    # gives every function its own `.s` file.
    local_defs: dict[str, set[Path]] = {}
    for path, text in texts.items():
        for name in LABEL_DEF.findall(text):
            local_defs.setdefault(name.strip(), set()).add(path)

    cross_refs: dict[str, set[Path]] = {}
    for path, text in texts.items():
        for a, b in USES.findall(text):
            name = (a or b).strip()
            if not name:
                continue
            defs = local_defs.get(name)
            if defs and path not in defs:
                cross_refs.setdefault(name, set()).update(defs)

    undefined: dict[str, set[Path]] = {}
    for path, text in texts.items():
        for a, b in USES.findall(text):
            name = (a or b).strip()
            if not name or name in defined:
                continue
            # Register names and operator keywords are matched by the branch
            # regex but are never linker symbols.
            if name.startswith(("$", "%")):
                continue
            undefined.setdefault(name, set()).add(path)

    print(f"{len(files)} asm files, {len(defined)} defined symbols, "
          f"{len(undefined)} undefined, {len(cross_refs)} cross-file locals")

    for name, users in sorted(undefined.items())[:40]:
        print(f"  UNDEFINED {name}  used by {len(users)} files, e.g. "
              f"{sorted(users)[0].name}")
    for name, owners in sorted(cross_refs.items())[:10]:
        print(f"  CROSS-FILE {name} defined in "
              f"{', '.join(sorted(p.name for p in owners))}")

    if ns.fix and cross_refs:
        for name, owners in cross_refs.items():
            for path in sorted(owners):
                text = texts[path]
                text = re.sub(rf"^(\s*)({re.escape(name)}):",
                              rf"\1.globl {name}\n\1{name}:", text,
                              flags=re.MULTILINE)
                texts[path] = text
        for path, text in texts.items():
            path.write_text(text, encoding="utf-8")
        print(f"promoted {len(cross_refs)} cross-file labels to global")
    return 0


if __name__ == "__main__":
    sys.exit(main())

"""Report which functions in the built ELF differ from the original.

The whole-module comparison in tools/build.py answers "how close are we"; this
answers "what is left to do", by attributing every mismatching byte to the
function that contains it.  That is the decompilation work list, largest
function first.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import pspelf  # noqa: E402


def load_functions() -> list[tuple[int, int, str]]:
    """(start, end, name) for every function, from the splat symbol map."""
    path = ROOT / "config/eboot.symbol_addrs.txt"
    funcs: list[tuple[int, int, str]] = []
    for raw in path.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        # `name = 0xADDR; // type:func` - the type lives in the trailing
        # comment, so the whole line has to be inspected before stripping it.
        if "type:func" not in line or "=" not in line:
            continue
        name, _, value = line.partition("=")
        name = name.strip()
        value = value.split(";")[0].strip()
        try:
            addr = int(value, 0)
        except ValueError:
            continue
        if name and value:
            funcs.append((addr, 0, name))
    funcs.sort()
    out = []
    for i, (addr, _, name) in enumerate(funcs):
        end = funcs[i + 1][0] if i + 1 < len(funcs) else addr + 4
        out.append((addr, end, name))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--target", default=str(ELF_PATH))
    ap.add_argument("--built", default=str(ROOT / "build/eboot.elf"))
    ap.add_argument("--top", type=int, default=30)
    ap.add_argument("--section", default=None,
                    help="only report this section")
    ns = ap.parse_args()

    want = pspelf.load(ns.target)
    got = pspelf.load(ns.built)
    funcs = load_functions()

    bad: list[tuple[int, int, str]] = []
    matched = 0
    for sec in want.sections:
        if ns.section and sec.name != ns.section:
            continue
        if not (sec.flags & 0x2) or sec.type == 8:
            continue
        other = got.section(sec.name)
        if other is None:
            continue
        n = min(len(sec.data), len(other.data))
        for i in range(0, n, 4):
            if sec.data[i:i + 4] != other.data[i:i + 4]:
                bad.append((sec.addr + i, sec.addr + i + 4, sec.name))
        matched += n

    # Attribute each mismatching word to the function that contains it.
    per_function: dict[str, int] = {}
    per_function_bytes: dict[str, int] = {}
    for start, _, name in funcs:
        per_function.setdefault(name, 0)
        per_function_bytes.setdefault(name, 0)

    for addr, _, _ in bad:
        name = "?"
        for start, end, fname in funcs:
            if start <= addr < end:
                name = fname
                break
        per_function[name] = per_function.get(name, 0) + 1
        per_function_bytes[name] = per_function_bytes.get(name, 0) + 4

    total_funcs = len(funcs)
    clean = sum(1 for n in per_function if per_function[n] == 0)
    print(f"functions: {total_funcs}, fully matching: {clean} "
          f"({100.0 * clean / max(total_funcs, 1):.2f}%)")
    print(f"mismatching words: {len(bad)} of {matched // 4}")

    ranked = sorted(per_function.items(), key=lambda kv: -kv[1])
    print(f"\nworst {ns.top} functions by mismatching bytes:")
    for name, count in ranked[:ns.top]:
        if count == 0:
            break
        print(f"  {per_function_bytes[name]:>8}  {count:>6} words  {name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

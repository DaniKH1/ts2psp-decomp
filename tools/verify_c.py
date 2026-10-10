"""Verify C candidates in src/ against the retail machine code.

Byte-exactness is the definition of "decompiled" in this project: a function
counts as done only when its C compiles to the exact instruction words in
BOOT.BIN.  Each candidate is compiled on its own, linked against the absolute
addresses in config/pgs-si2.symbols.ld (so %hi/%lo pairs and jal targets
resolve exactly like the retail link did), and the resulting .text bytes are
compared word by word.

    python tools/verify_c.py                          # check all of src/
    python tools/verify_c.py --function gameUpdate    # one candidate
    python tools/verify_c.py --verbose                # always show details
    python tools/verify_c.py --adopt                  # write matched list
    python tools/verify_c.py --lane gcc46             # only the era lane

Every candidate is tried against every installed toolchain lane (README
"Compiler era"): a MATCH under any lane counts, and the verdict names the
lane that produced it.

Renames: a candidate may use a human name instead of the inventory name; list
the mapping in config/renames.txt as "<inventory_name> <your_name>".
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import pspcc  # noqa: E402
import pspelf  # noqa: E402

SRC = ROOT / "src"
INVENTORY = ROOT / "config" / "functions.txt"
RENAMES = ROOT / "config" / "renames.txt"
MATCHED_LIST = ROOT / "config" / "matched_c.txt"
SYMBOLS_LD = ROOT / "config" / "pgs-si2.symbols.ld"


def load_inventory() -> dict[str, tuple[int, int]]:
    """inventory name -> (addr, size)."""
    table: dict[str, tuple[int, int]] = {}
    for line in INVENTORY.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        addr, size, _section, _tier, name = line.split()
        table[name] = (int(addr, 16), int(size, 16))
    return table


def load_renames() -> dict[str, str]:
    """new (human) name -> inventory name."""
    out: dict[str, str] = {}
    if not RENAMES.exists():
        return out
    for line in RENAMES.read_text(encoding="utf-8").splitlines():
        line = line.split("//")[0].strip()
        if not line:
            continue
        old, new = line.split()[:2]
        out[new] = old
    return out


def build(source: Path, root: Path, stem: str, tmp: str
          ) -> tuple[bytes | None, str]:
    """Compile+link+extract .text for one lane; (bytes, "") or (None, error)."""
    obj = Path(tmp) / f"{stem}.o"
    res = subprocess.run(
        [pspcc.tool("psp-gcc", root), *pspcc.CPPFLAGS, *pspcc.cflags(root),
         *pspcc.OPTFLAGS, "-c", str(source), "-o", str(obj)],
        env=pspcc.env(root), capture_output=True, text=True)
    if res.returncode != 0:
        return None, f"compile failed:\n{res.stderr[-600:]}"

    linked = Path(tmp) / f"{stem}.elf"
    link = subprocess.run(
        [pspcc.tool("psp-ld", root), "-T", str(SYMBOLS_LD),
         "-o", str(linked), str(obj), "-e", stem,
         "--unresolved-symbols=ignore-all",
         "--no-warn-rwx-segments"],
        env=pspcc.env(root), capture_output=True, text=True)
    if link.returncode != 0:
        return None, f"link failed:\n{link.stderr[-600:]}"

    blob = Path(tmp) / f"{stem}.bin"
    out = subprocess.run(
        [pspcc.tool("psp-objcopy", root), "-O", "binary",
         "--only-section=.text*", str(linked), str(blob)],
        env=pspcc.env(root), capture_output=True, text=True)
    if out.returncode != 0:
        return None, f"objcopy failed:\n{out.stderr[-400:]}"
    return blob.read_bytes(), ""


def compare(got: bytes, want: bytes, size: int,
            verbose: bool) -> tuple[bool, str]:
    if len(got) < size:
        return False, f"code too short: {len(got)} < {size} bytes"
    if got[:size] == want:
        extra = len(got) - size
        detail = f"{size} bytes identical"
        if extra:
            # Helper functions compiled into their own .text.* sections; the
            # candidate function itself matched, extras are reported as info.
            detail += f" (+{extra} bytes of helpers)"
        return True, detail

    words = sum(1 for i in range(0, size, 4) if want[i:i + 4] != got[i:i + 4])
    if verbose:
        first = next(i for i in range(0, size, 4) if want[i:i + 4] != got[i:i + 4])
        detail = (f"{words} of {size // 4} words differ, first at +0x{first:03X} "
                  f"(want {want[first:first+4].hex()}, got {got[first:first+4].hex()})")
    else:
        detail = f"{words} of {size // 4} words differ"
    return False, detail


def check_one(source: Path, elf: pspelf.Module, inventory, renames,
              verbose: bool, allowed: list[tuple[Path, str]]
              ) -> tuple[str, bool, str]:
    stem = source.stem
    inv_name = stem if stem in inventory else renames.get(stem)
    if inv_name is None:
        return stem, False, f"{source.name}: no inventory entry " \
                            f"(add to config/renames.txt?)"
    addr, size = inventory[inv_name]
    want = elf.read(addr, size)

    # Try every lane until one reproduces the retail bytes; a MATCH names the
    # lane, a failure lists each lane's own diagnosis.
    attempts: list[tuple[str, bool, str]] = []
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as tmp:
        for root, label in allowed:
            got, err = build(source, root, stem, tmp)
            ok, detail = (False, err) if err else compare(got, want, size,
                                                          verbose)
            attempts.append((label, ok, detail))
            if ok:
                break

    for label, ok, detail in attempts:
        if ok:
            return stem, True, f"{detail} [{label}]"

    details = [d for _, _, d in attempts]
    if len(set(details)) == 1:
        return stem, False, f"{details[0]} [{','.join(l for l, _, _ in attempts)}]"
    joined = " | ".join(f"{d} [{l}]" for l, _, d in attempts)
    return stem, False, joined


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--function", action="append",
                    help="check only this candidate (repeatable)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--adopt", action="store_true",
                    help="write config/matched_c.txt from the results")
    ap.add_argument("--lane", action="append", metavar="LABEL",
                    help="restrict to a toolchain lane (gcc15, gcc46)")
    ap.add_argument("--verbose", action="store_true")
    ns = ap.parse_args()

    pspcc.require_toolchain()
    allowed = pspcc.lanes()
    if ns.lane:
        unknown = [x for x in ns.lane
                   if x not in {label for _, label in allowed}]
        if unknown:
            known = ", ".join(label for _, label in allowed) or "none"
            sys.exit(f"unknown/unbuilt lane(s): {', '.join(unknown)} "
                     f"(available: {known})")
        allowed = [(r, l) for r, l in allowed if l in ns.lane]
    if not SYMBOLS_LD.exists():
        sys.exit(f"{SYMBOLS_LD} missing - run: python tools/linkerscript.py")

    sources = sorted(SRC.glob("*.c"))
    if ns.function:
        missing = [n for n in ns.function if not (SRC / f"{n}.c").exists()]
        if missing:
            sys.exit(f"no such candidate: {', '.join(missing)}")
        sources = [SRC / f"{n}.c" for n in ns.function]
    if not sources:
        print("no candidates in src/")
        return 0

    (ROOT / "build").mkdir(exist_ok=True)
    inventory = load_inventory()
    renames = load_renames()
    print(f"lanes: {', '.join(l for _, l in allowed)}")

    with pspelf.Module() as elf, ThreadPoolExecutor(max_workers=ns.jobs) as pool:
        results = list(pool.map(
            lambda s: check_one(s, elf, inventory, renames, ns.verbose,
                                allowed), sources))

    matched: list[str] = []
    for name, ok, detail in sorted(results):
        mark = "MATCH  " if ok else "DIFFERS"
        print(f"{mark} {name:<36} {detail}")
        if ok:
            matched.append(name)

    print(f"\n{len(matched)}/{len(results)} functions match byte for byte")

    if ns.adopt:
        existing: set[str] = set()
        if MATCHED_LIST.exists():
            existing = {
                line.split("//")[0].strip()
                for line in MATCHED_LIST.read_text(encoding="utf-8").splitlines()
                if line.split("//")[0].strip() and not line.startswith("//")
            }
        wanted = {m for m in existing if (SRC / f"{m}.c").exists()}
        wanted |= set(matched)
        header = ("// Functions whose C reproduces the original bytes exactly.\n"
                  "// GENERATED by tools/verify_c.py --adopt\n\n")
        MATCHED_LIST.write_text(header + "".join(f"{m}\n" for m in sorted(wanted)),
                                encoding="utf-8")
        print(f"wrote config/matched_c.txt ({len(wanted)} functions)")
    return 0


if __name__ == "__main__":
    sys.exit(main())

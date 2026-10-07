#!/usr/bin/env python3
"""Check that every function linked from C reports the size it should.

Why this exists, because it is not an obvious thing to check:

**GCC does not count an assembler-filled delay slot in a function's symbol size.**
Left to emit its own return, GCC writes `jr $ra`, the assembler appends a `nop` to
fill the slot, and the symbol ends up four bytes smaller than the code that is
actually there - the object measures 0x20 while the symbol says 0x1c.

One such function is harmless: the linker pads to the next symbol's address and the
bytes come out right.  Two of them cost eight bytes of `.text`, and everything after
them shifts - which is exactly what happened, and it showed up as the module image
going from 100 % to 55 % identical with no error anywhere.  `check_image.py` caught
it, but only after a full 60-second rebuild.

The fix is to write the `jr $ra` and its `nop` inside the asm block, so GCC counts
the `nop`.  The point of this script is to make the remaining cases obvious: a
matched function whose symbol is the wrong size is a landmine, because it will be
absorbed until enough of them accumulate.

    python tools/check_symbols.py           # every matched function
    python tools/check_symbols.py --all     # every function in the symbol map
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from paths import ELF_PATH, ROOT  # noqa: E402
import pspelf  # noqa: E402

SPLAT = ROOT / "asm" / "eboot"
MATCHED = ROOT / "config" / "matched_c.txt"
BUILT = ROOT / "build" / "eboot.elf"

# splat records the byte count next to the name: `nonmatching func_X, 0x20`.
NONMATCHING = re.compile(r"^nonmatching (\w+), (0x[0-9A-Fa-f]+)", re.MULTILINE)


def recorded_sizes() -> dict[str, int]:
    """The byte count splat wrote down for each function."""
    sizes: dict[str, int] = {}
    for path in SPLAT.glob("*.s"):
        text = path.read_text(encoding="utf-8", errors="replace")
        for name, size in NONMATCHING.findall(text):
            sizes[name] = int(size, 16)
    return sizes


def linked_sizes(path: Path) -> dict[str, int]:
    """The size each function's symbol has in a linked ELF."""
    import subprocess
    from build import env, tool  # noqa: E402

    out = subprocess.run(
        [tool("psp-nm"), "-S", str(path)],
        env=env(), capture_output=True, text=True,
    )
    if out.returncode != 0:
        raise SystemExit(f"psp-nm failed:\n{out.stderr[-400:]}")
    sizes: dict[str, int] = {}
    for line in out.stdout.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] == "A":
            sizes[parts[3]] = int(parts[1], 16)
    return sizes


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--all", action="store_true",
                    help="check every function, not just the hand-written ones")
    ns = ap.parse_args()

    if not BUILT.exists():
        print(f"no linked ELF at {BUILT}; run tools/build.py first", file=sys.stderr)
        return 1

    want = recorded_sizes()
    got = linked_sizes(BUILT)

    if ns.all:
        names = sorted(want)
        scope = "every function"
    else:
        names = sorted(
            line.split()[0]
            for line in MATCHED.read_text(encoding="utf-8").splitlines()
            if line.strip() and not line.startswith("//")
        ) if MATCHED.exists() else []
        scope = "every function linked from C"

    wrong: list[tuple[str, int, int | None]] = []
    missing: list[str] = []
    for name in names:
        expected = want.get(name)
        if expected is None:
            continue
        actual = got.get(name)
        if actual is None:
            missing.append(name)
        elif actual != expected:
            wrong.append((name, expected, actual))

    print(f"{len(names)} functions checked ({scope})")
    if missing:
        print(f"  {len(missing)} absent from the linked ELF: {missing[:5]}")
    if wrong:
        print(f"\n  {len(wrong)} have the wrong symbol size:")
        for name, expected, actual in wrong:
            print(f"    {name}: recorded 0x{expected:x}, symbol "
                  f"{'absent' if actual is None else f'0x{actual:x}'} "
                  f"({expected - (actual or 0):+d})")
        print("\n  Each of these is a delay-slot nop GCC did not count.")
        print("  Write the `jr $ra` and its `nop` inside the asm block, with")
        print("  __attribute__((noreturn)), so GCC counts the nop.")
        return 1

    print("  all sizes agree")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
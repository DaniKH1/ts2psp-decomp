"""Group the static constructors into subsystems by the tags they register.

`python tools/subsystems.py` reads a constructor's strings literally, which
makes it guess wrong often: most constructors call the same shared helper that
registers the module-wide table (`wave`, `sdta`, `xms `, ...), so the first
capitalised string is a control name like `JoystickX` rather than the module it
belongs to.

The tag *set* is a much better discriminator.  Every constructor registers the
shared baseline, and each adds the tags of its own subsystem on top:

    func_000103AC: surf, gshd, bmsh, body, banm        -> character mesh/animation
    func_00008E90: ... gshd, levl, node, ndbg, rdms    -> level streaming

So the baseline is whatever most constructors agree on, and the residue after
subtracting it groups the rest.  Tag sets that recur across several constructors
are genuine subsystems; the singletons are the ones worth reading the strings
for.

    python tools/subsystems.py          # grouped by tag signature
    python tools/subsystems.py --min 3  # only signatures seen 3+ times
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import pspelf  # noqa: E402
import tags as tags_mod  # noqa: E402

CPLINIT_HEADER = 8
# Every constructor registers at least this many tags; anything at or below it
# is the shared loader baseline rather than a subsystem marker.
BASELINE_MAX = 8


def load_sizes() -> dict[str, int]:
    out: dict[str, int] = {}
    for path in (ROOT / "asm/eboot").glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                out[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break
    return out


def read_cstring(elf, addr: int, limit: int = 64) -> str | None:
    data = elf.read(addr, limit)
    end = data.find(b"\x00")
    if end <= 0:
        return None
    raw = data[:end]
    if not all(32 <= b < 127 for b in raw):
        return None
    return raw.decode("ascii")


def literals(elf, start: int, size: int) -> list[str]:
    found: list[str] = []
    seen: set[int] = set()
    for rel in elf.relocs():
        if not (start <= rel.offset < start + size) or rel.target in seen:
            continue
        text = read_cstring(elf, rel.target)
        if text and len(text) > 2:
            seen.add(rel.target)
            found.append(text)
    return found


def constructor_tags(elf, addr: int, size: int) -> list[str]:
    out = []
    for value, _ in tags_mod.materialised_pairs(elf, addr, size):
        text = tags_mod.decode(value)
        if text:
            out.append(text)
    return out


def interesting_strings(texts: list[str], limit: int = 3) -> list[str]:
    """Literals likely to name the module rather than a value inside it.

    Tuned by hand: the shared table means most constructors mention button and
    joystick names, so those are dropped, and what is left tends to be behaviour
    names, which is what identifies a module.
    """
    skip = {"JoystickX", "JoystickY", "Confirm", "Cancel", "Button_Circle",
            "Button_Square", "Button_Triangle", "Button_X", "Button_L",
            "Button_R", "SonyButtonMap"}
    out = [t for t in texts if t not in skip and not t.startswith("Button_")]
    return out[:limit]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--min", type=int, default=2,
                    help="only show signatures shared by at least N "
                         "constructors")
    ap.add_argument("--strings", type=int, default=3,
                    help="how many literals to show per group")
    ns = ap.parse_args()

    elf = pspelf.load(str(ELF_PATH))
    names: dict[int, str] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "=" not in line:
            continue
        nm = line.split("=", 1)[0].strip()
        val = line.split("=")[1].split(";")[0].strip()
        try:
            names.setdefault(int(val, 16), nm)
        except ValueError:
            continue
    sizes = load_sizes()

    data = elf.section(".cplinit").data
    rows = []
    for i in range(0, len(data), 4):
        addr = struct.unpack_from("<I", data, i)[0]
        if not addr:
            continue
        name = names.get(addr, f"sub_{addr:08x}")
        size = sizes.get(name, 0)
        rows.append((i + CPLINIT_HEADER, addr, name,
                     constructor_tags(elf, addr, size),
                     literals(elf, addr, size)))

    # Half the constructors register no tags at all - they only touch the shared
    # runtime - so "more than half" finds nothing.  The baseline is better read
    # as what the constructors that *do* register tags agree on, and the sound
    # and script tags are the giveaway: every translation unit links the shared
    # asset loader, so `wave`, `sdta` and the `xms ` family are infrastructure.
    tagged = [r for r in rows if r[3]]
    counts: dict[str, int] = defaultdict(int)
    for _, _, _, tset, _ in tagged:
        for tag in set(tset):
            counts[tag] += 1
    cutoff = max(1, len(tagged) // 2)
    baseline = {t for t, n in counts.items() if n >= cutoff}

    groups: dict[tuple[str, ...], list] = defaultdict(list)
    for row in rows:
        extra = tuple(sorted(set(row[3]) - baseline))
        groups[extra or ("<shared loader only>",)].append(row)

    print(f"{len(rows)} constructors, {len(tagged)} register tags; "
          f"baseline (>= {cutoff}): {', '.join(sorted(baseline))}\n")
    print(f"{len(groups)} distinct tag signatures\n")

    ordered = sorted(groups.items(),
                     key=lambda kv: (-len(kv[1]), kv[0]))
    for extra, members in ordered:
        if len(members) < ns.min:
            continue
        print(f"  {len(members):>3}x  +{', '.join(extra) or '(none)'}")
        for slot, addr, name, _, texts in members[:6]:
            lit = interesting_strings(texts, ns.strings)
            print(f"        [{slot:>#6x}] {name:<26} "
                  f"{', '.join(repr(t) for t in lit)}")
        if len(members) > 6:
            print(f"             ... and {len(members) - 6} more")
    return 0


if __name__ == "__main__":
    sys.exit(main())

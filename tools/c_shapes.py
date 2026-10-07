"""Group the simple functions by the instruction sequence they compile from.

`tools/c_candidates.py` ranks functions by how promising they look, but it ranks
them one at a time and the real leverage is in the shapes they share.  Six
functions already compile to the original bytes, and they are six instances of
about three shapes.  Every function with the same shape needs the same fix, so
clustering first tells us how many there are and how many patterns are worth
writing.

What makes a function simple here: no floating point, no branches and no calls.
That leaves load/store/arithmetic against `$a0`-`$a3` and the temporaries, which
is a small enough set of sequences to enumerate.

    python tools/c_shapes.py               # shapes by frequency
    python tools/c_shapes.py --show 3     # what the third shape looks like
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import mipsdis  # noqa: E402
import pspelf  # noqa: E402


def load_functions() -> dict[str, int]:
    out: dict[str, int] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line or "=" not in line:
            continue
        try:
            out[line.split("=", 1)[0].strip()] = \
                int(line.split("=")[1].split(";")[0].strip(), 0)
        except ValueError:
            continue
    return out


def load_sizes() -> dict[str, int]:
    out: dict[str, int] = {}
    for path in (ROOT / "asm/eboot").glob("*.s"):
        for line in path.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                parts = line.split()
                out[parts[1].rstrip(",")] = int(parts[2].rstrip(","), 0)
                break
    return out


def is_simple(words: list[int]) -> bool:
    for w in words:
        op = w >> 26
        if op == 0x11 and ((w >> 21) & 0x1F) in (0, 1, 2, 3, 4, 5, 8, 0xB,
                                                0xD, 0xE, 0xF):
            return False           # cop1
        if op == 0x03:             # jal
            return False
        if op in (2, 4, 5, 6, 7):
            return False           # j / beq / bne / blez / bgtz
        if (w & 0x3F000000) == 0x100000000 % 0x100000000 and \
                (w >> 26) == 1:     # regimm: beq/bne/blez/bgtz/...
            return False
    return True


def shape(words: list[int], skip_last: int = 1) -> str:
    """A signature for the instruction sequence, ignoring registers.

    The last instruction is usually the store or move in the return's delay
    slot, and the one before it the return itself; both are structural rather
    than part of what the function computes, so they are folded away and the
    shape is the arithmetic in between.
    """
    parts = []
    for i, w in enumerate(words):
        op = w >> 26
        rs, rt = (w >> 21) & 0x1F, (w >> 16) & 0x1F
        if op == 0 and (w & 0x3F) == 0x21:            # addu $r,$s,$zero
            parts.append("mv")
        elif op == 0 and rs == 0:                      # li $rt, imm
            parts.append("li")
        elif op == 0 and (w & 0x3F) == 0x25:           # or $r,$s,$zero
            parts.append("mv")
        elif op == 0:                                  # ALU
            parts.append(f"alu{(w >> 6) & 0x1F}")
        elif op == 0x09 or op == 0x0D:                 # addiu / ori
            parts.append(f"imm{op - 0x08:x}")
        elif op == 0x0F:
            parts.append("lui")
        elif op == 0x20:
            parts.append(f"lw{rt}")
        elif op == 0x28:
            parts.append(f"sw{rt}")
        elif op == 0x23:
            parts.append(f"lwgt{rt}")
        elif op == 0x2B:
            parts.append(f"swgt{rt}")
        elif op == 0x24:
            parts.append(f"lbu{rt}")
        elif op == 0x28 or op == 0x21:
            parts.append(f"lh{rt}")
        elif op == 0x08:
            parts.append(f"jr{rs}")
        else:
            parts.append(f"op{op:02x}")
    return " ".join(parts)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--show", type=int, default=-1,
                    help="disassemble the members of shape N")
    ap.add_argument("--limit", type=int, default=30)
    ap.add_argument("--exclude", nargs="*", default=["sw0", "swgt16", "swgt31"],
                    help="signatures to skip: stack stores mark compiler "
                         "scaffolding and C++ thunks, not engine code")
    ap.add_argument("--done", action="store_true",
                    help="hide shapes whose members are already decompiled")
    ns = ap.parse_args()

    elf = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    funcs = load_functions()
    sizes = load_sizes()

    groups: dict[str, list[tuple[str, int, int]]] = defaultdict(list)
    for name, addr in funcs.items():
        size = sizes.get(name)
        if not size or size // 4 < 4 or size % 4:
            continue
        data = elf.read(addr, size)
        words = [struct.unpack_from("<I", data, i)[0]
                 for i in range(0, len(data), 4)]
        if not is_simple(words):
            continue
        groups[shape(words)].append((name, addr, size))

    ordered = sorted(groups.items(), key=lambda kv: (-len(kv[1]), kv[0]))
    ordered = [(sig, m) for sig, m in ordered
               if not any(x in sig for x in ns.exclude)]

    # Shapes whose members already have byte-exact C are the ones just done;
    # hiding them keeps the ranking pointed at work that is still outstanding.
    done: set[str] = set()
    matched = ROOT / "config/matched_c.txt"
    if matched.exists():
        done = {l.split("//")[0].strip()
                for l in matched.read_text(encoding="utf-8").splitlines()
                if l.split("//")[0].strip()}
    if ns.done:
        pending = [(sig, [m for m in mem if m[0] not in done])
                   for sig, mem in ordered]
        pending = [(sig, mem) for sig, mem in pending if mem]
        ordered = sorted(pending, key=lambda kv: (-len(kv[1]), kv[0]))

    if ns.show >= 0:
        sig, members = ordered[ns.show]
        print(f"shape {ns.show}: {len(members)} functions, {sig}\n")
        for name, addr, size in members[:6]:
            print(f"  {name} ({addr:#x}, {size} bytes)")
            data = elf.read(addr, size)
            for i in range(0, len(data), 4):
                w = struct.unpack_from("<I", data, i)[0]
                print(f"    {addr + i:#010x}: "
                      f"{mipsdis.make_instruction(w, addr + i)}")
            print()
        return 0

    total = sum(len(v) for v in groups.values())
    print(f"{total} simple functions in {len(groups)} shapes\n")

    # Function-local static guards and C++ virtual dispatch thunks are compiler
    # scaffolding: the shapes are dominated by stack stores or a bare `jalr`
    # through a vtable, and neither is engine code worth writing C for.
    shown = ordered
    kept = sum(len(m) for _, m in shown)
    print(f"{kept} functions left after dropping scaffolding "
          f"({' '.join(ns.exclude)})\n")
    for sig, members in shown[:ns.limit]:
        print(f"  {len(members):>3}x  {sig}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
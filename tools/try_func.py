"""Compile one candidate C file and diff it against the original function.

    python tools/try_func.py func_00000000
    python tools/try_func.py func_00000000 --flags -O1

Prints the assembled bytes of the built object next to the original's, so the
difference is visible directly.  Nothing is written to `src/`, which makes this
safe to use while hunting for the right source and compiler flags.
"""

from __future__ import annotations

import argparse
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from paths import ELF_PATH  # noqa: E402

import mipsdis  # noqa: E402
import pspelf  # noqa: E402
from build import ASFLAGS, CPPFLAGS, CFLAGS, env, tool  # noqa: E402

DEFAULT_FLAGS = ["-O2"]


def function_bounds(elf, name: str) -> tuple[int, int]:
    """Exact (start, end) of a function, using the `nonmatching` size splat
    recorded in its assembly - the symbol map only knows where it starts."""
    starts = []
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line:
            continue
        fname = line.split("=", 1)[0].strip()
        addr = int(line.split("=")[1].split(";")[0].strip(), 0)
        if fname == name:
            starts.append(addr)
    if not starts:
        raise SystemExit(f"no such function: {name}")
    start = starts[0]
    asm = ROOT / "asm/eboot" / f"{name}.s"
    if asm.exists():
        for line in asm.read_text(encoding="utf-8").splitlines():
            if line.startswith("nonmatching"):
                return start, start + int(line.split(",")[1].strip(), 0)
    later = [a for a in starts if a > start]
    return start, (later[0] if later else start + 0x40)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("function")
    ap.add_argument("--source", help="defaults to src/eboot/<name>.c")
    ap.add_argument("--flags", nargs="*", default=DEFAULT_FLAGS,
                    help="compiler flags to try")
    ap.add_argument("--opt", action="append", default=[],
                    help="optimisation level, e.g. --opt -O1 "
                         "(separate because --flags swallows -O1)")
    ap.add_argument("--quiet", action="store_true")
    ns = ap.parse_args()

    source = Path(ns.source) if ns.source else \
        ROOT / "src/eboot" / f"{ns.function}.c"
    if not source.exists():
        raise SystemExit(f"missing {source}")

    elf = pspelf.load(str(ELF_PATH))
    start, end = function_bounds(elf, ns.function)
    want = elf.read(start, end - start)

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "f.o"
        cmd = [tool("psp-gcc"), *CPPFLAGS, *CFLAGS, *ns.opt, *ns.flags,
               "-c", str(source), "-o", str(obj)]
        res = subprocess.run(cmd, env=env(), capture_output=True, text=True)
        if res.returncode != 0:
            print(res.stdout)
            print(res.stderr)
            return 1

        # Link against the absolute symbol assignments, so an inline `%%hi(sym)`
        # resolves.  Without it every data reference assembles as zero and the
        # comparison shows differences that are not there.  The result is
        # relocatable at the module's own addresses, exactly like the build.
        linked = Path(tmp) / "f.elf"
        link = subprocess.run(
            [tool("psp-ld"), "-T", str(ROOT / "config/eboot.symbols.ld"),
             "-o", str(linked), str(obj), "-e", ns.function,
             "--unresolved-symbols=ignore-all",
             "--no-warn-rwx-segments"],
            env=env(), capture_output=True, text=True)
        if link.returncode != 0:
            print(link.stderr[-2000:])
            return 1

        out = subprocess.run(
            [tool("psp-objcopy"), "-O", "binary", "--only-section=.text*",
             str(linked), str(Path(tmp) / "f.bin")],
            env=env(), capture_output=True, text=True)
        if out.returncode != 0 or not (Path(tmp) / "f.bin").exists():
            print(out.stdout, out.stderr)
            return 1
        got = (Path(tmp) / "f.bin").read_bytes()

    print(f"{ns.function}: original {len(want)} bytes, built {len(got)} bytes")
    n = max(len(want), len(got))
    got += b"\x00" * (n - len(got))
    diff = sum(1 for i in range(0, n, 4)
               if want[i:i + 4] != got[i:i + 4])
    print(f"{'MATCH' if diff == 0 and len(got) == len(want) else 'DIFFERS'}"
          f": {diff} differing words")
    if diff and not ns.quiet:
        print()
        print(f"{'addr':<10}{'original':<36}{'built':<36}")
        shown = 0
        for i in range(0, n, 4):
            a = want[i:i + 4]
            b = got[i:i + 4]
            if a == b:
                continue
            aw = struct.unpack("<I", a.ljust(4, b"\0"))[0] if len(a) == 4 else 0
            bw = struct.unpack("<I", b.ljust(4, b"\0"))[0] if len(b) == 4 else 0
            print(f"{start + i:<10x}{str(mipsdis.make_instruction(aw, start + i)):<36}"
                  f"{str(mipsdis.make_instruction(bw, start + i)):<36}")
            shown += 1
            if shown >= 40:
                print("...")
                break
    return 0 if diff == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

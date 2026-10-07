"""Verify the C in `src/eboot` against the original machine code.

The retail EBOOT was built with CodeWarrior for PSP, whose instruction
selection psp-gcc does not reproduce (see progress.md), so a C function only
counts as "decompiled" once it compiles to the original bytes.  This compiles
every `src/eboot/*.c` on its own, compares it with the original function, and
writes the ones that match to `config/matched_c.txt` - the list the linker
script uses to decide what to link from `src/` instead of `asm/`.

    python tools/verify_c.py            # check everything
    python tools/verify_c.py --function func_000000B0 --verbose
    python tools/verify_c.py --adopt    # keep only matching functions linked
"""

from __future__ import annotations

import argparse
import os
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import mipsdis  # noqa: E402
import pspelf  # noqa: E402
from build import CFLAGS, CPPFLAGS, OPTFLAGS, env, tool  # noqa: E402

MATCHED_LIST = ROOT / "config/matched_c.txt"


def load_functions() -> dict[str, int]:
    """function name -> start address, from the splat symbol map."""
    out: dict[str, int] = {}
    for line in (ROOT / "config/eboot.symbol_addrs.txt") \
            .read_text(encoding="utf-8").splitlines():
        if "type:func" not in line or "=" not in line:
            continue
        name = line.split("=", 1)[0].strip()
        value = line.split("=")[1].split(";")[0].strip()
        try:
            out[name] = int(value, 0)
        except ValueError:
            continue
    return out


def function_size(name: str) -> int | None:
    """Byte count, taken from the `nonmatching <name>, <size>` splat recorded."""
    asm = ROOT / "asm/eboot" / f"{name}.s"
    if not asm.exists():
        return None
    for line in asm.read_text(encoding="utf-8").splitlines():
        if line.startswith("nonmatching"):
            return int(line.split(",")[1].strip(), 0)
    return None


def check_one(source: Path, elf, sizes) -> tuple[str, bool, str]:
    name = source.stem
    addr = sizes.get(name)
    if addr is None:
        return name, False, "not in the symbol map"
    size = function_size(name)
    if size is None:
        return name, False, "no size recorded"

    want = elf.read(addr, size)
    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "f.o"
        res = subprocess.run(
            [tool("psp-gcc"), *CPPFLAGS, *CFLAGS, *OPTFLAGS, "-c", str(source),
             "-o", str(obj)],
            env=env(), capture_output=True, text=True)
        if res.returncode != 0:
            return name, False, f"compile failed:\n{res.stderr[-600:]}"
        # Link against the real absolute symbol assignments before comparing.
        # Without this an inline `%%hi(sym)` assembles to zero, because the
        # candidate is compiled on its own and nothing defines the address - the
        # relocation only resolves at link time, which is why the build needs
        # config/eboot.symbols.ld and a candidate check needs it too.
        linked = Path(tmp) / "f.elf"
        # `--unresolved-symbols=ignore-all`: a candidate that calls into the
        # rest of the engine references functions that live in other objects,
        # and linking them is not the point here - only the candidate's own
        # bytes are being compared.  Callees are still resolved when the
        # candidate is finally linked by the build.
        link = subprocess.run(
            [tool("psp-ld"), "-T", str(ROOT / "config/eboot.symbols.ld"),
             "-o", str(linked), str(obj), "-e", name,
             "--unresolved-symbols=ignore-all",
             "--no-warn-rwx-segments"],
            env=env(), capture_output=True, text=True)
        if link.returncode != 0:
            return name, False, f"link failed:\n{link.stderr[-600:]}"
        out = subprocess.run(
            [tool("psp-objcopy"), "-O", "binary", "--only-section=.text*",
             str(linked), str(Path(tmp) / "f.bin")],
            env=env(), capture_output=True, text=True)
        if out.returncode != 0:
            return name, False, f"objcopy failed:\n{out.stderr[-400:]}"
        got = (Path(tmp) / "f.bin").read_bytes()

    if got == want:
        return name, True, f"{size} bytes identical"
    if len(got) != size:
        return name, False, (f"size {len(got)} != {size}")
    diff = sum(1 for i in range(0, size, 4) if want[i:i + 4] != got[i:i + 4])
    return name, False, f"{diff} of {size // 4} words differ"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--function", action="append",
                    help="check only this function (repeatable)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--adopt", action="store_true",
                    help="write config/matched_c.txt from the results")
    ap.add_argument("--verbose", action="store_true")
    ns = ap.parse_args()

    src_dir = ROOT / "src/eboot"
    sources = sorted(src_dir.glob("*.c"))
    if ns.function:
        sources = [src_dir / f"{n}.c" for n in ns.function]
    if not sources:
        print("no C sources to check")
        return 0

    elf = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    sizes = load_functions()

    with ThreadPoolExecutor(max_workers=ns.jobs) as pool:
        results = list(pool.map(lambda s: check_one(s, elf, sizes), sources))

    matched = []
    for name, ok, detail in sorted(results):
        mark = "MATCH  " if ok else "DIFFERS"
        print(f"{mark} {name:<32} {detail if ns.verbose or not ok else ''}")
        if ok:
            matched.append(name)

    print(f"\n{len(matched)}/{len(results)} functions match byte for byte")

    if ns.adopt:
        existing = set()
        if MATCHED_LIST.exists():
            existing = {l.split("//")[0].strip()
                        for l in MATCHED_LIST.read_text(encoding="utf-8")
                        .splitlines() if l.split("//")[0].strip()}
        # Keep previously verified functions that still have a source file.
        wanted = {m for m in existing
                  if (src_dir / f"{m}.c").exists() or m in matched}
        wanted |= set(matched)
        header = ("// Functions whose C reproduces the original bytes exactly.\n"
                  "// GENERATED by tools/verify_c.py --adopt\n\n")
        MATCHED_LIST.write_text(
            header + "".join(f"{m}\n" for m in sorted(wanted)),
            encoding="utf-8")
        print(f"wrote {MATCHED_LIST} ({len(wanted)} functions)")
    return 0


if __name__ == "__main__":
    sys.exit(main())

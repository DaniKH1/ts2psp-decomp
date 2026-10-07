#!/usr/bin/env python3
"""Build driver for The Sims 2 PSP decompilation.

Mirrors the layout used by the reference project (mhp2g-decomp): splat owns the
segment/linker-script layout and the per-function asm, this script compiles the
C sources plus the asm into an ELF with the PSP toolchain and then reports how
much of it matches the original.

    python tools/build.py            # build everything
    python tools/build.py --diff     # build, then compare with the original
    python tools/build.py --stats    # just compare
"""

from __future__ import annotations

import argparse
import concurrent.futures
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
PSP_BIN = ROOT / "bin" / "pspdev" / "bin"
MSYS_BIN = Path(r"C:\msys64\usr\bin")

# ---------------------------------------------------------------------------
# Compiler flags
#
# The retail EBOOT was produced by psp-gcc.  The evidence in the binary:
#   * `-G0`        no small data -> no .sdata/.sbss and not a single
#                  R_MIPS_GPREL16 relocation, every global is reached with an
#                  absolute %hi/%lo pair
#   * `-fno-pic`   absolute addressing, no GOT indirection
#   * `-mabi=eabi` the ELF header reads `0x10a23001, noreorder, allegrex,
#                  eabi32, mips2`, which is exactly psp-gcc's default
#   * `-ffunction-sections`  the original translation units survive as
#                  `.text.collision`, `.text.drawing`, ...
#   * `-fno-common`          tentative C++ definitions landed in `.linkonce.d`
# ---------------------------------------------------------------------------
CPPFLAGS = ["-Iinclude"]
CFLAGS = [
    "-G0", "-mabi=eabi", "-march=allegrex",
    "-fno-pic", "-fno-common", "-ffunction-sections", "-fdata-sections",
    "-fno-strict-aliasing",
]
CXXFLAGS = CFLAGS + [
    "-fno-exceptions", "-fno-rtti", "-std=gnu++98",
]
ASFLAGS = ["-march=allegrex", "-mabi=eabi", "-Iinclude"]
# Kept separate from CFLAGS because tools/verify_c.py overrides it per candidate
# while the build must not.
OPTFLAGS = ["-O2"]
# `#include "..."` with either quote style, for the incremental header check.
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*["<]([^">]+)[">]', re.MULTILINE)
# Set by --force: rebuild every unit instead of only the stale ones.
FORCE = False


def tool(name: str) -> str:
    return str(PSP_BIN / f"{name}.exe")


def env() -> dict:
    """Environment for the MSYS2/Cygwin hosted PSP toolchain.

    The toolchain is a Cygwin build, so it insists on POSIX paths for its
    temporary files - Windows ones are not reachable through its
    /cygdrive/... view, which is why a plain TEMP override fails.
    """
    e = dict(os.environ)
    e["PATH"] = f"{MSYS_BIN};{PSP_BIN};" + e.get("PATH", "")
    e["TMPDIR"] = "/tmp"
    e["TMP"] = "/tmp"
    e["TEMP"] = "/tmp"
    return e


def run(cmd: list[str]) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, env=env(), capture_output=True, text=True)


def obj_path(src: Path) -> Path:
    """Object path, matching what splat wrote into the linker script.

    splat names every object `build/<path-of-source>.o` so the generated linker
    script can list them explicitly.
    """
    return BUILD / src.relative_to(ROOT).with_suffix(".o")


def sources(subdir: str, suffixes: tuple[str, ...]) -> list[Path]:
    d = ROOT / subdir
    if not d.is_dir():
        return []
    out: list[Path] = []
    for suffix in suffixes:
        out.extend(sorted(d.rglob(f"*{suffix}")))
    return out


def compile_one(src: Path) -> tuple[bool, str]:
    obj = obj_path(src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    rel = src.relative_to(ROOT).as_posix()
    # `-O2` is not optional: without it psp-gcc defaults to -O0 and emits
    # different code from the one tools/verify_c.py checks a candidate against,
    # so a function that verified would still break the build.
    if src.suffix == ".c":
        cmd = [tool("psp-gcc"), *CPPFLAGS, *CFLAGS, *OPTFLAGS,
               "-c", str(src), "-o", str(obj)]
    elif src.suffix in (".cpp", ".cc", ".cxx"):
        cmd = [tool("psp-g++"), *CPPFLAGS, *CXXFLAGS, *OPTFLAGS,
               "-c", str(src), "-o", str(obj)]
    elif src.suffix in (".s", ".S"):
        cmd = [tool("psp-as"), *ASFLAGS, "-o", str(obj), str(src)]
    else:
        return True, ""
    res = run(cmd)
    if res.returncode != 0 or not obj.exists():
        return False, f"{rel}\n{res.stdout}\n{res.stderr}"
    return True, ""


def binary_to_object(src: Path) -> tuple[bool, str]:
    """Turn a raw asset blob into an object file with a `.data` payload."""
    rel = src.relative_to(ROOT).as_posix()
    obj = obj_path(src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    symbol = src.stem.replace(".", "_")
    asm = obj.with_suffix(".s")
    data = src.read_bytes()
    lines = [
        "    .section .data",
        "    .align 2",
        f"    .global {symbol}",
        f"    .type {symbol}, @object",
        f"    .size {symbol}, {len(data)}",
        f"{symbol}:",
    ]
    for i in range(0, len(data), 4):
        word = int.from_bytes(data[i:i + 4].ljust(4, b"\0"), "little")
        lines.append(f"    .word 0x{word:08X}")
    asm.write_text("\n".join(lines) + "\n", encoding="utf-8")
    res = run([tool("psp-as"), *ASFLAGS, "-o", str(obj), str(asm)])
    if res.returncode != 0 or not obj.exists():
        return False, f"{rel}\n{res.stdout}\n{res.stderr}"
    return True, ""


def stale(src: Path) -> bool:
    """True when `src` is newer than its object, or the object is missing.

    The module is ~15,000 translation units and only one or two change per
    decompilation iteration, so rebuilding all of them dominated the iteration
    time.  A source is skipped when its object is at least as new *and* every
    header it includes is too - so editing `include/types.h` still rebuilds
    everything that uses it.

    `--force` bypasses this entirely, which is what a from-scratch verification
    run wants.
    """
    if FORCE:
        return True
    obj = obj_path(src)
    if not obj.exists():
        return True
    try:
        if src.stat().st_mtime > obj.stat().st_mtime:
            return True
    except OSError:
        return True
    if src.suffix in (".c", ".cpp", ".cc", ".cxx", ".h"):
        seen: set[Path] = set()
        newest = 0.0
        for header in headers_of(src, seen):
            try:
                newest = max(newest, header.stat().st_mtime)
            except OSError:
                return True
        if newest > obj.stat().st_mtime:
            return True
    return False


def headers_of(src: Path, seen: set[Path] | None = None) -> list[Path]:
    """The headers `src` includes, resolved relative to the include paths.

    A crude `#include "..."` scan is enough here: the decompilation's own
    headers are few, they never recurse much, and getting this wrong only costs
    a redundant recompile.
    """
    if seen is None:
        seen = set()
    out: list[Path] = []
    if src.suffix not in (".c", ".cpp", ".cc", ".cxx", ".h"):
        return out
    try:
        text = src.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return out
    for name in INCLUDE_RE.findall(text):
        if name in seen:
            continue
        for base in (src.parent, ROOT / "include"):
            candidate = (base / name).resolve()
            if candidate.is_file():
                seen.add(name)
                out.append(candidate)
                out.extend(headers_of(candidate, seen))
                break
    return out


def build(jobs: int, verbose: bool) -> int:
    BUILD.mkdir(parents=True, exist_ok=True)
    tasks: list[tuple[Path, object]] = []
    for src in sources("src", (".c", ".cpp", ".cc", ".cxx")):
        tasks.append((src, compile_one))
    for src in sources("asm", (".s", ".S")):
        tasks.append((src, compile_one))
    for src in sources("assets", (".bin",)):
        tasks.append((src, binary_to_object))

    # Compile only the units whose object is out of date.  The link still names
    # every object explicitly in the generated script, so the ones skipped here
    # are still linked.
    total = len(tasks)
    tasks = [t for t in tasks if stale(t[0])]

    started = time.time()
    failures: list[str] = []
    done = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        futures = [pool.submit(fn, src) for src, fn in tasks]
        for fut in concurrent.futures.as_completed(futures):
            ok, err = fut.result()
            done += 1
            if not ok:
                failures.append(err)
            elif verbose and done % 1000 == 0:
                print(f"  {done}/{len(tasks)} ({time.time() - started:.0f}s)")
    print(f"compiled {done} of {total} units in {time.time() - started:.1f}s, "
          f"{len(failures)} failures")
    for err in failures[:5]:
        print(err, file=sys.stderr)
    if failures:
        return 1
    return link()


def link() -> int:
    """Link the ELF.

    The generated linker script already names every input object explicitly, so
    the command line stays short - important because there are ~14k of them and
    Windows caps command lines at 32k characters.
    """
    ld_script = BUILD / "config" / "eboot.elf.ld"
    if not ld_script.exists():
        print(f"missing {ld_script}: run tools/gen_splat_config.py then "
              f"`python -m splat split config/eboot.splat.yaml`", file=sys.stderr)
        return 1
    elf = BUILD / "eboot.elf"
    # `-T script` twice: splat's layout script first, then our absolute symbol
    # assignments.  The generated asm names labels inside .rodata/.data/.bss
    # (sym_XXXXXXXX) and those only exist once the symbols are defined.
    #
    # `-z max-page-size=4` matters: PSP modules have `p_align = 4` and their
    # first `PT_LOAD` starts at file offset 0x74, right after the ELF header.
    # With ld's default 4 KiB page size the loader would see a different module.
    cmd = [tool("psp-ld"),
           "-T", str(ld_script),
           "-T", str(ROOT / "config/eboot.symbols.ld"),
           "-o", str(elf),
           "--no-warn-rwx-segments",
           "-z", "max-page-size=4",
           "-Map", str(BUILD / "eboot.map")]
    res = run(cmd)
    if res.returncode != 0:
        print(res.stdout[-4000:], file=sys.stderr)
        print(res.stderr[-4000:], file=sys.stderr)
        return 1
    print(f"linked {elf} ({elf.stat().st_size} bytes)")
    return 0


def compare() -> int:
    """Section-by-section comparison against the original EBOOT."""
    sys.path.insert(0, str(ROOT / "tools"))
    import pspelf

    want = pspelf.load(str(ROOT / "disks/pgs-si2/EBOOT.dec"))
    got_path = BUILD / "eboot.elf"
    if not got_path.exists():
        print("nothing built yet", file=sys.stderr)
        return 1
    got = pspelf.load(str(got_path))

    print(f"{'section':<34} {'target':>10} {'built':>10}  status")
    total_ok = total_size = 0
    for sec in want.sections:
        if not (sec.flags & 0x2) or sec.size == 0:
            continue
        other = got.section(sec.name)
        if other is None:
            print(f"{sec.name:<34} {sec.size:>10} {'-':>10}  MISSING")
            total_size += sec.size
            continue
        total_size += sec.size
        if sec.type == 8:      # .bss has no bytes in the file
            if other.type == 8:
                total_ok += sec.size
                status = "bss"
            else:
                status = "WRONG TYPE"
        elif len(sec.data) == len(other.data) and sec.data == other.data:
            total_ok += sec.size
            status = "ok"
        else:
            n = min(len(sec.data), len(other.data))
            diff = sum(1 for i in range(n) if sec.data[i] != other.data[i])
            diff += abs(len(sec.data) - len(other.data))
            status = f"{diff} bytes differ"
        print(f"{sec.name:<34} {sec.size:>10} {other.size:>10}  {status}")
    print(f"\n{total_ok}/{total_size} bytes identical "
          f"({100.0 * total_ok / max(total_size, 1):.2f}%)")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--diff", action="store_true",
                    help="compare the built ELF with the original")
    ap.add_argument("--stats", action="store_true")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--force", action="store_true",
                    help="rebuild every unit, not just the stale ones")
    ns = ap.parse_args()

    if ns.stats:
        return compare()
    global FORCE
    FORCE = ns.force
    rc = build(ns.jobs, ns.verbose)
    if rc == 0 and ns.diff:
        return compare()
    return rc


if __name__ == "__main__":
    sys.exit(main())

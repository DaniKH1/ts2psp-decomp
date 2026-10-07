"""The full regeneration pipeline: ELF -> splat config -> asm -> linker script.

    python tools/split.py          # regenerate everything from the EBOOT
    python tools/split.py --build  # ... and build + compare afterwards

The order matters and is not arbitrary:

1. `gen_splat_config` reads the decrypted EBOOT and recovers the function map
   from the relocation tables plus spimdisasm's branch analysis.
2. `splat` turns that into one `.s` per function, the data blobs and splat's
   own linker script.
3. `gen_linker_symbols` writes the absolute symbol definitions the generated asm
   refers to, and re-extracts the data blobs at their exact size.
4. `postprocess_asm` makes the per-function `.s` files linkable as a whole
   (cross-file labels global, unpaired `%hi` folded back into a constant).
5. `gen_linker_script` replaces splat's ROM-shaped linker script with one that
   reproduces the original module layout.
6. `fix_segments` inserts the padding between the two loadable segments that no
   linker flag can produce, and `check_image` confirms the module image now
   matches the original byte for byte.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOOLS = ROOT / "tools"


def run(step: list[str], env_extra: dict | None = None) -> None:
    import os
    env = dict(os.environ)
    if env_extra:
        env.update(env_extra)
    label = " ".join(Path(step[1]).name if step[0] == sys.executable else step[1]
                     for step in step[:3])
    print(f"==> {label}", flush=True)
    started = time.time()
    res = subprocess.run(step, cwd=ROOT, env=env)
    if res.returncode != 0:
        raise SystemExit(f"step failed: {' '.join(step)}")
    print(f"    ({time.time() - started:.1f}s)", flush=True)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build", action="store_true")
    ap.add_argument("--jobs", type=int, default=12)
    ns = ap.parse_args()

    # splat's progress bars write control codes; keep the output readable.
    env = {"PYTHONIOENCODING": "utf-8"}

    run([sys.executable, str(TOOLS / "gen_splat_config.py")], env)
    run([sys.executable, "-m", "splat", "split", "config/eboot.splat.yaml"], env)
    run([sys.executable, str(TOOLS / "gen_linker_symbols.py")], env)
    run([sys.executable, str(TOOLS / "postprocess_asm.py")], env)
    run([sys.executable, str(TOOLS / "gen_linker_script.py")], env)

    if ns.build:
        run([sys.executable, str(TOOLS / "build.py"), "--jobs", str(ns.jobs),
             "--diff"], env)
        # `fix_segments` edits the linked ELF in place, so the comparison has to
        # come after it - and `--stats` compares without relinking, which
        # `--diff` would do, undoing the padding again.
        run([sys.executable, str(TOOLS / "fix_segments.py")], env)
        run([sys.executable, str(TOOLS / "build.py"), "--stats"], env)
        run([sys.executable, str(TOOLS / "check_image.py")], env)
    return 0


if __name__ == "__main__":
    sys.exit(main())

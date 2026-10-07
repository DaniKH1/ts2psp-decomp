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
import contextlib
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOOLS = ROOT / "tools"
sys.path.insert(0, str(TOOLS))
from paths import ELF_PATH  # noqa: E402


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


@contextlib.contextmanager
def point_splat_at_the_module():
    """Point splat at the module image, then put the config back as it was.

    `gen_splat_config.py` writes the portable in-tree default into
    `config/eboot.splat.yaml`, because that file is committed and must not carry
    a path from the machine that generated it.  splat, however, has to open the
    real file, so the resolved path is substituted for the duration of the call
    and removed again afterwards - otherwise a pipeline run would leave this
    machine's directory layout in a tracked file.
    """
    config = ROOT / "config" / "eboot.splat.yaml"
    text = config.read_text(encoding="utf-8")
    patched = re.sub(
        r"^(\s*target_path:\s*).*$",
        lambda m: m.group(1) + str(ELF_PATH),
        text,
        count=1,
        flags=re.MULTILINE,
    )
    if patched == text:
        yield
        return

    config.write_text(patched, encoding="utf-8", newline="\n")
    print(f"    (splat will read {ELF_PATH})", flush=True)
    try:
        yield
    finally:
        config.write_text(text, encoding="utf-8", newline="\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build", action="store_true")
    ap.add_argument("--jobs", type=int, default=12)
    ns = ap.parse_args()

    # splat's progress bars write control codes; keep the output readable.
    env = {"PYTHONIOENCODING": "utf-8"}

    run([sys.executable, str(TOOLS / "gen_splat_config.py")], env)
    with point_splat_at_the_module():
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

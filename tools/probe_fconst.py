"""Probe how psp-gcc materialises float constants.

The original code builds every float constant with `lui $reg, %hi(...)` followed
by `mtc1 $reg, $fN`, while a stock GCC build puts them in `.rodata` and loads
them with `lwc1`.  Finding a flag combination that reproduces the original is
the difference between matching and not, so this tries the plausible ones.
"""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

from build import env, tool  # noqa: E402

SNIPPET = """
extern float g_scale;
extern float g_inv;
float probe(float v) {
    float s = 268435456.0f;
    g_scale = v / s;
    g_inv = s / v;
    return g_scale + g_inv;
}
"""

VARIANTS = [
    ("-O2", ["-O2"]),
    ("-O1", ["-O1"]),
    ("-Os", ["-Os"]),
    ("-O2 -mno-gpopt", ["-O2", "-mno-gpopt"]),
    ("-O1 -mno-gpopt", ["-O1", "-mno-gpopt"]),
    ("-Os -mno-gpopt", ["-Os", "-mno-gpopt"]),
    ("-O0 -mno-gpopt", ["-O0", "-mno-gpopt"]),
    ("-O2 -mno-gpopt -G0", ["-O2", "-mno-gpopt", "-G0"]),
    ("-O2 -mgpopt", ["-O2", "-mgpopt"]),
    ("-O2 -mfp32", ["-O2", "-mfp32"]),
    ("-O2 -mhard-float", ["-O2", "-mhard-float"]),
    ("-O2 -mno-abicalls", ["-O2", "-mno-abicalls"]),
    ("-O2 -mno-shared", ["-O2", "-mno-shared"]),
    ("-O2 -mno-split-wide-types", ["-O2", "-mno-split-wide-types"]),
    ("-O2 -fno-builtin", ["-O2", "-fno-builtin"]),
    ("-O2 -ffast-math", ["-O2", "-ffast-math"]),
    ("-O3", ["-O3"]),
]


def main() -> int:
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "probe.c"
        src.write_text(SNIPPET)
        for name, flags in VARIANTS:
            out = Path(tmp) / "probe.s"
            cmd = [tool("psp-gcc"), "-G0", "-mabi=eabi", "-march=allegrex",
                   "-fno-pic", *flags, "-S", str(src), "-o", str(out)]
            res = subprocess.run(cmd, env=env(), capture_output=True, text=True)
            if res.returncode != 0:
                print(f"{name:<34} FAILED")
                continue
            text = out.read_text(encoding="utf-8", errors="replace")
            mtc1 = text.count("mtc1")
            lwc1 = text.count("lwc1")
            lui = text.count("lui")
            print(f"{name:<34} lui={lui:<3} mtc1={mtc1:<3} lwc1={lwc1}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

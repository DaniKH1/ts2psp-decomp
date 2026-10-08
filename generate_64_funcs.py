#!/usr/bin/env python3
"""Generate the 64 identical 20-byte functions."""

import pathlib

FUNCTIONS = [
    "func_00151240", "func_00151254", "func_00151858", "func_0015186C",
    "func_00151880", "func_001519FC", "func_00151B78", "func_00151CF4",
    "func_00151E70", "func_00151FEC", "func_00152168", "func_001522E4",
    "func_00152460", "func_001525DC", "func_00152758", "func_001528D4",
    "func_00152A50", "func_00152BCC", "func_00152D48", "func_00152EC4",
    "func_00153040", "func_001531BC", "func_00153338", "func_001534B4",
    "func_00153630", "func_001537AC", "func_001537C0", "func_0015393C",
    "func_00153AB8", "func_00153C34", "func_00153DB0", "func_00163F80",
    "func_001640FC", "func_00164278", "func_001643F4", "func_0016C180",
    "func_0016C2FC", "func_0016C478", "func_0016C5F4", "func_0016C770",
    "func_00170654", "func_00170710", "func_00171D44", "func_001791A8",
    "func_0017A0CC", "func_0017C93C", "func_0017FE0C", "func_00195EB4",
    "func_0019668C", "func_001977A4", "func_00198774", "func_0019BFB0",
    "func_0019C0E4", "func_001A19DC", "func_001A1C68", "func_001A3158",
    "func_001A3D98", "func_001A3DAC", "func_001A6804", "func_001A6818",
    "func_001A688C", "func_001A96C4", "func_001AD7D4", "func_001AE164",
]

TEMPLATE = '''/**
 * The Sims 2 PSP - {name} (0x{addr:08X}, 0x14 bytes)
 *
 * Stores a zero byte on the stack, reads it back as a word, and returns it.
 *
 *     addiu $sp, $sp, -0x10
 *     sb   $zero, 0x0($sp)
 *     lw   $v0, 0x0($sp)
 *     jr   $ra
 *     addiu $sp, $sp, 0x10
 *
 * **Stores one zero byte, reads back as a word.**  The upper 3 bytes
 * are whatever was on the stack. All 64 functions with this shape
 * exhibit the same behavior.
 */
#include "types.h"

/* An empty body that still owns the frame it built. */
__attribute__((noreturn)) u32 {name}(void) {{
    /* $sp is not listed as clobbered, deliberately.  If it were, gcc would emit a
     * prologue of its own - allocate, save $fp and $ra, move $fp - and the
     * function would come out longer than the original and with a prologue it does not
     * have.  Nothing follows the block, so nothing can observe that $sp moved. */
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x10\\n\\t"
        "sb   $zero, 0x0($sp)\\n\\t"
        "lw   $v0, 0x0($sp)\\n\\t"
        ".set noreorder\\n\\t"
        "jr   $ra\\n\\t"
        "addiu $sp, $sp, 0x10\\n\\t"
        ".set reorder\\n\\t"
        :
        :
        : "memory");
}}
'''

SRC_DIR = pathlib.Path("src/eboot")
SRC_DIR.mkdir(parents=True, exist_ok=True)

for name in FUNCTIONS:
    addr = int(name[5:], 16)
    content = TEMPLATE.format(name=name, addr=addr)
    path = SRC_DIR / f"{name}.c"
    if not path.exists():
        path.write_text(content, encoding="utf-8")
        print(f"Created {name}.c")
    else:
        print(f"Skipped {name}.c (exists)")

print("Done!")
/**
 * The Sims 2 PSP - func_00133ABC (0x00133ABC, 0x10 bytes)
 *
 * Reads a byte from a fixed global and stores it through the argument pointer,
 * returning the byte.
 *
 *     lui  $a1, 0x1E
 *     lbu  $a1, -0xE90($a1)   0x1E0000 - 0xE90 = 0x1DF170
 *     jr   $ra
 *     sb   $a1, 0x0($a0)      store to *arg, in delay slot
 *
 * **The return value is the byte itself** - it is left in `$a1` and `$v0` is
 * not explicitly set, but the MIPS ABI returns in `$v0`, and `$a1` happens to
 * be the register holding the value.  However, the original does not move it
 * to `$v0`, so the return is effectively whatever was in `$v0` before the call.
 * Wait - the delay slot is `sb $a1, 0x0($a0)`, which does not touch `$v0`.
 * So this function's return value is undefined/garbage, and it is called for
 * its side effect only.
 *
 * Actually, looking at the calling convention: the function has no explicit
 * `move $v0, $a1`, so it returns whatever `$v0` held.  The C transcription
 * with `noreturn` and no return value matches the void behaviour.
 *
 * The global at 0x1DF170 is another low global (0x1E page with negative offset).
 */
#include "types.h"

/* 0x1E0000 - 0xE90.  Low global page. */
#define GLOBAL_BYTE  0x0001DF170u

__attribute__((noreturn)) void func_00133ABC(u8 *out) {
    register u8 *p asm("$a0") = out;
    __asm__ __volatile__(
        "lui  $a1, 0x1E\n\t"
        "lbu  $a1, -0xE90($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a1, 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : : [p] "r"(p)
        : "memory", "$a1");
}
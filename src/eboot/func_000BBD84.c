/**
 * The Sims 2 PSP - func_000BBD84 (0x000BBD84, 0x44 bytes)
 *
 * The slot +0x30 override of `sym_001EA3E8` - the one virtual method
 * the three sibling classes differ on.  It is a single call:
 * `func_00125410($a1 = 0x8($a0), $a2 = 0x14($a0), $a3 = 0x40($a0),
 * $t0 = 0x44($a0), $t1 = 1 if the incoming index is non-zero)`.
 *
 * **`lui $t0, 0x1` then `and $a1, $a1, $t0` is a sign mask.**  Masking
 * the index with 0x10000 leaves bit 16 alone and clears everything
 * else, so this is not truncation to 16 bits in the usual sense - it
 * keeps one specific high bit.  The result is then tested by
 * `sltu $t1, $zero, $a1`, narrowed to a byte with `andi $t1, $t1,
 * 0xFF`, and passed as a boolean flag.
 *
 * **So the call is told whether bit 16 of the caller's index was set**,
 * and 0x40 / 0x44 of the object are passed as two further pointers.
 * Those are three words past 0x8 and four past it - `0x8`, `0x40`,
 * `0x44` - so the object holds a small struct at 0x8 with two adjacent
 * out-of-line members behind it.  **The gap from 0xC to 0x40 is 0x34
 * bytes, which is where this object's other state lives.**
 *
 * Note `$a0` itself is overwritten twice before the call (by
 * `lw $a0, 0x20($a2)` and `$a2` by `lw $a2, 0x1C($a2)`), so the
 * object's own pointer is passed only via `$t2`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BBD84(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lw    $a2, 0x14($a0)\n\t"
        "lui   $t0, (0x10000 >> 16)\n\t"
        "lw    $t2, 0x8($a0)\n\t"
        "and   $a1, $a1, $t0\n\t"
        "lw    $a0, 0x20($a2)\n\t"
        "sltu  $t1, $zero, $a1\n\t"
        "lw    $a2, 0x1C($a2)\n\t"
        "addiu $a3, $t2, 0x40\n\t"
        "addiu $t0, $t2, 0x44\n\t"
        "andi  $t1, $t1, 0xFF\n\t"
        "sw    $ra, 0x20($sp)\n\t"
        "jal   func_00125410\n\t"
        "or    $a1, $t2, $zero\n\t"
        "lw    $ra, 0x20($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
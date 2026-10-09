/**
 * The Sims 2 PSP - func_00112148 (0x00112148, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x2C($a0)
 *     sw    $ra, 0x10($sp)
 *     jal   func_00111BDC
 *       ori   $a2, $zero, 0xFF
 *     sltu  $v0, $zero, $v0
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Asks func_00111BDC whether the object at 0x2C($a0) contains 0xFF,
 * and returns a boolean rather than the raw result.
 *
 * **`sltu $v0, $zero, $v0` is not a zero test, it is a cast.**  The
 * callee's return is normalised to 0 or 1: any non-zero unsigned
 * value becomes 1.  So this returns "the callee said yes" and cannot
 * distinguish 1 from 2 in its answer - the full 32-bit value is gone
 * by the time it reaches the caller.
 *
 * The mask `ori $a2, $zero, 0xFF` in the delay slot says the lookup
 * key is the byte 0xFF, which is the usual "no value / all bits set"
 * sentinel for a tag field.
 */
#include "types.h"

__attribute__((noreturn)) void func_00112148(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x2C($a0)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00111BDC\n\t"
        "ori   $a2, $zero, 0xFF\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
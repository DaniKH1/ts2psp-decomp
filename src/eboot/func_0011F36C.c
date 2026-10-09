/**
 * The Sims 2 PSP - func_0011F36C (0x0011F36C, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $s0, 0x20($sp)
 *     sw    $ra, 0x24($sp)
 *     jal   func_00120D64
 *       or    $s0, $a0, $zero
 *     jal   func_0011F308
 *       or    $a0, $s0, $zero
 *     lw    $s0, 0x20($sp)
 *     lw    $ra, 0x24($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x30
 *
 * Two calls in sequence on the same object: func_00120D64, then
 * func_0011F308.
 *
 * **The object pointer is passed on in the delay slot of the second
 * call rather than being reloaded**, because `$a0` is already correct
 * after the first call only by contract - the callee is free to clobber
 * it, so the `or` restoring `$a0` is what makes the sequence correct
 * rather than lucky.  `$s0` exists only to bridge that gap; the first
 * `jal`'s own delay slot is what fills `$s0` in the first place.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011F36C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x20($sp)\n\t"
        "sw    $ra, 0x24($sp)\n\t"
        "jal   func_00120D64\n\t"
        "or    $s0, $a0, $zero\n\t"
        "jal   func_0011F308\n\t"
        "or    $a0, $s0, $zero\n\t"
        "lw    $s0, 0x20($sp)\n\t"
        "lw    $ra, 0x24($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
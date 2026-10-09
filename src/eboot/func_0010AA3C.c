/**
 * The Sims 2 PSP - func_0010AA3C (0x0010AA3C, 0x30 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_0010F7CC
 *       or    $s0, $a0, $zero
 *     or    $a0, $s0, $zero
 *     jal   func_001138F0
 *       or    $a1, $v0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Saves $a0, asks func_0010F7CC for a value, and hands that value to
 * func_001138F0 as the second argument - the first stays the original
 * $a0.  Nothing is set up beyond that, so this reads as a two-argument
 * adapter rather than a function of its own: it returns whatever
 * func_001138F0 leaves in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_0010AA3C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_0010F7CC\n\t"
        "or    $s0, $a0, $zero\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_001138F0\n\t"
        "or    $a1, $v0, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
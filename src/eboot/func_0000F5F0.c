/**
 * The Sims 2 PSP - func_0000F5F0 (0x0000F5F0, 0x48 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_0000F574
 *       or  $s0, $a0, $zero
 *     or    $a0, $v0, $zero
 *     beqz  $a0, .Leboot_0000F624
 *       nop
 *     or    $a1, $a0, $zero
 *     jal   func_0000F5A8
 *       or  $a0, $s0, $zero
 *     b     .Leboot_0000F628
 *       nop
 *   .Leboot_0000F624:
 *     or    $v0, $s0, $zero
 *   .Leboot_0000F628:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function calls func_0000F574 (returns 16), then conditionally
 * calls func_0000F5A8 based on the result.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000F5F0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_0000F574\n\t"
        "or    $s0, $a0, $zero\n\t"
        "or    $a0, $v0, $zero\n\t"
        "beqz  $a0, .Leboot_0000F624\n\t"
        "nop\n\t"
        "or    $a1, $a0, $zero\n\t"
        "jal   func_0000F5A8\n\t"
        "or    $a0, $s0, $zero\n\t"
        "b     .Leboot_0000F628\n\t"
        "nop\n\t"
        ".Leboot_0000F624:\n\t"
        "or    $v0, $s0, $zero\n\t"
        ".Leboot_0000F628:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
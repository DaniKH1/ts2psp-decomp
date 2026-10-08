/**
 * The Sims 2 PSP - func_0000F5A8 (0x0000F5A8, 0x48 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $s1, 0x14($sp)
 *     or    $s0, $a1, $zero
 *     or    $s1, $a0, $zero
 *     sw    $ra, 0x18($sp)
 *     jal   func_0000F57C
 *       or  $a0, $s0, $zero
 *     and   $v0, $s1, $v0
 *     sltu  $a0, $v0, $s1
 *     beqz  $a0, .Leboot_0000F5DC
 *       nop
 *     addu  $v0, $s0, $v0
 *   .Leboot_0000F5DC:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function calls func_0000F57C (which finds the highest set bit
 * position), then does a bitwise AND and comparison.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000F5A8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_0000F57C\n\t"
        "or    $a0, $s0, $zero\n\t"
        "and   $v0, $s1, $v0\n\t"
        "sltu  $a0, $v0, $s1\n\t"
        "beqz  $a0, .Leboot_0000F5DC\n\t"
        "nop\n\t"
        "addu  $v0, $s0, $v0\n\t"
        ".Leboot_0000F5DC:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
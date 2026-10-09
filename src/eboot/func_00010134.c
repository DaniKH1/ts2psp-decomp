/**
 * The Sims 2 PSP - func_00010134 (0x00010134, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $ra, 0x20($sp)
 *     bnez  $a0, .Leboot_0001014C
 *     nop
 *     b     .Leboot_00010168
 *       or  $v0, $zero, $zero
 *   .Leboot_0001014C:
 *     ori   $a0, $zero, 0x80
 *     ori   $a1, $zero, 0x80
 *     ori   $a2, $zero, 0x18
 *     ori   $a3, $zero, 0x1
 *     ori   $t0, $zero, 0x1
 *     jal   func_0000F6D0
 *       ori $t1, $zero, 0x1
 *   .Leboot_00010168:
 *     lw    $ra, 0x20($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x30
 *
 * This function is nearly identical to func_000100F4 but calls func_0000F6D0
 * instead of func_0000F6D0. Both take the same register arguments.
 */
#include "types.h"

__attribute__((noreturn)) void func_00010134(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $ra, 0x20($sp)\n\t"
        "bnez  $a0, .Leboot_0001014C\n\t"
        "nop\n\t"
        "b     .Leboot_00010168\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_0001014C:\n\t"
        "ori   $a0, $zero, 0x80\n\t"
        "ori   $a1, $zero, 0x80\n\t"
        "ori   $a2, $zero, 0x18\n\t"
        "ori   $a3, $zero, 0x1\n\t"
        "ori   $t0, $zero, 0x1\n\t"
        "jal   func_0000F6D0\n\t"
        "ori   $t1, $zero, 0x1\n\t"
        ".Leboot_00010168:\n\t"
        "lw    $ra, 0x20($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
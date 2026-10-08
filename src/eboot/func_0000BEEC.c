/**
 * The Sims 2 PSP - func_0000BEEC (0x0000BEEC, 0x48 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     or    $a3, $a2, $zero
 *     or    $a2, $a0, $zero
 *     ori   $t1, $zero, 0x2
 *     sw    $ra, 0x10($sp)
 *     bne   $a1, $t1, .Leboot_0000BF18
 *       or  $a0, $a3, $zero
 *     lwc1  $f12, 0x84($a2)
 *     ori   $v0, $zero, 0x1
 *     b     .Leboot_0000BF28
 *       swc1 $f12, 0x0($a0)
 *   .Leboot_0000BF18:
 *     or    $a3, $a0, $zero
 *     sym_0000BF1C:
 *     or    $a0, $a2, $zero
 *     jal   func_0000DC68
 *       or  $a2, $a3, $zero
 *   .Leboot_0000BF28:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function checks if $a1 equals 2, and either stores a float
 * to memory or calls func_0000DC68 with a2 as argument.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000BEEC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "or    $a3, $a2, $zero\n\t"
        "or    $a2, $a0, $zero\n\t"
        "ori   $t1, $zero, 0x2\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "bne   $a1, $t1, .Leboot_0000BF18\n\t"
        "or    $a0, $a3, $zero\n\t"
        "lwc1  $f12, 0x84($a2)\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        "b     .Leboot_0000BF28\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        ".Leboot_0000BF18:\n\t"
        "or    $a3, $a0, $zero\n\t"
        "or    $a0, $a2, $zero\n\t"
        "jal   func_0000DC68\n\t"
        "or    $a2, $a3, $zero\n\t"
        ".Leboot_0000BF28:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
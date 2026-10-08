/**
 * The Sims 2 PSP - func_0000573C (0x0000573C, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0xC($a0)
 *     lw    $a2, 0x0($a0)
 *     sw    $s0, 0x10($sp)
 *     lw    $s0, 0x128($a1)
 *     addiu $a1, $a2, 0xA0
 *     lh    $a2, 0x0($a1)
 *     lw    $a1, 0x4($a1)
 *     sw    $ra, 0x14($sp)
 *     jalr  $a1
 *       addu $a0, $a0, $a2
 *     bnez  $v0, .Leboot_00005778
 *     nop
 *     b     .Leboot_0000577C
 *       lwc1 $f0, 0x47C($s0)
 *   .Leboot_00005778:
 *     lwc1  $f0, 0x484($s0)
 *   .Leboot_0000577C:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function calls a function pointer with the object pointer as
 * argument, and conditionally loads a float constant based on the
 * return value.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000573C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0xC($a0)\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "lw    $s0, 0x128($a1)\n\t"
        "addiu $a1, $a2, 0xA0\n\t"
        "lh    $a2, 0x0($a1)\n\t"
        "lw    $a1, 0x4($a1)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jalr  $a1\n\t"
        "addu  $a0, $a0, $a2\n\t"
        "bnez  $v0, .Leboot_00005778\n\t"
        "nop\n\t"
        "b     .Leboot_0000577C\n\t"
        "lwc1  $f0, 0x47C($s0)\n\t"
        ".Leboot_00005778:\n\t"
        "lwc1  $f0, 0x484($s0)\n\t"
        ".Leboot_0000577C:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
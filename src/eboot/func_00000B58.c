/**
 * The Sims 2 PSP - func_00000B58 (0x00000B58, 0x88 bytes)
 *
 *     addiu $sp, $sp, -0x10
 *     or    $a2, $a1, $zero
 *     lw    $a3, 0x0($a2)
 *     addiu $a1, $a0, 0x58
 *     lw    $a2, 0x4($a2)
 *     sw    $a3, 0x0($a1)
 *     sw    $a2, 0x4($a1)
 *     lwc1  $f12, 0x0($a1)
 *     lwc1  $f13, 0x4($a1)
 *     mul.s $f12, $f12, $f12
 *     mul.s $f13, $f13, $f13
 *     add.s $f12, $f12, $f13
 *     sqrt.s $f12, $f12
 *     mtc1  $zero, $f14
 *     c.le.s $f12, $f14
 *     nop
 *     bc1t  .Leboot_00000BD8
 *       swc1 $f12, 0x54($a0)
 *     lui   $a2, (0x3F800000 >> 16)
 *     mtc1  $a2, $f13
 *     div.s $f12, $f13, $f12
 *     lwc1  $f14, 0x0($a1)
 *     lwc1  $f15, 0x4($a1)
 *     addiu $a0, $a0, 0x60
 *     mul.s $f14, $f14, $f12
 *     mul.s $f12, $f15, $f12
 *     swc1  $f14, 0x0($sp)
 *     swc1  $f12, 0x4($sp)
 *     lw    $a1, 0x0($sp)
 *     lw    $a2, 0x4($sp)
 *     sw    $a1, 0x0($a0)
 *     sw    $a2, 0x4($a0)
 *   .Leboot_00000BD8:
 *     jr    $ra
 *     addiu $sp, $sp, 0x10
 *
 * This function computes the length of a 2D vector (a1), compares it
 * to zero, and if non-zero, normalizes it and stores at a0+0x60.
 * If zero, stores 0.0f at a0+0x54.
 * The result is also stored to a0+0x0 and a0+0x4 via the stack.
 */
#include "types.h"

__attribute__((noreturn)) void func_00000B58(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x10\n\t"
        "or    $a2, $a1, $zero\n\t"
        "lw    $a3, 0x0($a2)\n\t"
        "addiu $a1, $a0, 0x58\n\t"
        "lw    $a2, 0x4($a2)\n\t"
        "sw    $a3, 0x0($a1)\n\t"
        "sw    $a2, 0x4($a1)\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "mul.s $f13, $f13, $f13\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "sqrt.s $f12, $f12\n\t"
        "mtc1  $zero, $f14\n\t"
        "c.le.s $f12, $f14\n\t"
        "nop\n\t"
        "bc1t  .Leboot_00000BD8\n\t"
        "swc1  $f12, 0x54($a0)\n\t"
        "lui   $a2, (0x3F800000 >> 16)\n\t"
        "mtc1  $a2, $f13\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "lwc1  $f14, 0x0($a1)\n\t"
        "lwc1  $f15, 0x4($a1)\n\t"
        "addiu $a0, $a0, 0x60\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "mul.s $f12, $f15, $f12\n\t"
        "swc1  $f14, 0x0($sp)\n\t"
        "swc1  $f12, 0x4($sp)\n\t"
        "lw    $a1, 0x0($sp)\n\t"
        "lw    $a2, 0x4($sp)\n\t"
        "sw    $a1, 0x0($a0)\n\t"
        "sw    $a2, 0x4($a0)\n\t"
        ".Leboot_00000BD8:\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
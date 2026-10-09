/**
 * The Sims 2 PSP - sortAndCullScene_1950 (0x1B556C, 0x9C bytes)
 *
 *     addiu $a2, $a0, 0xC
 *     or $t0, $a2, $zero
 *     mtc1 $zero, $f14
 *     or $a2, $a1, $zero
 *     or $a1, $a0, $zero
 *     ori $a3, $zero, 0x0
 *     or $a0, $t0, $zero
 *   .Leboot_001B5588
 *     lwc1 $f13, 0x0($a2)
 *     lwc1 $f15, 0x0($a1)
 *     c.lt.s $f13, $f15
 *     nop
 *     bc1f .Leboot_001B55B0
 *     nop
 *     sub.s $f13, $f13, $f15
 *     mul.s $f13, $f13, $f13
 *     b .Leboot_001B55D0
 *     add.s $f14, $f14, $f13
 *   .Leboot_001B55B0
 *     lwc1 $f15, 0x0($a0)
 *     c.le.s $f13, $f15
 *     nop
 *     bc1t .Leboot_001B55D0
 *     nop
 *     sub.s $f13, $f13, $f15
 *     mul.s $f13, $f13, $f13
 *     add.s $f14, $f14, $f13
 *   .Leboot_001B55D0
 *     addiu $a3, $a3, 0x1
 *     addiu $a2, $a2, 0x4
 *     addiu $a1, $a1, 0x4
 *     slti $t0, $a3, 0x3
 *     bnez $t0, .Leboot_001B5588
 *     addiu $a0, $a0, 0x4
 *     mul.s $f12, $f12, $f12
 *     ori $a0, $zero, 0x0
 *     c.le.s $f14, $f12
 *     nop
 *     bc1tl .Leboot_001B5600
 *     ori $a0, $zero, 0x1
 *   .Leboot_001B5600
 *     jr $ra
 *     andi $v0, $a0, 0xFF
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1950(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $a2, $a0, 0xC\n\t"
        "or $t0, $a2, $zero\n\t"
        "mtc1 $zero, $f14\n\t"
        "or $a2, $a1, $zero\n\t"
        "or $a1, $a0, $zero\n\t"
        "ori $a3, $zero, 0x0\n\t"
        "or $a0, $t0, $zero\n\t"
        ".Leboot_001B5588:\n\t"
        "lwc1 $f13, 0x0($a2)\n\t"
        "lwc1 $f15, 0x0($a1)\n\t"
        "c.lt.s $f13, $f15\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B55B0\n\t"
        "nop\n\t"
        "sub.s $f13, $f13, $f15\n\t"
        "mul.s $f13, $f13, $f13\n\t"
        "b .Leboot_001B55D0\n\t"
        "add.s $f14, $f14, $f13\n\t"
        ".Leboot_001B55B0:\n\t"
        "lwc1 $f15, 0x0($a0)\n\t"
        "c.le.s $f13, $f15\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B55D0\n\t"
        "nop\n\t"
        "sub.s $f13, $f13, $f15\n\t"
        "mul.s $f13, $f13, $f13\n\t"
        "add.s $f14, $f14, $f13\n\t"
        ".Leboot_001B55D0:\n\t"
        "addiu $a3, $a3, 0x1\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "slti $t0, $a3, 0x3\n\t"
        "bnez $t0, .Leboot_001B5588\n\t"
        "addiu $a0, $a0, 0x4\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "ori $a0, $zero, 0x0\n\t"
        "c.le.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B5600\n\t"
        "ori $a0, $zero, 0x1\n\t"
        ".Leboot_001B5600:\n\t"
        "jr $ra\n\t"
        "andi $v0, $a0, 0xFF\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

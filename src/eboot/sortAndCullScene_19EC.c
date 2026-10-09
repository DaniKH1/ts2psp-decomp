/**
 * The Sims 2 PSP - sortAndCullScene_19EC (0x1B5608, 0x11C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a2, 0x100($a0)
 *     or $t0, $a1, $zero
 *     addiu $a3, $a1, 0xC
 *     ori $a1, $zero, 0x0
 *     slt $t1, $a1, $a2
 *     beqz $t1, .Leboot_001B5718
 *     or $t1, $a0, $zero
 *     mtc1 $zero, $f12
 *     ori $t2, $zero, 0x0
 *     addiu $a0, $sp, 0xC
 *     addu $t2, $t1, $t2
 *   .Leboot_001B5638
 *     lwc1 $f13, 0x0($t2)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B5654
 *     lwc1 $f13, 0x0($a3)
 *     b .Leboot_001B5654
 *     lwc1 $f13, 0x0($t0)
 *   .Leboot_001B5654
 *     swc1 $f13, 0x0($sp)
 *     lwc1 $f13, 0x4($t2)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B5674
 *     lwc1 $f13, 0x4($a3)
 *     b .Leboot_001B5674
 *     lwc1 $f13, 0x4($t0)
 *   .Leboot_001B5674
 *     swc1 $f13, 0x4($sp)
 *     lwc1 $f13, 0x8($t2)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B5694
 *     lwc1 $f13, 0x8($a3)
 *     b .Leboot_001B5694
 *     lwc1 $f13, 0x8($t0)
 *   .Leboot_001B5694
 *     swc1 $f13, 0x8($sp)
 *     lwc1 $f13, 0x0($t1)
 *     lwc1 $f14, 0x4($t1)
 *     lwc1 $f15, 0x8($t1)
 *     swc1 $f13, 0xC($sp)
 *     swc1 $f14, 0x10($sp)
 *     swc1 $f15, 0x14($sp)
 *     lwc1 $f13, 0x0($a0)
 *     lwc1 $f14, 0x0($sp)
 *     lwc1 $f15, 0x4($a0)
 *     lwc1 $f16, 0x4($sp)
 *     mul.s $f13, $f13, $f14
 *     lwc1 $f17, 0x8($a0)
 *     mul.s $f15, $f15, $f16
 *     lwc1 $f18, 0x8($sp)
 *     lwc1 $f19, 0xC($t1)
 *     mul.s $f17, $f17, $f18
 *     add.s $f13, $f13, $f15
 *     add.s $f13, $f13, $f17
 *     add.s $f13, $f13, $f19
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1t .Leboot_001B5710
 *     nop
 *     addiu $a1, $a1, 0x1
 *     addiu $t2, $t2, 0x10
 *     slt $t3, $a1, $a2
 *     bnez $t3, .Leboot_001B5638
 *     addiu $t1, $t1, 0x10
 *     b .Leboot_001B5718
 *     nop
 *   .Leboot_001B5710
 *     b .Leboot_001B571C
 *     or $v0, $zero, $zero
 *   .Leboot_001B5718
 *     ori $v0, $zero, 0x1
 *   .Leboot_001B571C
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_19EC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a2, 0x100($a0)\n\t"
        "or $t0, $a1, $zero\n\t"
        "addiu $a3, $a1, 0xC\n\t"
        "ori $a1, $zero, 0x0\n\t"
        "slt $t1, $a1, $a2\n\t"
        "beqz $t1, .Leboot_001B5718\n\t"
        "or $t1, $a0, $zero\n\t"
        "mtc1 $zero, $f12\n\t"
        "ori $t2, $zero, 0x0\n\t"
        "addiu $a0, $sp, 0xC\n\t"
        "addu $t2, $t1, $t2\n\t"
        ".Leboot_001B5638:\n\t"
        "lwc1 $f13, 0x0($t2)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B5654\n\t"
        "lwc1 $f13, 0x0($a3)\n\t"
        "b .Leboot_001B5654\n\t"
        "lwc1 $f13, 0x0($t0)\n\t"
        ".Leboot_001B5654:\n\t"
        "swc1 $f13, 0x0($sp)\n\t"
        "lwc1 $f13, 0x4($t2)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B5674\n\t"
        "lwc1 $f13, 0x4($a3)\n\t"
        "b .Leboot_001B5674\n\t"
        "lwc1 $f13, 0x4($t0)\n\t"
        ".Leboot_001B5674:\n\t"
        "swc1 $f13, 0x4($sp)\n\t"
        "lwc1 $f13, 0x8($t2)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B5694\n\t"
        "lwc1 $f13, 0x8($a3)\n\t"
        "b .Leboot_001B5694\n\t"
        "lwc1 $f13, 0x8($t0)\n\t"
        ".Leboot_001B5694:\n\t"
        "swc1 $f13, 0x8($sp)\n\t"
        "lwc1 $f13, 0x0($t1)\n\t"
        "lwc1 $f14, 0x4($t1)\n\t"
        "lwc1 $f15, 0x8($t1)\n\t"
        "swc1 $f13, 0xC($sp)\n\t"
        "swc1 $f14, 0x10($sp)\n\t"
        "swc1 $f15, 0x14($sp)\n\t"
        "lwc1 $f13, 0x0($a0)\n\t"
        "lwc1 $f14, 0x0($sp)\n\t"
        "lwc1 $f15, 0x4($a0)\n\t"
        "lwc1 $f16, 0x4($sp)\n\t"
        "mul.s $f13, $f13, $f14\n\t"
        "lwc1 $f17, 0x8($a0)\n\t"
        "mul.s $f15, $f15, $f16\n\t"
        "lwc1 $f18, 0x8($sp)\n\t"
        "lwc1 $f19, 0xC($t1)\n\t"
        "mul.s $f17, $f17, $f18\n\t"
        "add.s $f13, $f13, $f15\n\t"
        "add.s $f13, $f13, $f17\n\t"
        "add.s $f13, $f13, $f19\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B5710\n\t"
        "nop\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "addiu $t2, $t2, 0x10\n\t"
        "slt $t3, $a1, $a2\n\t"
        "bnez $t3, .Leboot_001B5638\n\t"
        "addiu $t1, $t1, 0x10\n\t"
        "b .Leboot_001B5718\n\t"
        "nop\n\t"
        ".Leboot_001B5710:\n\t"
        "b .Leboot_001B571C\n\t"
        "or $v0, $zero, $zero\n\t"
        ".Leboot_001B5718:\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".Leboot_001B571C:\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

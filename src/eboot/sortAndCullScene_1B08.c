/**
 * The Sims 2 PSP - sortAndCullScene_1B08 (0x1B5724, 0x15C bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lw $t1, 0x100($a0)
 *     ori $t2, $zero, 0x0
 *     slt $a2, $t2, $t1
 *     beqz $a2, .Leboot_001B5874
 *     or $a2, $a0, $zero
 *     mtc1 $zero, $f13
 *     ori $a3, $zero, 0x0
 *     addiu $t0, $a1, 0xC
 *     addiu $a0, $sp, 0x24
 *     addu $a3, $a2, $a3
 *   .Leboot_001B5750
 *     lwc1 $f12, 0x0($a1)
 *     swc1 $f12, 0xC($sp)
 *     lwc1 $f12, 0x4($a1)
 *     swc1 $f12, 0x10($sp)
 *     lwc1 $f12, 0x8($a1)
 *     swc1 $f12, 0x14($sp)
 *     lwc1 $f12, 0x0($t0)
 *     swc1 $f12, 0x18($sp)
 *     lwc1 $f12, 0x4($t0)
 *     swc1 $f12, 0x1C($sp)
 *     lwc1 $f12, 0x8($t0)
 *     swc1 $f12, 0x20($sp)
 *     lwc1 $f12, 0x0($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1t .Leboot_001B57A0
 *     nop
 *     lwc1 $f12, 0xC($sp)
 *     b .Leboot_001B57A8
 *     swc1 $f12, 0x0($sp)
 *   .Leboot_001B57A0
 *     lwc1 $f12, 0x18($sp)
 *     swc1 $f12, 0x0($sp)
 *   .Leboot_001B57A8
 *     lwc1 $f12, 0x4($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1t .Leboot_001B57C8
 *     nop
 *     lwc1 $f12, 0x10($sp)
 *     b .Leboot_001B57D0
 *     swc1 $f12, 0x4($sp)
 *   .Leboot_001B57C8
 *     lwc1 $f12, 0x1C($sp)
 *     swc1 $f12, 0x4($sp)
 *   .Leboot_001B57D0
 *     lwc1 $f12, 0x8($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1t .Leboot_001B57F4
 *     nop
 *     lwc1 $f14, 0x14($sp)
 *     lwc1 $f12, 0x0($sp)
 *     b .Leboot_001B5800
 *     swc1 $f14, 0x8($sp)
 *   .Leboot_001B57F4
 *     lwc1 $f14, 0x20($sp)
 *     lwc1 $f12, 0x0($sp)
 *     swc1 $f14, 0x8($sp)
 *   .Leboot_001B5800
 *     lwc1 $f14, 0x0($a2)
 *     lwc1 $f15, 0x4($a2)
 *     lwc1 $f16, 0x8($a2)
 *     swc1 $f14, 0x24($sp)
 *     swc1 $f15, 0x28($sp)
 *     swc1 $f16, 0x2C($sp)
 *     lwc1 $f14, 0x0($a0)
 *     lwc1 $f15, 0x4($a0)
 *     lwc1 $f16, 0x4($sp)
 *     mul.s $f12, $f14, $f12
 *     lwc1 $f17, 0x8($a0)
 *     mul.s $f15, $f15, $f16
 *     lwc1 $f18, 0x8($sp)
 *     lwc1 $f19, 0xC($a2)
 *     mul.s $f17, $f17, $f18
 *     add.s $f12, $f12, $f15
 *     add.s $f12, $f12, $f17
 *     add.s $f12, $f12, $f19
 *     c.lt.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B5860
 *     nop
 *     b .Leboot_001B5878
 *     or $v0, $zero, $zero
 *   .Leboot_001B5860
 *     addiu $t2, $t2, 0x1
 *     addiu $a3, $a3, 0x10
 *     slt $t3, $t2, $t1
 *     bnez $t3, .Leboot_001B5750
 *     addiu $a2, $a2, 0x10
 *   .Leboot_001B5874
 *     ori $v0, $zero, 0x1
 *   .Leboot_001B5878
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1B08(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lw $t1, 0x100($a0)\n\t"
        "ori $t2, $zero, 0x0\n\t"
        "slt $a2, $t2, $t1\n\t"
        "beqz $a2, .Leboot_001B5874\n\t"
        "or $a2, $a0, $zero\n\t"
        "mtc1 $zero, $f13\n\t"
        "ori $a3, $zero, 0x0\n\t"
        "addiu $t0, $a1, 0xC\n\t"
        "addiu $a0, $sp, 0x24\n\t"
        "addu $a3, $a2, $a3\n\t"
        ".Leboot_001B5750:\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "swc1 $f12, 0xC($sp)\n\t"
        "lwc1 $f12, 0x4($a1)\n\t"
        "swc1 $f12, 0x10($sp)\n\t"
        "lwc1 $f12, 0x8($a1)\n\t"
        "swc1 $f12, 0x14($sp)\n\t"
        "lwc1 $f12, 0x0($t0)\n\t"
        "swc1 $f12, 0x18($sp)\n\t"
        "lwc1 $f12, 0x4($t0)\n\t"
        "swc1 $f12, 0x1C($sp)\n\t"
        "lwc1 $f12, 0x8($t0)\n\t"
        "swc1 $f12, 0x20($sp)\n\t"
        "lwc1 $f12, 0x0($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B57A0\n\t"
        "nop\n\t"
        "lwc1 $f12, 0xC($sp)\n\t"
        "b .Leboot_001B57A8\n\t"
        "swc1 $f12, 0x0($sp)\n\t"
        ".Leboot_001B57A0:\n\t"
        "lwc1 $f12, 0x18($sp)\n\t"
        "swc1 $f12, 0x0($sp)\n\t"
        ".Leboot_001B57A8:\n\t"
        "lwc1 $f12, 0x4($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B57C8\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x10($sp)\n\t"
        "b .Leboot_001B57D0\n\t"
        "swc1 $f12, 0x4($sp)\n\t"
        ".Leboot_001B57C8:\n\t"
        "lwc1 $f12, 0x1C($sp)\n\t"
        "swc1 $f12, 0x4($sp)\n\t"
        ".Leboot_001B57D0:\n\t"
        "lwc1 $f12, 0x8($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B57F4\n\t"
        "nop\n\t"
        "lwc1 $f14, 0x14($sp)\n\t"
        "lwc1 $f12, 0x0($sp)\n\t"
        "b .Leboot_001B5800\n\t"
        "swc1 $f14, 0x8($sp)\n\t"
        ".Leboot_001B57F4:\n\t"
        "lwc1 $f14, 0x20($sp)\n\t"
        "lwc1 $f12, 0x0($sp)\n\t"
        "swc1 $f14, 0x8($sp)\n\t"
        ".Leboot_001B5800:\n\t"
        "lwc1 $f14, 0x0($a2)\n\t"
        "lwc1 $f15, 0x4($a2)\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "swc1 $f14, 0x24($sp)\n\t"
        "swc1 $f15, 0x28($sp)\n\t"
        "swc1 $f16, 0x2C($sp)\n\t"
        "lwc1 $f14, 0x0($a0)\n\t"
        "lwc1 $f15, 0x4($a0)\n\t"
        "lwc1 $f16, 0x4($sp)\n\t"
        "mul.s $f12, $f14, $f12\n\t"
        "lwc1 $f17, 0x8($a0)\n\t"
        "mul.s $f15, $f15, $f16\n\t"
        "lwc1 $f18, 0x8($sp)\n\t"
        "lwc1 $f19, 0xC($a2)\n\t"
        "mul.s $f17, $f17, $f18\n\t"
        "add.s $f12, $f12, $f15\n\t"
        "add.s $f12, $f12, $f17\n\t"
        "add.s $f12, $f12, $f19\n\t"
        "c.lt.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B5860\n\t"
        "nop\n\t"
        "b .Leboot_001B5878\n\t"
        "or $v0, $zero, $zero\n\t"
        ".Leboot_001B5860:\n\t"
        "addiu $t2, $t2, 0x1\n\t"
        "addiu $a3, $a3, 0x10\n\t"
        "slt $t3, $t2, $t1\n\t"
        "bnez $t3, .Leboot_001B5750\n\t"
        "addiu $a2, $a2, 0x10\n\t"
        ".Leboot_001B5874:\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".Leboot_001B5878:\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

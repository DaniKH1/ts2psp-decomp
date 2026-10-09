/**
 * The Sims 2 PSP - syncSkeleton_2650 (0x1BB06C, 0x180 bytes)
 *
 *     addiu $sp, $sp, -0x60
 *     or $a3, $a1, $zero
 *     lwc1 $f13, 0x0($a3)
 *     lwc1 $f14, 0x0($a2)
 *     lwc1 $f15, 0x4($a3)
 *     lwc1 $f16, 0x4($a2)
 *     mul.s $f13, $f13, $f14
 *     mul.s $f15, $f15, $f16
 *     lwc1 $f17, 0x8($a3)
 *     lwc1 $f18, 0x8($a2)
 *     mul.s $f17, $f17, $f18
 *     add.s $f13, $f13, $f15
 *     lwc1 $f19, 0xC($a3)
 *     lwc1 $f0, 0xC($a2)
 *     mul.s $f14, $f19, $f0
 *     add.s $f13, $f13, $f17
 *     add.s $f13, $f13, $f14
 *     swc1 $f20, 0x50($sp)
 *     mtc1 $zero, $f20
 *     sw $s0, 0x54($sp)
 *     or $s0, $a0, $zero
 *     or $a1, $a2, $zero
 *     c.lt.s $f13, $f20
 *     sw $s1, 0x58($sp)
 *     sw $ra, 0x5C($sp)
 *     bc1f .Leboot_001BB10C
 *     or $a0, $a3, $zero
 *     lwc1 $f13, 0x0($a2)
 *     lwc1 $f14, 0x4($a2)
 *     lwc1 $f15, 0x8($a2)
 *     neg.s $f13, $f13
 *     lwc1 $f16, 0xC($a2)
 *     neg.s $f14, $f14
 *     swc1 $f13, 0x40($sp)
 *     neg.s $f15, $f15
 *     swc1 $f14, 0x44($sp)
 *     neg.s $f16, $f16
 *     swc1 $f15, 0x48($sp)
 *     addiu $a1, $sp, 0x40
 *     swc1 $f16, 0x4C($sp)
 *   .Leboot_001BB10C
 *     lwc1 $f13, 0x0($a1)
 *     swc1 $f13, 0x20($sp)
 *     lwc1 $f13, 0x4($a1)
 *     or $a3, $a0, $zero
 *     swc1 $f13, 0x24($sp)
 *     lwc1 $f13, 0x8($a1)
 *     addiu $s1, $sp, 0x10
 *     swc1 $f13, 0x28($sp)
 *     lwc1 $f13, 0xC($a1)
 *     addiu $a2, $sp, 0x20
 *     or $a0, $s1, $zero
 *     swc1 $f13, 0x2C($sp)
 *     jal func_001AF16C
 *     or $a1, $a3, $zero
 *     lwc1 $f12, 0x0($s1)
 *     swc1 $f12, 0x0($s0)
 *     lwc1 $f12, 0x4($s1)
 *     lwc1 $f13, 0x0($s0)
 *     swc1 $f12, 0x4($s0)
 *     lwc1 $f12, 0x8($s1)
 *     mul.s $f14, $f13, $f13
 *     lwc1 $f15, 0x4($s0)
 *     swc1 $f12, 0x8($s0)
 *     mul.s $f12, $f15, $f15
 *     lwc1 $f16, 0xC($s1)
 *     lwc1 $f17, 0x8($s0)
 *     mul.s $f18, $f16, $f16
 *     mul.s $f17, $f17, $f17
 *     add.s $f12, $f14, $f12
 *     swc1 $f16, 0xC($s0)
 *     add.s $f12, $f12, $f17
 *     add.s $f12, $f12, $f18
 *     sqrt.s $f12, $f12
 *     lui $a0, 0x3F80
 *     c.lt.s $f16, $f20
 *     mtc1 $a0, $f15
 *     div.s $f12, $f15, $f12
 *     bc1tl .Leboot_001BB1A8
 *     neg.s $f12, $f12
 *   .Leboot_001BB1A8
 *     lwc1 $f14, 0x4($s0)
 *     mul.s $f13, $f13, $f12
 *     lwc1 $f15, 0x8($s0)
 *     mul.s $f14, $f14, $f12
 *     lwc1 $f16, 0xC($s0)
 *     mul.s $f15, $f15, $f12
 *     swc1 $f13, 0x0($s0)
 *     swc1 $f14, 0x4($s0)
 *     mul.s $f12, $f16, $f12
 *     swc1 $f15, 0x8($s0)
 *     swc1 $f12, 0xC($s0)
 *     lwc1 $f20, 0x50($sp)
 *     lw $s0, 0x54($sp)
 *     lw $s1, 0x58($sp)
 *     lw $ra, 0x5C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x60
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_2650(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x60\n\t"
        "or $a3, $a1, $zero\n\t"
        "lwc1 $f13, 0x0($a3)\n\t"
        "lwc1 $f14, 0x0($a2)\n\t"
        "lwc1 $f15, 0x4($a3)\n\t"
        "lwc1 $f16, 0x4($a2)\n\t"
        "mul.s $f13, $f13, $f14\n\t"
        "mul.s $f15, $f15, $f16\n\t"
        "lwc1 $f17, 0x8($a3)\n\t"
        "lwc1 $f18, 0x8($a2)\n\t"
        "mul.s $f17, $f17, $f18\n\t"
        "add.s $f13, $f13, $f15\n\t"
        "lwc1 $f19, 0xC($a3)\n\t"
        "lwc1 $f0, 0xC($a2)\n\t"
        "mul.s $f14, $f19, $f0\n\t"
        "add.s $f13, $f13, $f17\n\t"
        "add.s $f13, $f13, $f14\n\t"
        "swc1 $f20, 0x50($sp)\n\t"
        "mtc1 $zero, $f20\n\t"
        "sw $s0, 0x54($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "or $a1, $a2, $zero\n\t"
        "c.lt.s $f13, $f20\n\t"
        "sw $s1, 0x58($sp)\n\t"
        "sw $ra, 0x5C($sp)\n\t"
        "bc1f .Leboot_001BB10C\n\t"
        "or $a0, $a3, $zero\n\t"
        "lwc1 $f13, 0x0($a2)\n\t"
        "lwc1 $f14, 0x4($a2)\n\t"
        "lwc1 $f15, 0x8($a2)\n\t"
        "neg.s $f13, $f13\n\t"
        "lwc1 $f16, 0xC($a2)\n\t"
        "neg.s $f14, $f14\n\t"
        "swc1 $f13, 0x40($sp)\n\t"
        "neg.s $f15, $f15\n\t"
        "swc1 $f14, 0x44($sp)\n\t"
        "neg.s $f16, $f16\n\t"
        "swc1 $f15, 0x48($sp)\n\t"
        "addiu $a1, $sp, 0x40\n\t"
        "swc1 $f16, 0x4C($sp)\n\t"
        ".Leboot_001BB10C:\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "swc1 $f13, 0x20($sp)\n\t"
        "lwc1 $f13, 0x4($a1)\n\t"
        "or $a3, $a0, $zero\n\t"
        "swc1 $f13, 0x24($sp)\n\t"
        "lwc1 $f13, 0x8($a1)\n\t"
        "addiu $s1, $sp, 0x10\n\t"
        "swc1 $f13, 0x28($sp)\n\t"
        "lwc1 $f13, 0xC($a1)\n\t"
        "addiu $a2, $sp, 0x20\n\t"
        "or $a0, $s1, $zero\n\t"
        "swc1 $f13, 0x2C($sp)\n\t"
        "jal func_001AF16C\n\t"
        "or $a1, $a3, $zero\n\t"
        "lwc1 $f12, 0x0($s1)\n\t"
        "swc1 $f12, 0x0($s0)\n\t"
        "lwc1 $f12, 0x4($s1)\n\t"
        "lwc1 $f13, 0x0($s0)\n\t"
        "swc1 $f12, 0x4($s0)\n\t"
        "lwc1 $f12, 0x8($s1)\n\t"
        "mul.s $f14, $f13, $f13\n\t"
        "lwc1 $f15, 0x4($s0)\n\t"
        "swc1 $f12, 0x8($s0)\n\t"
        "mul.s $f12, $f15, $f15\n\t"
        "lwc1 $f16, 0xC($s1)\n\t"
        "lwc1 $f17, 0x8($s0)\n\t"
        "mul.s $f18, $f16, $f16\n\t"
        "mul.s $f17, $f17, $f17\n\t"
        "add.s $f12, $f14, $f12\n\t"
        "swc1 $f16, 0xC($s0)\n\t"
        "add.s $f12, $f12, $f17\n\t"
        "add.s $f12, $f12, $f18\n\t"
        "sqrt.s $f12, $f12\n\t"
        "lui $a0, 0x3F80\n\t"
        "c.lt.s $f16, $f20\n\t"
        "mtc1 $a0, $f15\n\t"
        "div.s $f12, $f15, $f12\n\t"
        "bc1tl .Leboot_001BB1A8\n\t"
        "neg.s $f12, $f12\n\t"
        ".Leboot_001BB1A8:\n\t"
        "lwc1 $f14, 0x4($s0)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "lwc1 $f15, 0x8($s0)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1 $f16, 0xC($s0)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "swc1 $f13, 0x0($s0)\n\t"
        "swc1 $f14, 0x4($s0)\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        "swc1 $f15, 0x8($s0)\n\t"
        "swc1 $f12, 0xC($s0)\n\t"
        "lwc1 $f20, 0x50($sp)\n\t"
        "lw $s0, 0x54($sp)\n\t"
        "lw $s1, 0x58($sp)\n\t"
        "lw $ra, 0x5C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x60\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

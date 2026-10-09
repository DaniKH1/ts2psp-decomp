/**
 * The Sims 2 PSP - sortAndCullScene_18DC (0x1B54F8, 0x74 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s0, 0x1C($sp)
 *     sw $s1, 0x20($sp)
 *     or $s0, $a1, $zero
 *     addiu $s1, $sp, 0x10
 *     or $a1, $s1, $zero
 *     sw $ra, 0x24($sp)
 *     jal sortAndCullScene_1844
 *     or $a2, $s0, $zero
 *     lwc1 $f12, 0x0($s0)
 *     lwc1 $f13, 0x0($s1)
 *     lwc1 $f14, 0x4($s0)
 *     lwc1 $f15, 0x4($s1)
 *     sub.s $f12, $f12, $f13
 *     sub.s $f14, $f14, $f15
 *     lwc1 $f16, 0x8($s0)
 *     lwc1 $f17, 0x8($s1)
 *     mul.s $f12, $f12, $f12
 *     sub.s $f16, $f16, $f17
 *     mul.s $f14, $f14, $f14
 *     mul.s $f13, $f16, $f16
 *     add.s $f12, $f12, $f14
 *     add.s $f0, $f12, $f13
 *     sqrt.s $f0, $f0
 *     lw $s0, 0x1C($sp)
 *     lw $s1, 0x20($sp)
 *     lw $ra, 0x24($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_18DC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s0, 0x1C($sp)\n\t"
        "sw $s1, 0x20($sp)\n\t"
        "or $s0, $a1, $zero\n\t"
        "addiu $s1, $sp, 0x10\n\t"
        "or $a1, $s1, $zero\n\t"
        "sw $ra, 0x24($sp)\n\t"
        "jal sortAndCullScene_1844\n\t"
        "or $a2, $s0, $zero\n\t"
        "lwc1 $f12, 0x0($s0)\n\t"
        "lwc1 $f13, 0x0($s1)\n\t"
        "lwc1 $f14, 0x4($s0)\n\t"
        "lwc1 $f15, 0x4($s1)\n\t"
        "sub.s $f12, $f12, $f13\n\t"
        "sub.s $f14, $f14, $f15\n\t"
        "lwc1 $f16, 0x8($s0)\n\t"
        "lwc1 $f17, 0x8($s1)\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "sub.s $f16, $f16, $f17\n\t"
        "mul.s $f14, $f14, $f14\n\t"
        "mul.s $f13, $f16, $f16\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "add.s $f0, $f12, $f13\n\t"
        "sqrt.s $f0, $f0\n\t"
        "lw $s0, 0x1C($sp)\n\t"
        "lw $s1, 0x20($sp)\n\t"
        "lw $ra, 0x24($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - sortAndCullScene_179C (0x1B53B8, 0x5C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     addiu $a2, $a0, 0x20
 *     lwc1 $f12, 0x0($a2)
 *     lwc1 $f13, 0x0($a1)
 *     lwc1 $f14, 0x4($a2)
 *     lwc1 $f15, 0x4($a1)
 *     sub.s $f12, $f12, $f13
 *     sub.s $f14, $f14, $f15
 *     lwc1 $f16, 0x8($a2)
 *     lwc1 $f17, 0x8($a1)
 *     mul.s $f12, $f12, $f12
 *     sub.s $f16, $f16, $f17
 *     mul.s $f14, $f14, $f14
 *     mul.s $f13, $f16, $f16
 *     add.s $f12, $f12, $f14
 *     add.s $f12, $f12, $f13
 *     sw $ra, 0x10($sp)
 *     sqrt.s $f12, $f12
 *     jal sortAndCullScene_17F8
 *     nop
 *     lw $ra, 0x10($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * sortAndCullScene: subtract the float at 0x0 of the second argument from the float at 0x20 of the first.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_179C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "addiu $a2, $a0, 0x20\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "lwc1 $f14, 0x4($a2)\n\t"
        "lwc1 $f15, 0x4($a1)\n\t"
        "sub.s $f12, $f12, $f13\n\t"
        "sub.s $f14, $f14, $f15\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "lwc1 $f17, 0x8($a1)\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "sub.s $f16, $f16, $f17\n\t"
        "mul.s $f14, $f14, $f14\n\t"
        "mul.s $f13, $f16, $f16\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "sw $ra, 0x10($sp)\n\t"
        "sqrt.s $f12, $f12\n\t"
        "jal sortAndCullScene_17F8\n\t"
        "nop\n\t"
        "lw $ra, 0x10($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - drawing_0914 (0x1BBB94, 0xA4 bytes)
 *
 *     addiu $sp, $sp, -0x80
 *     lui $a0, %%hi(sym_001DAF50)
 *     lwc1 $f16, %%lo(sym_001DAF50)($a0)
 *     swc1 $f12, 0x20($sp)
 *     add.s $f14, $f14, $f12
 *     swc1 $f13, 0x24($sp)
 *     mtc1 $zero, $f17
 *     swc1 $f16, 0x28($sp)
 *     swc1 $f17, 0x2C($sp)
 *     add.s $f15, $f15, $f13
 *     swc1 $f17, 0x30($sp)
 *     swc1 $f14, 0x34($sp)
 *     swc1 $f13, 0x38($sp)
 *     swc1 $f16, 0x3C($sp)
 *     lui $a3, 0x3F80
 *     mtc1 $a3, $f13
 *     swc1 $f17, 0x44($sp)
 *     swc1 $f13, 0x40($sp)
 *     swc1 $f14, 0x48($sp)
 *     swc1 $f15, 0x4C($sp)
 *     swc1 $f16, 0x50($sp)
 *     swc1 $f13, 0x54($sp)
 *     swc1 $f13, 0x58($sp)
 *     swc1 $f12, 0x5C($sp)
 *     swc1 $f15, 0x60($sp)
 *     swc1 $f16, 0x64($sp)
 *     swc1 $f17, 0x68($sp)
 *     addiu $a0, $sp, 0x20
 *     addiu $a1, $sp, 0x2C
 *     addiu $a2, $sp, 0x34
 *     addiu $a3, $sp, 0x40
 *     addiu $t0, $sp, 0x48
 *     addiu $t1, $sp, 0x54
 *     addiu $t2, $sp, 0x5C
 *     swc1 $f13, 0x6C($sp)
 *     sw $ra, 0x70($sp)
 *     jal drawing_0A50
 *     addiu $t3, $sp, 0x68
 *     lw $ra, 0x70($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x80
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0914(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x80\n\t"
        "lui $a0, %%hi(sym_001DAF50)\n\t"
        "lwc1 $f16, %%lo(sym_001DAF50)($a0)\n\t"
        "swc1 $f12, 0x20($sp)\n\t"
        "add.s $f14, $f14, $f12\n\t"
        "swc1 $f13, 0x24($sp)\n\t"
        "mtc1 $zero, $f17\n\t"
        "swc1 $f16, 0x28($sp)\n\t"
        "swc1 $f17, 0x2C($sp)\n\t"
        "add.s $f15, $f15, $f13\n\t"
        "swc1 $f17, 0x30($sp)\n\t"
        "swc1 $f14, 0x34($sp)\n\t"
        "swc1 $f13, 0x38($sp)\n\t"
        "swc1 $f16, 0x3C($sp)\n\t"
        "lui $a3, 0x3F80\n\t"
        "mtc1 $a3, $f13\n\t"
        "swc1 $f17, 0x44($sp)\n\t"
        "swc1 $f13, 0x40($sp)\n\t"
        "swc1 $f14, 0x48($sp)\n\t"
        "swc1 $f15, 0x4C($sp)\n\t"
        "swc1 $f16, 0x50($sp)\n\t"
        "swc1 $f13, 0x54($sp)\n\t"
        "swc1 $f13, 0x58($sp)\n\t"
        "swc1 $f12, 0x5C($sp)\n\t"
        "swc1 $f15, 0x60($sp)\n\t"
        "swc1 $f16, 0x64($sp)\n\t"
        "swc1 $f17, 0x68($sp)\n\t"
        "addiu $a0, $sp, 0x20\n\t"
        "addiu $a1, $sp, 0x2C\n\t"
        "addiu $a2, $sp, 0x34\n\t"
        "addiu $a3, $sp, 0x40\n\t"
        "addiu $t0, $sp, 0x48\n\t"
        "addiu $t1, $sp, 0x54\n\t"
        "addiu $t2, $sp, 0x5C\n\t"
        "swc1 $f13, 0x6C($sp)\n\t"
        "sw $ra, 0x70($sp)\n\t"
        "jal drawing_0A50\n\t"
        "addiu $t3, $sp, 0x68\n\t"
        "lw $ra, 0x70($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x80\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

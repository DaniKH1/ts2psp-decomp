/**
 * The Sims 2 PSP - drawing_00B4 (0x1BB334, 0x378 bytes)
 *
 *     addiu $sp, $sp, -0x110
 *     lh $t0, 0x10($a0)
 *     mtc1 $t0, $f13
 *     cvt.s.w $f13, $f13
 *     lwc1 $f12, 0x0($a3)
 *     lwc1 $f14, 0x4($a3)
 *     lwc1 $f15, 0x8($a3)
 *     mul.s $f12, $f12, $f13
 *     mul.s $f14, $f14, $f13
 *     mul.s $f15, $f15, $f13
 *     swc1 $f12, 0x2C($sp)
 *     swc1 $f14, 0x30($sp)
 *     swc1 $f15, 0x34($sp)
 *     addiu $t1, $sp, 0x2C
 *     lwc1 $f12, 0x0($a1)
 *     lwc1 $f14, 0x0($t1)
 *     lwc1 $f15, 0x4($a1)
 *     lwc1 $f16, 0x4($t1)
 *     add.s $f12, $f12, $f14
 *     lwc1 $f17, 0x8($a1)
 *     lwc1 $f18, 0x8($t1)
 *     add.s $f15, $f15, $f16
 *     lh $t1, 0x12($a0)
 *     add.s $f17, $f17, $f18
 *     swc1 $f12, 0x20($sp)
 *     mtc1 $t1, $f12
 *     swc1 $f15, 0x24($sp)
 *     cvt.s.w $f12, $f12
 *     swc1 $f17, 0x28($sp)
 *     lwc1 $f14, 0x0($a2)
 *     lwc1 $f15, 0x4($a2)
 *     mul.s $f14, $f14, $f12
 *     lwc1 $f16, 0x8($a2)
 *     mul.s $f15, $f15, $f12
 *     addiu $t2, $sp, 0x20
 *     mul.s $f16, $f16, $f12
 *     lwc1 $f17, 0x0($t2)
 *     swc1 $f14, 0x38($sp)
 *     swc1 $f15, 0x3C($sp)
 *     addiu $t3, $sp, 0x38
 *     swc1 $f16, 0x40($sp)
 *     lwc1 $f14, 0x0($t3)
 *     lwc1 $f15, 0x4($t2)
 *     lwc1 $f16, 0x4($t3)
 *     add.s $f14, $f17, $f14
 *     lwc1 $f18, 0x8($t2)
 *     lwc1 $f19, 0x8($t3)
 *     add.s $f15, $f15, $f16
 *     lh $t2, 0x14($a0)
 *     add.s $f18, $f18, $f19
 *     swc1 $f14, 0xB0($sp)
 *     addu $t0, $t0, $t2
 *     mtc1 $t0, $f14
 *     swc1 $f15, 0xB4($sp)
 *     cvt.s.w $f14, $f14
 *     swc1 $f18, 0xB8($sp)
 *     lwc1 $f15, 0x0($a3)
 *     lwc1 $f16, 0x4($a3)
 *     mul.s $f15, $f15, $f14
 *     lwc1 $f17, 0x8($a3)
 *     mul.s $f16, $f16, $f14
 *     mul.s $f17, $f17, $f14
 *     swc1 $f15, 0x50($sp)
 *     swc1 $f16, 0x54($sp)
 *     swc1 $f17, 0x58($sp)
 *     addiu $t0, $sp, 0x50
 *     lwc1 $f15, 0x0($a1)
 *     lwc1 $f16, 0x0($t0)
 *     lwc1 $f17, 0x4($a1)
 *     lwc1 $f18, 0x4($t0)
 *     lwc1 $f19, 0x8($a1)
 *     add.s $f15, $f15, $f16
 *     lwc1 $f0, 0x8($t0)
 *     add.s $f17, $f17, $f18
 *     add.s $f19, $f19, $f0
 *     swc1 $f15, 0x44($sp)
 *     swc1 $f17, 0x48($sp)
 *     swc1 $f19, 0x4C($sp)
 *     lwc1 $f15, 0x0($a2)
 *     lwc1 $f17, 0x4($a2)
 *     mul.s $f15, $f15, $f12
 *     mul.s $f17, $f17, $f12
 *     lwc1 $f16, 0x8($a2)
 *     addiu $t0, $sp, 0x44
 *     mul.s $f12, $f16, $f12
 *     lwc1 $f18, 0x0($t0)
 *     swc1 $f15, 0x5C($sp)
 *     swc1 $f17, 0x60($sp)
 *     addiu $t2, $sp, 0x5C
 *     swc1 $f12, 0x64($sp)
 *     lwc1 $f12, 0x0($t2)
 *     lwc1 $f15, 0x4($t0)
 *     lwc1 $f17, 0x4($t2)
 *     lwc1 $f16, 0x8($t0)
 *     add.s $f12, $f18, $f12
 *     lwc1 $f19, 0x8($t2)
 *     add.s $f15, $f15, $f17
 *     add.s $f16, $f16, $f19
 *     swc1 $f12, 0xBC($sp)
 *     swc1 $f15, 0xC0($sp)
 *     swc1 $f16, 0xC4($sp)
 *     lwc1 $f12, 0x0($a3)
 *     lwc1 $f15, 0x4($a3)
 *     mul.s $f12, $f12, $f14
 *     mul.s $f15, $f15, $f14
 *     lwc1 $f16, 0x8($a3)
 *     mul.s $f14, $f16, $f14
 *     swc1 $f12, 0x74($sp)
 *     swc1 $f15, 0x78($sp)
 *     swc1 $f14, 0x7C($sp)
 *     addiu $t0, $sp, 0x74
 *     lwc1 $f12, 0x0($a1)
 *     lwc1 $f14, 0x0($t0)
 *     lwc1 $f15, 0x4($a1)
 *     lwc1 $f17, 0x4($t0)
 *     add.s $f12, $f12, $f14
 *     lwc1 $f16, 0x8($a1)
 *     lwc1 $f18, 0x8($t0)
 *     add.s $f15, $f15, $f17
 *     lh $t0, 0x16($a0)
 *     add.s $f16, $f16, $f18
 *     swc1 $f12, 0x68($sp)
 *     addu $t0, $t1, $t0
 *     mtc1 $t0, $f12
 *     swc1 $f15, 0x6C($sp)
 *     cvt.s.w $f12, $f12
 *     swc1 $f16, 0x70($sp)
 *     lwc1 $f14, 0x0($a2)
 *     lwc1 $f15, 0x4($a2)
 *     mul.s $f14, $f14, $f12
 *     lwc1 $f16, 0x8($a2)
 *     mul.s $f15, $f15, $f12
 *     addiu $t0, $sp, 0x68
 *     mul.s $f16, $f16, $f12
 *     lwc1 $f17, 0x0($t0)
 *     swc1 $f14, 0x80($sp)
 *     swc1 $f15, 0x84($sp)
 *     addiu $t1, $sp, 0x80
 *     swc1 $f16, 0x88($sp)
 *     lwc1 $f14, 0x0($t1)
 *     lwc1 $f15, 0x4($t0)
 *     lwc1 $f16, 0x4($t1)
 *     lwc1 $f18, 0x8($t0)
 *     add.s $f14, $f17, $f14
 *     lwc1 $f19, 0x8($t1)
 *     add.s $f15, $f15, $f16
 *     add.s $f18, $f18, $f19
 *     swc1 $f14, 0xC8($sp)
 *     swc1 $f15, 0xCC($sp)
 *     swc1 $f18, 0xD0($sp)
 *     lwc1 $f14, 0x0($a3)
 *     lwc1 $f15, 0x4($a3)
 *     mul.s $f14, $f14, $f13
 *     mul.s $f15, $f15, $f13
 *     lwc1 $f17, 0x8($a3)
 *     mul.s $f13, $f17, $f13
 *     swc1 $f14, 0x98($sp)
 *     swc1 $f15, 0x9C($sp)
 *     swc1 $f13, 0xA0($sp)
 *     addiu $a3, $sp, 0x98
 *     lwc1 $f13, 0x0($a1)
 *     lwc1 $f14, 0x0($a3)
 *     lwc1 $f15, 0x4($a1)
 *     lwc1 $f16, 0x4($a3)
 *     lwc1 $f17, 0x8($a1)
 *     add.s $f13, $f13, $f14
 *     lwc1 $f18, 0x8($a3)
 *     add.s $f15, $f15, $f16
 *     add.s $f17, $f17, $f18
 *     swc1 $f13, 0x8C($sp)
 *     swc1 $f15, 0x90($sp)
 *     swc1 $f17, 0x94($sp)
 *     lwc1 $f13, 0x0($a2)
 *     lwc1 $f14, 0x4($a2)
 *     mul.s $f13, $f13, $f12
 *     mul.s $f14, $f14, $f12
 *     lwc1 $f15, 0x8($a2)
 *     addiu $a1, $sp, 0x8C
 *     mul.s $f12, $f15, $f12
 *     lwc1 $f16, 0x0($a1)
 *     swc1 $f13, 0xA4($sp)
 *     swc1 $f14, 0xA8($sp)
 *     addiu $a2, $sp, 0xA4
 *     swc1 $f12, 0xAC($sp)
 *     lwc1 $f12, 0x0($a2)
 *     lwc1 $f13, 0x4($a1)
 *     lwc1 $f14, 0x4($a2)
 *     add.s $f12, $f16, $f12
 *     lwc1 $f15, 0x8($a1)
 *     lwc1 $f17, 0x8($a2)
 *     add.s $f13, $f13, $f14
 *     lwc1 $f18, 0x0($a0)
 *     add.s $f15, $f15, $f17
 *     swc1 $f12, 0xD4($sp)
 *     swc1 $f13, 0xD8($sp)
 *     lwc1 $f12, 0x4($a0)
 *     swc1 $f15, 0xDC($sp)
 *     swc1 $f18, 0xE0($sp)
 *     swc1 $f12, 0xE4($sp)
 *     lwc1 $f13, 0x8($a0)
 *     swc1 $f13, 0xE8($sp)
 *     swc1 $f12, 0xEC($sp)
 *     lwc1 $f12, 0xC($a0)
 *     swc1 $f13, 0xF0($sp)
 *     swc1 $f12, 0xF4($sp)
 *     addiu $v0, $sp, 0xB0
 *     swc1 $f18, 0xF8($sp)
 *     addiu $v1, $sp, 0xBC
 *     addiu $t0, $sp, 0xC8
 *     addiu $t2, $sp, 0xD4
 *     addiu $a1, $sp, 0xE0
 *     addiu $a3, $sp, 0xE8
 *     addiu $t1, $sp, 0xF0
 *     swc1 $f12, 0xFC($sp)
 *     addiu $t3, $sp, 0xF8
 *     or $a0, $v0, $zero
 *     sw $ra, 0x100($sp)
 *     jal drawing_0A50
 *     or $a2, $v1, $zero
 *     lw $ra, 0x100($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x110
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_00B4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x110\n\t"
        "lh $t0, 0x10($a0)\n\t"
        "mtc1 $t0, $f13\n\t"
        "cvt.s.w $f13, $f13\n\t"
        "lwc1 $f12, 0x0($a3)\n\t"
        "lwc1 $f14, 0x4($a3)\n\t"
        "lwc1 $f15, 0x8($a3)\n\t"
        "mul.s $f12, $f12, $f13\n\t"
        "mul.s $f14, $f14, $f13\n\t"
        "mul.s $f15, $f15, $f13\n\t"
        "swc1 $f12, 0x2C($sp)\n\t"
        "swc1 $f14, 0x30($sp)\n\t"
        "swc1 $f15, 0x34($sp)\n\t"
        "addiu $t1, $sp, 0x2C\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "lwc1 $f14, 0x0($t1)\n\t"
        "lwc1 $f15, 0x4($a1)\n\t"
        "lwc1 $f16, 0x4($t1)\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "lwc1 $f17, 0x8($a1)\n\t"
        "lwc1 $f18, 0x8($t1)\n\t"
        "add.s $f15, $f15, $f16\n\t"
        "lh $t1, 0x12($a0)\n\t"
        "add.s $f17, $f17, $f18\n\t"
        "swc1 $f12, 0x20($sp)\n\t"
        "mtc1 $t1, $f12\n\t"
        "swc1 $f15, 0x24($sp)\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "swc1 $f17, 0x28($sp)\n\t"
        "lwc1 $f14, 0x0($a2)\n\t"
        "lwc1 $f15, 0x4($a2)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $t2, $sp, 0x20\n\t"
        "mul.s $f16, $f16, $f12\n\t"
        "lwc1 $f17, 0x0($t2)\n\t"
        "swc1 $f14, 0x38($sp)\n\t"
        "swc1 $f15, 0x3C($sp)\n\t"
        "addiu $t3, $sp, 0x38\n\t"
        "swc1 $f16, 0x40($sp)\n\t"
        "lwc1 $f14, 0x0($t3)\n\t"
        "lwc1 $f15, 0x4($t2)\n\t"
        "lwc1 $f16, 0x4($t3)\n\t"
        "add.s $f14, $f17, $f14\n\t"
        "lwc1 $f18, 0x8($t2)\n\t"
        "lwc1 $f19, 0x8($t3)\n\t"
        "add.s $f15, $f15, $f16\n\t"
        "lh $t2, 0x14($a0)\n\t"
        "add.s $f18, $f18, $f19\n\t"
        "swc1 $f14, 0xB0($sp)\n\t"
        "addu $t0, $t0, $t2\n\t"
        "mtc1 $t0, $f14\n\t"
        "swc1 $f15, 0xB4($sp)\n\t"
        "cvt.s.w $f14, $f14\n\t"
        "swc1 $f18, 0xB8($sp)\n\t"
        "lwc1 $f15, 0x0($a3)\n\t"
        "lwc1 $f16, 0x4($a3)\n\t"
        "mul.s $f15, $f15, $f14\n\t"
        "lwc1 $f17, 0x8($a3)\n\t"
        "mul.s $f16, $f16, $f14\n\t"
        "mul.s $f17, $f17, $f14\n\t"
        "swc1 $f15, 0x50($sp)\n\t"
        "swc1 $f16, 0x54($sp)\n\t"
        "swc1 $f17, 0x58($sp)\n\t"
        "addiu $t0, $sp, 0x50\n\t"
        "lwc1 $f15, 0x0($a1)\n\t"
        "lwc1 $f16, 0x0($t0)\n\t"
        "lwc1 $f17, 0x4($a1)\n\t"
        "lwc1 $f18, 0x4($t0)\n\t"
        "lwc1 $f19, 0x8($a1)\n\t"
        "add.s $f15, $f15, $f16\n\t"
        "lwc1 $f0, 0x8($t0)\n\t"
        "add.s $f17, $f17, $f18\n\t"
        "add.s $f19, $f19, $f0\n\t"
        "swc1 $f15, 0x44($sp)\n\t"
        "swc1 $f17, 0x48($sp)\n\t"
        "swc1 $f19, 0x4C($sp)\n\t"
        "lwc1 $f15, 0x0($a2)\n\t"
        "lwc1 $f17, 0x4($a2)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "mul.s $f17, $f17, $f12\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "addiu $t0, $sp, 0x44\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        "lwc1 $f18, 0x0($t0)\n\t"
        "swc1 $f15, 0x5C($sp)\n\t"
        "swc1 $f17, 0x60($sp)\n\t"
        "addiu $t2, $sp, 0x5C\n\t"
        "swc1 $f12, 0x64($sp)\n\t"
        "lwc1 $f12, 0x0($t2)\n\t"
        "lwc1 $f15, 0x4($t0)\n\t"
        "lwc1 $f17, 0x4($t2)\n\t"
        "lwc1 $f16, 0x8($t0)\n\t"
        "add.s $f12, $f18, $f12\n\t"
        "lwc1 $f19, 0x8($t2)\n\t"
        "add.s $f15, $f15, $f17\n\t"
        "add.s $f16, $f16, $f19\n\t"
        "swc1 $f12, 0xBC($sp)\n\t"
        "swc1 $f15, 0xC0($sp)\n\t"
        "swc1 $f16, 0xC4($sp)\n\t"
        "lwc1 $f12, 0x0($a3)\n\t"
        "lwc1 $f15, 0x4($a3)\n\t"
        "mul.s $f12, $f12, $f14\n\t"
        "mul.s $f15, $f15, $f14\n\t"
        "lwc1 $f16, 0x8($a3)\n\t"
        "mul.s $f14, $f16, $f14\n\t"
        "swc1 $f12, 0x74($sp)\n\t"
        "swc1 $f15, 0x78($sp)\n\t"
        "swc1 $f14, 0x7C($sp)\n\t"
        "addiu $t0, $sp, 0x74\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "lwc1 $f14, 0x0($t0)\n\t"
        "lwc1 $f15, 0x4($a1)\n\t"
        "lwc1 $f17, 0x4($t0)\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "lwc1 $f16, 0x8($a1)\n\t"
        "lwc1 $f18, 0x8($t0)\n\t"
        "add.s $f15, $f15, $f17\n\t"
        "lh $t0, 0x16($a0)\n\t"
        "add.s $f16, $f16, $f18\n\t"
        "swc1 $f12, 0x68($sp)\n\t"
        "addu $t0, $t1, $t0\n\t"
        "mtc1 $t0, $f12\n\t"
        "swc1 $f15, 0x6C($sp)\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "swc1 $f16, 0x70($sp)\n\t"
        "lwc1 $f14, 0x0($a2)\n\t"
        "lwc1 $f15, 0x4($a2)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $t0, $sp, 0x68\n\t"
        "mul.s $f16, $f16, $f12\n\t"
        "lwc1 $f17, 0x0($t0)\n\t"
        "swc1 $f14, 0x80($sp)\n\t"
        "swc1 $f15, 0x84($sp)\n\t"
        "addiu $t1, $sp, 0x80\n\t"
        "swc1 $f16, 0x88($sp)\n\t"
        "lwc1 $f14, 0x0($t1)\n\t"
        "lwc1 $f15, 0x4($t0)\n\t"
        "lwc1 $f16, 0x4($t1)\n\t"
        "lwc1 $f18, 0x8($t0)\n\t"
        "add.s $f14, $f17, $f14\n\t"
        "lwc1 $f19, 0x8($t1)\n\t"
        "add.s $f15, $f15, $f16\n\t"
        "add.s $f18, $f18, $f19\n\t"
        "swc1 $f14, 0xC8($sp)\n\t"
        "swc1 $f15, 0xCC($sp)\n\t"
        "swc1 $f18, 0xD0($sp)\n\t"
        "lwc1 $f14, 0x0($a3)\n\t"
        "lwc1 $f15, 0x4($a3)\n\t"
        "mul.s $f14, $f14, $f13\n\t"
        "mul.s $f15, $f15, $f13\n\t"
        "lwc1 $f17, 0x8($a3)\n\t"
        "mul.s $f13, $f17, $f13\n\t"
        "swc1 $f14, 0x98($sp)\n\t"
        "swc1 $f15, 0x9C($sp)\n\t"
        "swc1 $f13, 0xA0($sp)\n\t"
        "addiu $a3, $sp, 0x98\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "lwc1 $f14, 0x0($a3)\n\t"
        "lwc1 $f15, 0x4($a1)\n\t"
        "lwc1 $f16, 0x4($a3)\n\t"
        "lwc1 $f17, 0x8($a1)\n\t"
        "add.s $f13, $f13, $f14\n\t"
        "lwc1 $f18, 0x8($a3)\n\t"
        "add.s $f15, $f15, $f16\n\t"
        "add.s $f17, $f17, $f18\n\t"
        "swc1 $f13, 0x8C($sp)\n\t"
        "swc1 $f15, 0x90($sp)\n\t"
        "swc1 $f17, 0x94($sp)\n\t"
        "lwc1 $f13, 0x0($a2)\n\t"
        "lwc1 $f14, 0x4($a2)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1 $f15, 0x8($a2)\n\t"
        "addiu $a1, $sp, 0x8C\n\t"
        "mul.s $f12, $f15, $f12\n\t"
        "lwc1 $f16, 0x0($a1)\n\t"
        "swc1 $f13, 0xA4($sp)\n\t"
        "swc1 $f14, 0xA8($sp)\n\t"
        "addiu $a2, $sp, 0xA4\n\t"
        "swc1 $f12, 0xAC($sp)\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "lwc1 $f13, 0x4($a1)\n\t"
        "lwc1 $f14, 0x4($a2)\n\t"
        "add.s $f12, $f16, $f12\n\t"
        "lwc1 $f15, 0x8($a1)\n\t"
        "lwc1 $f17, 0x8($a2)\n\t"
        "add.s $f13, $f13, $f14\n\t"
        "lwc1 $f18, 0x0($a0)\n\t"
        "add.s $f15, $f15, $f17\n\t"
        "swc1 $f12, 0xD4($sp)\n\t"
        "swc1 $f13, 0xD8($sp)\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "swc1 $f15, 0xDC($sp)\n\t"
        "swc1 $f18, 0xE0($sp)\n\t"
        "swc1 $f12, 0xE4($sp)\n\t"
        "lwc1 $f13, 0x8($a0)\n\t"
        "swc1 $f13, 0xE8($sp)\n\t"
        "swc1 $f12, 0xEC($sp)\n\t"
        "lwc1 $f12, 0xC($a0)\n\t"
        "swc1 $f13, 0xF0($sp)\n\t"
        "swc1 $f12, 0xF4($sp)\n\t"
        "addiu $v0, $sp, 0xB0\n\t"
        "swc1 $f18, 0xF8($sp)\n\t"
        "addiu $v1, $sp, 0xBC\n\t"
        "addiu $t0, $sp, 0xC8\n\t"
        "addiu $t2, $sp, 0xD4\n\t"
        "addiu $a1, $sp, 0xE0\n\t"
        "addiu $a3, $sp, 0xE8\n\t"
        "addiu $t1, $sp, 0xF0\n\t"
        "swc1 $f12, 0xFC($sp)\n\t"
        "addiu $t3, $sp, 0xF8\n\t"
        "or $a0, $v0, $zero\n\t"
        "sw $ra, 0x100($sp)\n\t"
        "jal drawing_0A50\n\t"
        "or $a2, $v1, $zero\n\t"
        "lw $ra, 0x100($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x110\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

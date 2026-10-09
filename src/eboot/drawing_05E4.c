/**
 * The Sims 2 PSP - drawing_05E4 (0x1BB864, 0x19C bytes)
 *
 *     addiu $sp, $sp, -0x40
 *     swc1 $f20, 0x20($sp)
 *     sw $s0, 0x24($sp)
 *     sw $s1, 0x28($sp)
 *     sw $s2, 0x2C($sp)
 *     mov.s $f20, $f12
 *     or $s2, $a3, $zero
 *     or $s1, $a2, $zero
 *     or $s0, $a1, $zero
 *     sw $s3, 0x30($sp)
 *     sw $s4, 0x34($sp)
 *     sw $s5, 0x38($sp)
 *     sw $ra, 0x3C($sp)
 *     jal drawing_051C
 *     or $s5, $a0, $zero
 *     lui $s3, %%hi(sym_001DACC0)
 *     addiu $s3, $s3, %%lo(sym_001DACC0)
 *     jal func_00102954
 *     or $a0, $s3, $zero
 *     lwc1 $f12, 0x0($s5)
 *     swc1 $f12, 0x230($s3)
 *     lwc1 $f12, 0x4($s5)
 *     swc1 $f12, 0x234($s3)
 *     lwc1 $f12, 0x8($s5)
 *     swc1 $f12, 0x238($s3)
 *     lwc1 $f12, 0xC($s5)
 *     lui $a0, 0x3DCC
 *     ori $a0, $a0, (0x3DCCCCCD & 0xFFFF)
 *     swc1 $f12, 0x23C($s3)
 *     mtc1 $a0, $f12
 *     andi $s4, $s1, 0x2
 *     lw $a1, 0x270($s3)
 *     lui $a0, %%hi(D_FFF0FFFF)
 *     beqz $s4, .Leboot_001BB900
 *     addiu $a0, $a0, %%lo(D_FFF0FFFF)
 *     sw $zero, 0x26C($s3)
 *     sw $zero, 0x268($s3)
 *     b .Leboot_001BB914
 *     swc1 $f12, 0x264($s3)
 *   .Leboot_001BB900
 *     ori $a2, $zero, 0x4
 *     sw $a2, 0x26C($s3)
 *     ori $a2, $zero, 0x7
 *     sw $a2, 0x268($s3)
 *     swc1 $f12, 0x264($s3)
 *   .Leboot_001BB914
 *     addiu $a2, $zero, -0x1001
 *     and $a1, $a1, $a2
 *     sw $a1, 0x270($s3)
 *     addiu $a2, $zero, -0xF1
 *     bnez $s2, .Leboot_001BB940
 *     and $a1, $a1, $a2
 *     sw $a1, 0x270($s3)
 *     ori $a1, $a1, 0x10
 *     sw $a1, 0x270($s3)
 *     b .Leboot_001BB950
 *     and $a1, $a1, $a0
 *   .Leboot_001BB940
 *     sw $a1, 0x270($s3)
 *     or $a1, $a1, $s2
 *     sw $a1, 0x270($s3)
 *     and $a1, $a1, $a0
 *   .Leboot_001BB950
 *     sw $a1, 0x270($s3)
 *     lui $a0, 0x1
 *     or $a1, $a1, $a0
 *     andi $a0, $s1, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BB978
 *     sw $a1, 0x270($s3)
 *     ori $a0, $a1, (0x10100 & 0xFFFF)
 *     b .Leboot_001BB984
 *     sw $a0, 0x270($s3)
 *   .Leboot_001BB978
 *     addiu $a0, $zero, -0x101
 *     and $a0, $a1, $a0
 *     sw $a0, 0x270($s3)
 *   .Leboot_001BB984
 *     ori $s2, $zero, 0x1
 *     lui $t0, 0xF
 *     sw $s2, 0x274($s3)
 *     and $t0, $s1, $t0
 *     or $a0, $s3, $zero
 *     or $a1, $zero, $zero
 *     or $a2, $s0, $zero
 *     jal func_001029C8
 *     ori $a3, $zero, 0x1
 *     sh $s2, 0x278($s3)
 *     ori $s0, $zero, 0x4
 *     bnel $s4, $zero, .Leboot_001BB9B8
 *     ori $s0, $zero, 0x0
 *   .Leboot_001BB9B8
 *     lui $a0, %%hi(D_C9010100)
 *     sw $s0, 0x18($s3)
 *     addiu $a0, $a0, %%lo(D_C9010100)
 *     sw $a0, 0x1C($s3)
 *     swc1 $f20, 0xC($s3)
 *     or $a0, $s3, $zero
 *     jal renderCommon_1618
 *     or $a1, $zero, $zero
 *     lwc1 $f20, 0x20($sp)
 *     lw $s0, 0x24($sp)
 *     lw $s1, 0x28($sp)
 *     lw $s2, 0x2C($sp)
 *     lw $s3, 0x30($sp)
 *     lw $s4, 0x34($sp)
 *     lw $s5, 0x38($sp)
 *     lw $ra, 0x3C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x40
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_05E4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "swc1 $f20, 0x20($sp)\n\t"
        "sw $s0, 0x24($sp)\n\t"
        "sw $s1, 0x28($sp)\n\t"
        "sw $s2, 0x2C($sp)\n\t"
        "mov.s $f20, $f12\n\t"
        "or $s2, $a3, $zero\n\t"
        "or $s1, $a2, $zero\n\t"
        "or $s0, $a1, $zero\n\t"
        "sw $s3, 0x30($sp)\n\t"
        "sw $s4, 0x34($sp)\n\t"
        "sw $s5, 0x38($sp)\n\t"
        "sw $ra, 0x3C($sp)\n\t"
        "jal drawing_051C\n\t"
        "or $s5, $a0, $zero\n\t"
        "lui $s3, %%hi(sym_001DACC0)\n\t"
        "addiu $s3, $s3, %%lo(sym_001DACC0)\n\t"
        "jal func_00102954\n\t"
        "or $a0, $s3, $zero\n\t"
        "lwc1 $f12, 0x0($s5)\n\t"
        "swc1 $f12, 0x230($s3)\n\t"
        "lwc1 $f12, 0x4($s5)\n\t"
        "swc1 $f12, 0x234($s3)\n\t"
        "lwc1 $f12, 0x8($s5)\n\t"
        "swc1 $f12, 0x238($s3)\n\t"
        "lwc1 $f12, 0xC($s5)\n\t"
        "lui $a0, 0x3DCC\n\t"
        "ori $a0, $a0, (0x3DCCCCCD & 0xFFFF)\n\t"
        "swc1 $f12, 0x23C($s3)\n\t"
        "mtc1 $a0, $f12\n\t"
        "andi $s4, $s1, 0x2\n\t"
        "lw $a1, 0x270($s3)\n\t"
        "lui $a0, %%hi(D_FFF0FFFF)\n\t"
        "beqz $s4, .Leboot_001BB900\n\t"
        "addiu $a0, $a0, %%lo(D_FFF0FFFF)\n\t"
        "sw $zero, 0x26C($s3)\n\t"
        "sw $zero, 0x268($s3)\n\t"
        "b .Leboot_001BB914\n\t"
        "swc1 $f12, 0x264($s3)\n\t"
        ".Leboot_001BB900:\n\t"
        "ori $a2, $zero, 0x4\n\t"
        "sw $a2, 0x26C($s3)\n\t"
        "ori $a2, $zero, 0x7\n\t"
        "sw $a2, 0x268($s3)\n\t"
        "swc1 $f12, 0x264($s3)\n\t"
        ".Leboot_001BB914:\n\t"
        "addiu $a2, $zero, -0x1001\n\t"
        "and $a1, $a1, $a2\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "addiu $a2, $zero, -0xF1\n\t"
        "bnez $s2, .Leboot_001BB940\n\t"
        "and $a1, $a1, $a2\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "ori $a1, $a1, 0x10\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "b .Leboot_001BB950\n\t"
        "and $a1, $a1, $a0\n\t"
        ".Leboot_001BB940:\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "or $a1, $a1, $s2\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "and $a1, $a1, $a0\n\t"
        ".Leboot_001BB950:\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "lui $a0, 0x1\n\t"
        "or $a1, $a1, $a0\n\t"
        "andi $a0, $s1, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BB978\n\t"
        "sw $a1, 0x270($s3)\n\t"
        "ori $a0, $a1, (0x10100 & 0xFFFF)\n\t"
        "b .Leboot_001BB984\n\t"
        "sw $a0, 0x270($s3)\n\t"
        ".Leboot_001BB978:\n\t"
        "addiu $a0, $zero, -0x101\n\t"
        "and $a0, $a1, $a0\n\t"
        "sw $a0, 0x270($s3)\n\t"
        ".Leboot_001BB984:\n\t"
        "ori $s2, $zero, 0x1\n\t"
        "lui $t0, 0xF\n\t"
        "sw $s2, 0x274($s3)\n\t"
        "and $t0, $s1, $t0\n\t"
        "or $a0, $s3, $zero\n\t"
        "or $a1, $zero, $zero\n\t"
        "or $a2, $s0, $zero\n\t"
        "jal func_001029C8\n\t"
        "ori $a3, $zero, 0x1\n\t"
        "sh $s2, 0x278($s3)\n\t"
        "ori $s0, $zero, 0x4\n\t"
        "bnel $s4, $zero, .Leboot_001BB9B8\n\t"
        "ori $s0, $zero, 0x0\n\t"
        ".Leboot_001BB9B8:\n\t"
        "lui $a0, %%hi(D_C9010100)\n\t"
        "sw $s0, 0x18($s3)\n\t"
        "addiu $a0, $a0, %%lo(D_C9010100)\n\t"
        "sw $a0, 0x1C($s3)\n\t"
        "swc1 $f20, 0xC($s3)\n\t"
        "or $a0, $s3, $zero\n\t"
        "jal renderCommon_1618\n\t"
        "or $a1, $zero, $zero\n\t"
        "lwc1 $f20, 0x20($sp)\n\t"
        "lw $s0, 0x24($sp)\n\t"
        "lw $s1, 0x28($sp)\n\t"
        "lw $s2, 0x2C($sp)\n\t"
        "lw $s3, 0x30($sp)\n\t"
        "lw $s4, 0x34($sp)\n\t"
        "lw $s5, 0x38($sp)\n\t"
        "lw $ra, 0x3C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

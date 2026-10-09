/**
 * The Sims 2 PSP - sortAndCullScene_0B44 (0x1B4760, 0x338 bytes)
 *
 *     addiu $sp, $sp, -0xE0
 *     sw $s7, 0xD4($sp)
 *     or $s7, $a0, $zero
 *     lw $a0, 0x78($s7)
 *     sw $a1, 0x20($sp)
 *     addiu $a0, $a0, 0x18
 *     lh $a1, 0x0($a0)
 *     lw $a2, 0x4($a0)
 *     addu $a0, $s7, $a1
 *     sw $s0, 0xB8($sp)
 *     sw $s1, 0xBC($sp)
 *     sw $s2, 0xC0($sp)
 *     sw $s3, 0xC4($sp)
 *     sw $s4, 0xC8($sp)
 *     sw $s5, 0xCC($sp)
 *     sw $s6, 0xD0($sp)
 *     sw $fp, 0xD8($sp)
 *     sw $ra, 0xDC($sp)
 *     jalr $a2
 *     addiu $a1, $sp, 0x20
 *     sw $v0, 0xAC($sp)
 *     slti $a0, $v0, 0x2
 *     lw $s0, 0x38($s7)
 *     addiu $s1, $s7, 0x3C
 *     addiu $s2, $sp, 0x24
 *     addiu $s5, $sp, 0x3C
 *     beqz $a0, .Leboot_001B47D8
 *     addiu $s6, $sp, 0x54
 *     ori $a0, $zero, 0x1
 *     sw $a0, 0xAC($sp)
 *   .Leboot_001B47D8
 *     sll $a0, $s0, 2
 *     sw $s0, 0xB4($sp)
 *     sb $zero, 0x91($sp)
 *     addu $s0, $s7, $a0
 *     lb $a0, 0x91($sp)
 *     sb $zero, 0x94($sp)
 *     lb $a1, 0x94($sp)
 *     sb $a0, 0x90($sp)
 *     addiu $s0, $s0, 0x3C
 *     jal func_001A3D98
 *     sb $a1, 0x95($sp)
 *     sw $v0, 0xA4($sp)
 *     lb $a0, 0xA4($sp)
 *     bne $s0, $s1, .Leboot_001B4824
 *     sb $a0, 0x93($sp)
 *     lw $s0, 0x78($s7)
 *     sw $s1, 0xB0($sp)
 *     b .Leboot_001B4840
 *     addiu $s0, $s0, 0x98
 *   .Leboot_001B4824
 *     sw $s1, 0xB0($sp)
 *     subu $a2, $s0, $s1
 *     or $a0, $s2, $zero
 *     jal func_00143770
 *     or $a1, $s1, $zero
 *     lw $s0, 0x78($s7)
 *     addiu $s0, $s0, 0x98
 *   .Leboot_001B4840
 *     sw $zero, 0x38($s7)
 *     addiu $a0, $s7, 0x60
 *     lwc1 $f12, 0x0($a0)
 *     addiu $a1, $s7, 0x6C
 *     swc1 $f12, 0x3C($sp)
 *     lwc1 $f12, 0x4($a0)
 *     swc1 $f12, 0x40($sp)
 *     lwc1 $f12, 0x8($a0)
 *     swc1 $f12, 0x44($sp)
 *     lwc1 $f12, 0x0($a1)
 *     addiu $a0, $sp, 0x48
 *     swc1 $f12, 0x48($sp)
 *     lwc1 $f12, 0x4($a1)
 *     swc1 $f12, 0x4C($sp)
 *     lwc1 $f12, 0x8($a1)
 *     lwc1 $f13, 0x0($s5)
 *     swc1 $f12, 0x50($sp)
 *     lwc1 $f12, 0x0($a0)
 *     lwc1 $f14, 0x4($s5)
 *     lwc1 $f15, 0x4($a0)
 *     add.s $f12, $f13, $f12
 *     lwc1 $f16, 0x8($s5)
 *     lwc1 $f17, 0x8($a0)
 *     add.s $f14, $f14, $f15
 *     addiu $a0, $sp, 0x98
 *     add.s $f16, $f16, $f17
 *     swc1 $f12, 0x98($sp)
 *     lui $a1, 0x3F00
 *     swc1 $f14, 0x9C($sp)
 *     mtc1 $a1, $f13
 *     swc1 $f16, 0xA0($sp)
 *     lwc1 $f12, 0x0($a0)
 *     lwc1 $f14, 0x4($a0)
 *     mul.s $f12, $f12, $f13
 *     lwc1 $f15, 0x8($a0)
 *     mul.s $f14, $f14, $f13
 *     swc1 $f12, 0x54($sp)
 *     mul.s $f13, $f15, $f13
 *     swc1 $f14, 0x58($sp)
 *     swc1 $f13, 0x5C($sp)
 *     lh $a0, 0x0($s0)
 *     lw $a1, 0x4($s0)
 *     jalr $a1
 *     addu $a0, $s7, $a0
 *     lw $a0, 0xAC($sp)
 *     ori $a1, $zero, 0x0
 *     or $s4, $v0, $zero
 *     slt $a0, $a1, $a0
 *     beqz $a0, .Leboot_001B4A0C
 *     sw $a1, 0xA8($sp)
 *     addiu $s3, $sp, 0x80
 *     addiu $s2, $sp, 0x60
 *     ori $fp, $zero, 0x0
 *   .Leboot_001B4914
 *     lw $a0, 0x20($sp)
 *     addu $a0, $a0, $fp
 *     lw $s1, 0x0($a0)
 *     addiu $s1, $s1, 0xA8
 *     lw $a0, 0x0($s1)
 *     sw $s1, 0x78($sp)
 *     xor $a1, $a0, $s1
 *     sltu $a1, $zero, $a1
 *     andi $a1, $a1, 0xFF
 *     beqz $a1, .Leboot_001B49F0
 *     sw $a0, 0x7C($sp)
 *   .Leboot_001B4940
 *     lw $s0, 0x8($a0)
 *     beql $s0, $zero, .Leboot_001B49D0
 *     lw $a0, 0x0($a0)
 *     lb $a1, 0x38($s0)
 *     andi $a1, $a1, 0x1
 *     beql $a1, $zero, .Leboot_001B49D0
 *     lw $a0, 0x0($a0)
 *     beqz $s4, .Leboot_001B496C
 *     lb $a1, 0x38($s0)
 *     b .Leboot_001B4970
 *     andi $a1, $a1, 0x8
 *   .Leboot_001B496C
 *     andi $a1, $a1, 0x4
 *   .Leboot_001B4970
 *     bnez $a1, .Leboot_001B49CC
 *     addiu $a1, $s0, 0x20
 *     lwc1 $f12, 0x0($a1)
 *     swc1 $f12, 0x80($sp)
 *     lwc1 $f12, 0x4($a1)
 *     or $a0, $s5, $zero
 *     swc1 $f12, 0x84($sp)
 *     lwc1 $f12, 0x8($a1)
 *     swc1 $f12, 0x88($sp)
 *     lwc1 $f12, 0xC($a1)
 *     or $a1, $s3, $zero
 *     jal sortAndCullScene_1950
 *     swc1 $f12, 0x8C($sp)
 *     beqz $v0, .Leboot_001B49C8
 *     nop
 *     or $a0, $s7, $zero
 *     or $a1, $s0, $zero
 *     or $a2, $s6, $zero
 *     or $a3, $zero, $zero
 *     ori $t0, $zero, 0x6
 *     jal func_000CEEE8
 *     or $t1, $s2, $zero
 *   .Leboot_001B49C8
 *     lw $a0, 0x7C($sp)
 *   .Leboot_001B49CC
 *     lw $a0, 0x0($a0)
 *   .Leboot_001B49D0
 *     sw $s1, 0x78($sp)
 *     sw $a0, 0x7C($sp)
 *     lw $a0, 0x7C($sp)
 *     xor $a1, $a0, $s1
 *     sltu $a1, $zero, $a1
 *     andi $a1, $a1, 0xFF
 *     bnez $a1, .Leboot_001B4940
 *     nop
 *   .Leboot_001B49F0
 *     lw $a0, 0xA8($sp)
 *     lw $a1, 0xAC($sp)
 *     addiu $a0, $a0, 0x1
 *     addiu $fp, $fp, 0x4
 *     slt $a1, $a0, $a1
 *     bnez $a1, .Leboot_001B4914
 *     sw $a0, 0xA8($sp)
 *   .Leboot_001B4A0C
 *     lw $a0, 0x38($s7)
 *     lw $a1, 0xB4($sp)
 *     bne $a0, $a1, .Leboot_001B4A5C
 *     sll $a0, $a0, 2
 *     lw $a1, 0xB0($sp)
 *     addu $a0, $s7, $a0
 *     addiu $a0, $a0, 0x3C
 *     beq $a1, $a0, .Leboot_001B4A50
 *     addiu $a2, $sp, 0x24
 *   .Leboot_001B4A30
 *     lw $a3, 0x0($a1)
 *     lw $t0, 0x0($a2)
 *     beq $a3, $t0, .Leboot_001B4A48
 *     addiu $a1, $a1, 0x4
 *     b .Leboot_001B4A54
 *     ori $a0, $zero, 0x0
 *   .Leboot_001B4A48
 *     bne $a1, $a0, .Leboot_001B4A30
 *     addiu $a2, $a2, 0x4
 *   .Leboot_001B4A50
 *     ori $a0, $zero, 0x1
 *   .Leboot_001B4A54
 *     bnez $a0, .Leboot_001B4A68
 *     nop
 *   .Leboot_001B4A5C
 *     lw $a0, 0x5C($s7)
 *     ori $a0, $a0, 0x2
 *     sw $a0, 0x5C($s7)
 *   .Leboot_001B4A68
 *     lw $s0, 0xB8($sp)
 *     lw $s1, 0xBC($sp)
 *     lw $s2, 0xC0($sp)
 *     lw $s3, 0xC4($sp)
 *     lw $s4, 0xC8($sp)
 *     lw $s5, 0xCC($sp)
 *     lw $s6, 0xD0($sp)
 *     lw $s7, 0xD4($sp)
 *     lw $fp, 0xD8($sp)
 *     lw $ra, 0xDC($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0xE0
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_0B44(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0xE0\n\t"
        "sw $s7, 0xD4($sp)\n\t"
        "or $s7, $a0, $zero\n\t"
        "lw $a0, 0x78($s7)\n\t"
        "sw $a1, 0x20($sp)\n\t"
        "addiu $a0, $a0, 0x18\n\t"
        "lh $a1, 0x0($a0)\n\t"
        "lw $a2, 0x4($a0)\n\t"
        "addu $a0, $s7, $a1\n\t"
        "sw $s0, 0xB8($sp)\n\t"
        "sw $s1, 0xBC($sp)\n\t"
        "sw $s2, 0xC0($sp)\n\t"
        "sw $s3, 0xC4($sp)\n\t"
        "sw $s4, 0xC8($sp)\n\t"
        "sw $s5, 0xCC($sp)\n\t"
        "sw $s6, 0xD0($sp)\n\t"
        "sw $fp, 0xD8($sp)\n\t"
        "sw $ra, 0xDC($sp)\n\t"
        "jalr $a2\n\t"
        "addiu $a1, $sp, 0x20\n\t"
        "sw $v0, 0xAC($sp)\n\t"
        "slti $a0, $v0, 0x2\n\t"
        "lw $s0, 0x38($s7)\n\t"
        "addiu $s1, $s7, 0x3C\n\t"
        "addiu $s2, $sp, 0x24\n\t"
        "addiu $s5, $sp, 0x3C\n\t"
        "beqz $a0, .Leboot_001B47D8\n\t"
        "addiu $s6, $sp, 0x54\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "sw $a0, 0xAC($sp)\n\t"
        ".Leboot_001B47D8:\n\t"
        "sll $a0, $s0, 2\n\t"
        "sw $s0, 0xB4($sp)\n\t"
        "sb $zero, 0x91($sp)\n\t"
        "addu $s0, $s7, $a0\n\t"
        "lb $a0, 0x91($sp)\n\t"
        "sb $zero, 0x94($sp)\n\t"
        "lb $a1, 0x94($sp)\n\t"
        "sb $a0, 0x90($sp)\n\t"
        "addiu $s0, $s0, 0x3C\n\t"
        "jal func_001A3D98\n\t"
        "sb $a1, 0x95($sp)\n\t"
        "sw $v0, 0xA4($sp)\n\t"
        "lb $a0, 0xA4($sp)\n\t"
        "bne $s0, $s1, .Leboot_001B4824\n\t"
        "sb $a0, 0x93($sp)\n\t"
        "lw $s0, 0x78($s7)\n\t"
        "sw $s1, 0xB0($sp)\n\t"
        "b .Leboot_001B4840\n\t"
        "addiu $s0, $s0, 0x98\n\t"
        ".Leboot_001B4824:\n\t"
        "sw $s1, 0xB0($sp)\n\t"
        "subu $a2, $s0, $s1\n\t"
        "or $a0, $s2, $zero\n\t"
        "jal func_00143770\n\t"
        "or $a1, $s1, $zero\n\t"
        "lw $s0, 0x78($s7)\n\t"
        "addiu $s0, $s0, 0x98\n\t"
        ".Leboot_001B4840:\n\t"
        "sw $zero, 0x38($s7)\n\t"
        "addiu $a0, $s7, 0x60\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "addiu $a1, $s7, 0x6C\n\t"
        "swc1 $f12, 0x3C($sp)\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "swc1 $f12, 0x40($sp)\n\t"
        "lwc1 $f12, 0x8($a0)\n\t"
        "swc1 $f12, 0x44($sp)\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "addiu $a0, $sp, 0x48\n\t"
        "swc1 $f12, 0x48($sp)\n\t"
        "lwc1 $f12, 0x4($a1)\n\t"
        "swc1 $f12, 0x4C($sp)\n\t"
        "lwc1 $f12, 0x8($a1)\n\t"
        "lwc1 $f13, 0x0($s5)\n\t"
        "swc1 $f12, 0x50($sp)\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f14, 0x4($s5)\n\t"
        "lwc1 $f15, 0x4($a0)\n\t"
        "add.s $f12, $f13, $f12\n\t"
        "lwc1 $f16, 0x8($s5)\n\t"
        "lwc1 $f17, 0x8($a0)\n\t"
        "add.s $f14, $f14, $f15\n\t"
        "addiu $a0, $sp, 0x98\n\t"
        "add.s $f16, $f16, $f17\n\t"
        "swc1 $f12, 0x98($sp)\n\t"
        "lui $a1, 0x3F00\n\t"
        "swc1 $f14, 0x9C($sp)\n\t"
        "mtc1 $a1, $f13\n\t"
        "swc1 $f16, 0xA0($sp)\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f14, 0x4($a0)\n\t"
        "mul.s $f12, $f12, $f13\n\t"
        "lwc1 $f15, 0x8($a0)\n\t"
        "mul.s $f14, $f14, $f13\n\t"
        "swc1 $f12, 0x54($sp)\n\t"
        "mul.s $f13, $f15, $f13\n\t"
        "swc1 $f14, 0x58($sp)\n\t"
        "swc1 $f13, 0x5C($sp)\n\t"
        "lh $a0, 0x0($s0)\n\t"
        "lw $a1, 0x4($s0)\n\t"
        "jalr $a1\n\t"
        "addu $a0, $s7, $a0\n\t"
        "lw $a0, 0xAC($sp)\n\t"
        "ori $a1, $zero, 0x0\n\t"
        "or $s4, $v0, $zero\n\t"
        "slt $a0, $a1, $a0\n\t"
        "beqz $a0, .Leboot_001B4A0C\n\t"
        "sw $a1, 0xA8($sp)\n\t"
        "addiu $s3, $sp, 0x80\n\t"
        "addiu $s2, $sp, 0x60\n\t"
        "ori $fp, $zero, 0x0\n\t"
        ".Leboot_001B4914:\n\t"
        "lw $a0, 0x20($sp)\n\t"
        "addu $a0, $a0, $fp\n\t"
        "lw $s1, 0x0($a0)\n\t"
        "addiu $s1, $s1, 0xA8\n\t"
        "lw $a0, 0x0($s1)\n\t"
        "sw $s1, 0x78($sp)\n\t"
        "xor $a1, $a0, $s1\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "beqz $a1, .Leboot_001B49F0\n\t"
        "sw $a0, 0x7C($sp)\n\t"
        ".Leboot_001B4940:\n\t"
        "lw $s0, 0x8($a0)\n\t"
        "beql $s0, $zero, .Leboot_001B49D0\n\t"
        "lw $a0, 0x0($a0)\n\t"
        "lb $a1, 0x38($s0)\n\t"
        "andi $a1, $a1, 0x1\n\t"
        "beql $a1, $zero, .Leboot_001B49D0\n\t"
        "lw $a0, 0x0($a0)\n\t"
        "beqz $s4, .Leboot_001B496C\n\t"
        "lb $a1, 0x38($s0)\n\t"
        "b .Leboot_001B4970\n\t"
        "andi $a1, $a1, 0x8\n\t"
        ".Leboot_001B496C:\n\t"
        "andi $a1, $a1, 0x4\n\t"
        ".Leboot_001B4970:\n\t"
        "bnez $a1, .Leboot_001B49CC\n\t"
        "addiu $a1, $s0, 0x20\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "swc1 $f12, 0x80($sp)\n\t"
        "lwc1 $f12, 0x4($a1)\n\t"
        "or $a0, $s5, $zero\n\t"
        "swc1 $f12, 0x84($sp)\n\t"
        "lwc1 $f12, 0x8($a1)\n\t"
        "swc1 $f12, 0x88($sp)\n\t"
        "lwc1 $f12, 0xC($a1)\n\t"
        "or $a1, $s3, $zero\n\t"
        "jal sortAndCullScene_1950\n\t"
        "swc1 $f12, 0x8C($sp)\n\t"
        "beqz $v0, .Leboot_001B49C8\n\t"
        "nop\n\t"
        "or $a0, $s7, $zero\n\t"
        "or $a1, $s0, $zero\n\t"
        "or $a2, $s6, $zero\n\t"
        "or $a3, $zero, $zero\n\t"
        "ori $t0, $zero, 0x6\n\t"
        "jal func_000CEEE8\n\t"
        "or $t1, $s2, $zero\n\t"
        ".Leboot_001B49C8:\n\t"
        "lw $a0, 0x7C($sp)\n\t"
        ".Leboot_001B49CC:\n\t"
        "lw $a0, 0x0($a0)\n\t"
        ".Leboot_001B49D0:\n\t"
        "sw $s1, 0x78($sp)\n\t"
        "sw $a0, 0x7C($sp)\n\t"
        "lw $a0, 0x7C($sp)\n\t"
        "xor $a1, $a0, $s1\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "bnez $a1, .Leboot_001B4940\n\t"
        "nop\n\t"
        ".Leboot_001B49F0:\n\t"
        "lw $a0, 0xA8($sp)\n\t"
        "lw $a1, 0xAC($sp)\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "addiu $fp, $fp, 0x4\n\t"
        "slt $a1, $a0, $a1\n\t"
        "bnez $a1, .Leboot_001B4914\n\t"
        "sw $a0, 0xA8($sp)\n\t"
        ".Leboot_001B4A0C:\n\t"
        "lw $a0, 0x38($s7)\n\t"
        "lw $a1, 0xB4($sp)\n\t"
        "bne $a0, $a1, .Leboot_001B4A5C\n\t"
        "sll $a0, $a0, 2\n\t"
        "lw $a1, 0xB0($sp)\n\t"
        "addu $a0, $s7, $a0\n\t"
        "addiu $a0, $a0, 0x3C\n\t"
        "beq $a1, $a0, .Leboot_001B4A50\n\t"
        "addiu $a2, $sp, 0x24\n\t"
        ".Leboot_001B4A30:\n\t"
        "lw $a3, 0x0($a1)\n\t"
        "lw $t0, 0x0($a2)\n\t"
        "beq $a3, $t0, .Leboot_001B4A48\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "b .Leboot_001B4A54\n\t"
        "ori $a0, $zero, 0x0\n\t"
        ".Leboot_001B4A48:\n\t"
        "bne $a1, $a0, .Leboot_001B4A30\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        ".Leboot_001B4A50:\n\t"
        "ori $a0, $zero, 0x1\n\t"
        ".Leboot_001B4A54:\n\t"
        "bnez $a0, .Leboot_001B4A68\n\t"
        "nop\n\t"
        ".Leboot_001B4A5C:\n\t"
        "lw $a0, 0x5C($s7)\n\t"
        "ori $a0, $a0, 0x2\n\t"
        "sw $a0, 0x5C($s7)\n\t"
        ".Leboot_001B4A68:\n\t"
        "lw $s0, 0xB8($sp)\n\t"
        "lw $s1, 0xBC($sp)\n\t"
        "lw $s2, 0xC0($sp)\n\t"
        "lw $s3, 0xC4($sp)\n\t"
        "lw $s4, 0xC8($sp)\n\t"
        "lw $s5, 0xCC($sp)\n\t"
        "lw $s6, 0xD0($sp)\n\t"
        "lw $s7, 0xD4($sp)\n\t"
        "lw $fp, 0xD8($sp)\n\t"
        "lw $ra, 0xDC($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0xE0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

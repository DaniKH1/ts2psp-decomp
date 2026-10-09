/**
 * The Sims 2 PSP - renderMeshInstances_0B1C (0x1BE51C, 0x100 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s2, 0x18($sp)
 *     andi $s2, $a2, 0xFF
 *     lbu $a2, 0x11C($a0)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x1C($sp)
 *     bnez $a2, .Leboot_001BE54C
 *     or $s0, $a1, $zero
 *     jal sortAndCullScene_07CC
 *     or $a0, $s1, $zero
 *   .Leboot_001BE54C
 *     lbu $a0, 0x11C($s1)
 *     beqz $a0, .Leboot_001BE570
 *     nop
 *     andi $a0, $s2, 0xFF
 *     beqz $a0, .Leboot_001BE578
 *     lw $s2, 0x3F0($s1)
 *     ori $a0, $s2, 0x100
 *     b .Leboot_001BE584
 *     sw $a0, 0x3F0($s1)
 *   .Leboot_001BE570
 *     b .Leboot_001BE604
 *     nop
 *   .Leboot_001BE578
 *     addiu $a0, $zero, -0x101
 *     and $a0, $s2, $a0
 *     sw $a0, 0x3F0($s1)
 *   .Leboot_001BE584
 *     lw $a0, 0x148($s1)
 *     addiu $a1, $s1, 0x120
 *     xor $a0, $a1, $a0
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BE5BC
 *     sw $zero, 0x150($s1)
 *     lw $a0, 0x158($s1)
 *     lw $a1, 0x154($s1)
 *     sh $a0, 0x3F8($s1)
 *     sw $a1, 0x3F4($s1)
 *     lw $a0, 0x4($s0)
 *     b .Leboot_001BE5CC
 *     addiu $a0, $a0, 0x28
 *   .Leboot_001BE5BC
 *     jal func_000AC498
 *     or $a0, $s1, $zero
 *     lw $a0, 0x4($s0)
 *     addiu $a0, $a0, 0x28
 *   .Leboot_001BE5CC
 *     jal func_00102C84
 *     nop
 *     lw $a1, 0x4($s0)
 *     addiu $a0, $s1, 0x180
 *     lw $a1, 0x10($a1)
 *     xori $a1, $a1, 0x1
 *     sltiu $a1, $a1, 0x1
 *     jal renderCommon_1618
 *     andi $a1, $a1, 0xFF
 *     jal renderCommon_0F7C
 *     or $a0, $s0, $zero
 *     or $a0, $s1, $zero
 *     jal renderMeshInstances_0C80
 *     or $a1, $s0, $zero
 *   .Leboot_001BE604
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $ra, 0x1C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0B1C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "andi $s2, $a2, 0xFF\n\t"
        "lbu $a2, 0x11C($a0)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x1C($sp)\n\t"
        "bnez $a2, .Leboot_001BE54C\n\t"
        "or $s0, $a1, $zero\n\t"
        "jal sortAndCullScene_07CC\n\t"
        "or $a0, $s1, $zero\n\t"
        ".Leboot_001BE54C:\n\t"
        "lbu $a0, 0x11C($s1)\n\t"
        "beqz $a0, .Leboot_001BE570\n\t"
        "nop\n\t"
        "andi $a0, $s2, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE578\n\t"
        "lw $s2, 0x3F0($s1)\n\t"
        "ori $a0, $s2, 0x100\n\t"
        "b .Leboot_001BE584\n\t"
        "sw $a0, 0x3F0($s1)\n\t"
        ".Leboot_001BE570:\n\t"
        "b .Leboot_001BE604\n\t"
        "nop\n\t"
        ".Leboot_001BE578:\n\t"
        "addiu $a0, $zero, -0x101\n\t"
        "and $a0, $s2, $a0\n\t"
        "sw $a0, 0x3F0($s1)\n\t"
        ".Leboot_001BE584:\n\t"
        "lw $a0, 0x148($s1)\n\t"
        "addiu $a1, $s1, 0x120\n\t"
        "xor $a0, $a1, $a0\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE5BC\n\t"
        "sw $zero, 0x150($s1)\n\t"
        "lw $a0, 0x158($s1)\n\t"
        "lw $a1, 0x154($s1)\n\t"
        "sh $a0, 0x3F8($s1)\n\t"
        "sw $a1, 0x3F4($s1)\n\t"
        "lw $a0, 0x4($s0)\n\t"
        "b .Leboot_001BE5CC\n\t"
        "addiu $a0, $a0, 0x28\n\t"
        ".Leboot_001BE5BC:\n\t"
        "jal func_000AC498\n\t"
        "or $a0, $s1, $zero\n\t"
        "lw $a0, 0x4($s0)\n\t"
        "addiu $a0, $a0, 0x28\n\t"
        ".Leboot_001BE5CC:\n\t"
        "jal func_00102C84\n\t"
        "nop\n\t"
        "lw $a1, 0x4($s0)\n\t"
        "addiu $a0, $s1, 0x180\n\t"
        "lw $a1, 0x10($a1)\n\t"
        "xori $a1, $a1, 0x1\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "jal renderCommon_1618\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "jal renderCommon_0F7C\n\t"
        "or $a0, $s0, $zero\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal renderMeshInstances_0C80\n\t"
        "or $a1, $s0, $zero\n\t"
        ".Leboot_001BE604:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $ra, 0x1C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

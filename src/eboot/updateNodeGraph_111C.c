/**
 * The Sims 2 PSP - updateNodeGraph_111C (0x1B699C, 0x11C bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s1, 0x24($sp)
 *     or $s1, $a0, $zero
 *     ori $a0, $zero, 0x1
 *     sw $s0, 0x20($sp)
 *     sw $s2, 0x28($sp)
 *     sb $a0, 0x84($s1)
 *     addiu $s2, $s1, 0x90
 *     or $s0, $a1, $zero
 *     sw $ra, 0x2C($sp)
 *     jal func_0012E348
 *     or $a0, $s2, $zero
 *     sltiu $a0, $v0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001B6AA0
 *     nop
 *     lw $a0, 0x0($s0)
 *     lw $a1, 0x4($s0)
 *     lw $a2, 0x8($s0)
 *     sw $a0, 0x0($s2)
 *     lw $a0, 0xC($s0)
 *     sw $a1, 0x4($s2)
 *     lw $a1, 0x10($s0)
 *     sw $a2, 0x8($s2)
 *     lw $a2, 0x14($s0)
 *     sw $a0, 0xC($s2)
 *     lw $a0, 0x18($s0)
 *     sw $a1, 0x10($s2)
 *     lw $a1, 0x1C($s0)
 *     sw $a2, 0x14($s2)
 *     lw $a2, 0x20($s0)
 *     sw $a0, 0x18($s2)
 *     lw $a0, 0x24($s0)
 *     sw $a1, 0x1C($s2)
 *     lw $a1, 0x28($s0)
 *     sw $a2, 0x20($s2)
 *     lw $a2, 0x2C($s0)
 *     sw $a0, 0x24($s2)
 *     lw $a0, 0x30($s0)
 *     sw $a1, 0x28($s2)
 *     lw $a1, 0x34($s0)
 *     sw $a2, 0x2C($s2)
 *     lw $a2, 0x38($s0)
 *     sw $a0, 0x30($s2)
 *     lw $a0, 0x3C($s0)
 *     sw $a1, 0x34($s2)
 *     sw $a2, 0x38($s2)
 *     sw $a0, 0x3C($s2)
 *     lw $a0, 0xC($s1)
 *     lui $a1, 0x4000
 *     or $a0, $a0, $a1
 *     lw $a1, 0x0($s1)
 *     sw $a0, 0xC($s1)
 *     lw $a0, 0x4($s1)
 *     lw $a2, 0x38($a1)
 *     sra $a3, $a0, 5
 *     sll $a3, $a3, 2
 *     addu $a2, $a2, $a3
 *     lw $a3, 0x0($a2)
 *     andi $a0, $a0, 0x1F
 *     ori $t0, $zero, 0x1
 *     sllv $a0, $t0, $a0
 *     or $a0, $a3, $a0
 *     sw $a0, 0x0($a2)
 *     sb $zero, 0x34($a1)
 *   .Leboot_001B6AA0
 *     lw $s0, 0x20($sp)
 *     lw $s1, 0x24($sp)
 *     lw $s2, 0x28($sp)
 *     lw $ra, 0x2C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_111C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s1, 0x24($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "sw $s0, 0x20($sp)\n\t"
        "sw $s2, 0x28($sp)\n\t"
        "sb $a0, 0x84($s1)\n\t"
        "addiu $s2, $s1, 0x90\n\t"
        "or $s0, $a1, $zero\n\t"
        "sw $ra, 0x2C($sp)\n\t"
        "jal func_0012E348\n\t"
        "or $a0, $s2, $zero\n\t"
        "sltiu $a0, $v0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001B6AA0\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s0)\n\t"
        "lw $a1, 0x4($s0)\n\t"
        "lw $a2, 0x8($s0)\n\t"
        "sw $a0, 0x0($s2)\n\t"
        "lw $a0, 0xC($s0)\n\t"
        "sw $a1, 0x4($s2)\n\t"
        "lw $a1, 0x10($s0)\n\t"
        "sw $a2, 0x8($s2)\n\t"
        "lw $a2, 0x14($s0)\n\t"
        "sw $a0, 0xC($s2)\n\t"
        "lw $a0, 0x18($s0)\n\t"
        "sw $a1, 0x10($s2)\n\t"
        "lw $a1, 0x1C($s0)\n\t"
        "sw $a2, 0x14($s2)\n\t"
        "lw $a2, 0x20($s0)\n\t"
        "sw $a0, 0x18($s2)\n\t"
        "lw $a0, 0x24($s0)\n\t"
        "sw $a1, 0x1C($s2)\n\t"
        "lw $a1, 0x28($s0)\n\t"
        "sw $a2, 0x20($s2)\n\t"
        "lw $a2, 0x2C($s0)\n\t"
        "sw $a0, 0x24($s2)\n\t"
        "lw $a0, 0x30($s0)\n\t"
        "sw $a1, 0x28($s2)\n\t"
        "lw $a1, 0x34($s0)\n\t"
        "sw $a2, 0x2C($s2)\n\t"
        "lw $a2, 0x38($s0)\n\t"
        "sw $a0, 0x30($s2)\n\t"
        "lw $a0, 0x3C($s0)\n\t"
        "sw $a1, 0x34($s2)\n\t"
        "sw $a2, 0x38($s2)\n\t"
        "sw $a0, 0x3C($s2)\n\t"
        "lw $a0, 0xC($s1)\n\t"
        "lui $a1, 0x4000\n\t"
        "or $a0, $a0, $a1\n\t"
        "lw $a1, 0x0($s1)\n\t"
        "sw $a0, 0xC($s1)\n\t"
        "lw $a0, 0x4($s1)\n\t"
        "lw $a2, 0x38($a1)\n\t"
        "sra $a3, $a0, 5\n\t"
        "sll $a3, $a3, 2\n\t"
        "addu $a2, $a2, $a3\n\t"
        "lw $a3, 0x0($a2)\n\t"
        "andi $a0, $a0, 0x1F\n\t"
        "ori $t0, $zero, 0x1\n\t"
        "sllv $a0, $t0, $a0\n\t"
        "or $a0, $a3, $a0\n\t"
        "sw $a0, 0x0($a2)\n\t"
        "sb $zero, 0x34($a1)\n\t"
        ".Leboot_001B6AA0:\n\t"
        "lw $s0, 0x20($sp)\n\t"
        "lw $s1, 0x24($sp)\n\t"
        "lw $s2, 0x28($sp)\n\t"
        "lw $ra, 0x2C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

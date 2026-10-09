/**
 * The Sims 2 PSP - updateNodeGraph_02A0 (0x1B5B20, 0xCC bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x10($sp)
 *     or $s0, $a1, $zero
 *     sw $s1, 0x14($sp)
 *     sw $ra, 0x18($sp)
 *     jal func_0012E348
 *     or $s1, $a0, $zero
 *     sltiu $a0, $v0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001B5BD8
 *     nop
 *     lw $a0, 0x0($s0)
 *     lw $a1, 0x4($s0)
 *     lw $a2, 0x8($s0)
 *     sw $a0, 0x0($s1)
 *     lw $a0, 0xC($s0)
 *     sw $a1, 0x4($s1)
 *     lw $a1, 0x10($s0)
 *     sw $a2, 0x8($s1)
 *     lw $a2, 0x14($s0)
 *     sw $a0, 0xC($s1)
 *     lw $a0, 0x18($s0)
 *     sw $a1, 0x10($s1)
 *     lw $a1, 0x1C($s0)
 *     sw $a2, 0x14($s1)
 *     lw $a2, 0x20($s0)
 *     sw $a0, 0x18($s1)
 *     lw $a0, 0x24($s0)
 *     sw $a1, 0x1C($s1)
 *     lw $a1, 0x28($s0)
 *     sw $a2, 0x20($s1)
 *     lw $a2, 0x2C($s0)
 *     sw $a0, 0x24($s1)
 *     lw $a0, 0x30($s0)
 *     sw $a1, 0x28($s1)
 *     lw $a1, 0x34($s0)
 *     sw $a2, 0x2C($s1)
 *     lw $a2, 0x38($s0)
 *     sw $a0, 0x30($s1)
 *     lw $a0, 0x3C($s0)
 *     sw $a1, 0x34($s1)
 *     sw $a2, 0x38($s1)
 *     sw $a0, 0x3C($s1)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_03A0
 *     lui $a1, 0x4000
 *   .Leboot_001B5BD8
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_02A0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "or $s0, $a1, $zero\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "jal func_0012E348\n\t"
        "or $s1, $a0, $zero\n\t"
        "sltiu $a0, $v0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001B5BD8\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s0)\n\t"
        "lw $a1, 0x4($s0)\n\t"
        "lw $a2, 0x8($s0)\n\t"
        "sw $a0, 0x0($s1)\n\t"
        "lw $a0, 0xC($s0)\n\t"
        "sw $a1, 0x4($s1)\n\t"
        "lw $a1, 0x10($s0)\n\t"
        "sw $a2, 0x8($s1)\n\t"
        "lw $a2, 0x14($s0)\n\t"
        "sw $a0, 0xC($s1)\n\t"
        "lw $a0, 0x18($s0)\n\t"
        "sw $a1, 0x10($s1)\n\t"
        "lw $a1, 0x1C($s0)\n\t"
        "sw $a2, 0x14($s1)\n\t"
        "lw $a2, 0x20($s0)\n\t"
        "sw $a0, 0x18($s1)\n\t"
        "lw $a0, 0x24($s0)\n\t"
        "sw $a1, 0x1C($s1)\n\t"
        "lw $a1, 0x28($s0)\n\t"
        "sw $a2, 0x20($s1)\n\t"
        "lw $a2, 0x2C($s0)\n\t"
        "sw $a0, 0x24($s1)\n\t"
        "lw $a0, 0x30($s0)\n\t"
        "sw $a1, 0x28($s1)\n\t"
        "lw $a1, 0x34($s0)\n\t"
        "sw $a2, 0x2C($s1)\n\t"
        "lw $a2, 0x38($s0)\n\t"
        "sw $a0, 0x30($s1)\n\t"
        "lw $a0, 0x3C($s0)\n\t"
        "sw $a1, 0x34($s1)\n\t"
        "sw $a2, 0x38($s1)\n\t"
        "sw $a0, 0x3C($s1)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_03A0\n\t"
        "lui $a1, 0x4000\n\t"
        ".Leboot_001B5BD8:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $ra, 0x18($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

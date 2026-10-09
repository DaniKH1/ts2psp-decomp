/**
 * The Sims 2 PSP - updateNodeGraph_1238 (0x1B6AB8, 0x10C bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lui $a2, 0x8000
 *     sw $s0, 0x10($sp)
 *     and $a2, $a1, $a2
 *     or $s0, $a0, $zero
 *     sw $s1, 0x14($sp)
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     sw $s4, 0x20($sp)
 *     sw $s5, 0x24($sp)
 *     sw $ra, 0x28($sp)
 *     beqz $a2, .Leboot_001B6B30
 *     or $s1, $a1, $zero
 *     ori $a0, $zero, 0x0
 *     ori $a1, $zero, 0x0
 *     or $a2, $s0, $zero
 *   .Leboot_001B6AF8
 *     lw $a3, 0x20($a2)
 *     beqz $a3, .Leboot_001B6B14
 *     nop
 *     lwc1 $f12, 0x0($a3)
 *     lw $a3, 0x8($s0)
 *     addu $a3, $a3, $a1
 *     swc1 $f12, 0x0($a3)
 *   .Leboot_001B6B14
 *     addiu $a0, $a0, 0x1
 *     addiu $a1, $a1, 0x4
 *     slti $a3, $a0, 0xA
 *     bnez $a3, .Leboot_001B6AF8
 *     addiu $a2, $a2, 0x4
 *     b .Leboot_001B6BA0
 *     nop
 *   .Leboot_001B6B30
 *     ori $s2, $zero, 0x0
 *     ori $s5, $zero, 0x1
 *     ori $s3, $zero, 0x0
 *     or $s4, $s0, $zero
 *   .Leboot_001B6B40
 *     lw $a0, 0x20($s4)
 *     beqz $a0, .Leboot_001B6B8C
 *     sllv $a1, $s5, $s2
 *     and $a1, $s1, $a1
 *     beqz $a1, .Leboot_001B6B8C
 *     nop
 *     lw $a1, 0x8($s0)
 *     addu $a1, $a1, $s3
 *     lwc1 $f12, 0x0($a1)
 *     lw $a1, 0x4($a0)
 *     swc1 $f12, 0x0($a0)
 *     lw $a2, 0x0($a1)
 *     xor $a1, $a2, $a1
 *     sltiu $a1, $a1, 0x1
 *     andi $a1, $a1, 0xFF
 *     bnez $a1, .Leboot_001B6B8C
 *     nop
 *     jal func_000D8798
 *     nop
 *   .Leboot_001B6B8C
 *     addiu $s2, $s2, 0x1
 *     addiu $s3, $s3, 0x4
 *     slti $a0, $s2, 0xA
 *     bnez $a0, .Leboot_001B6B40
 *     addiu $s4, $s4, 0x4
 *   .Leboot_001B6BA0
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $s4, 0x20($sp)
 *     lw $s5, 0x24($sp)
 *     lw $ra, 0x28($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_1238(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lui $a2, 0x8000\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "and $a2, $a1, $a2\n\t"
        "or $s0, $a0, $zero\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "sw $s4, 0x20($sp)\n\t"
        "sw $s5, 0x24($sp)\n\t"
        "sw $ra, 0x28($sp)\n\t"
        "beqz $a2, .Leboot_001B6B30\n\t"
        "or $s1, $a1, $zero\n\t"
        "ori $a0, $zero, 0x0\n\t"
        "ori $a1, $zero, 0x0\n\t"
        "or $a2, $s0, $zero\n\t"
        ".Leboot_001B6AF8:\n\t"
        "lw $a3, 0x20($a2)\n\t"
        "beqz $a3, .Leboot_001B6B14\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x0($a3)\n\t"
        "lw $a3, 0x8($s0)\n\t"
        "addu $a3, $a3, $a1\n\t"
        "swc1 $f12, 0x0($a3)\n\t"
        ".Leboot_001B6B14:\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "slti $a3, $a0, 0xA\n\t"
        "bnez $a3, .Leboot_001B6AF8\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        "b .Leboot_001B6BA0\n\t"
        "nop\n\t"
        ".Leboot_001B6B30:\n\t"
        "ori $s2, $zero, 0x0\n\t"
        "ori $s5, $zero, 0x1\n\t"
        "ori $s3, $zero, 0x0\n\t"
        "or $s4, $s0, $zero\n\t"
        ".Leboot_001B6B40:\n\t"
        "lw $a0, 0x20($s4)\n\t"
        "beqz $a0, .Leboot_001B6B8C\n\t"
        "sllv $a1, $s5, $s2\n\t"
        "and $a1, $s1, $a1\n\t"
        "beqz $a1, .Leboot_001B6B8C\n\t"
        "nop\n\t"
        "lw $a1, 0x8($s0)\n\t"
        "addu $a1, $a1, $s3\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "lw $a1, 0x4($a0)\n\t"
        "swc1 $f12, 0x0($a0)\n\t"
        "lw $a2, 0x0($a1)\n\t"
        "xor $a1, $a2, $a1\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "bnez $a1, .Leboot_001B6B8C\n\t"
        "nop\n\t"
        "jal func_000D8798\n\t"
        "nop\n\t"
        ".Leboot_001B6B8C:\n\t"
        "addiu $s2, $s2, 0x1\n\t"
        "addiu $s3, $s3, 0x4\n\t"
        "slti $a0, $s2, 0xA\n\t"
        "bnez $a0, .Leboot_001B6B40\n\t"
        "addiu $s4, $s4, 0x4\n\t"
        ".Leboot_001B6BA0:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $s4, 0x20($sp)\n\t"
        "lw $s5, 0x24($sp)\n\t"
        "lw $ra, 0x28($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

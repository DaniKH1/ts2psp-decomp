/**
 * The Sims 2 PSP - updateNodeGraph_0E74 (0x1B66F4, 0x138 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s1, 0x14($sp)
 *     sw $s2, 0x18($sp)
 *     andi $s2, $a1, 0x3FE
 *     or $s1, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x1C($sp)
 *     beqz $s2, .Leboot_001B6740
 *     or $s0, $a1, $zero
 *     lbu $a0, 0x84($s1)
 *     bnez $a0, .Leboot_001B6740
 *     nop
 *     lw $a0, 0x18($s1)
 *     addiu $a1, $s1, 0x90
 *     addiu $a0, $a0, 0xD0
 *     lh $a2, 0x0($a0)
 *     lw $a3, 0x4($a0)
 *     jalr $a3
 *     addu $a0, $s1, $a2
 *   .Leboot_001B6740
 *     lui $a0, %%hi(D_400003FE)
 *     addiu $a0, $a0, %%lo(D_400003FE)
 *     and $a0, $s0, $a0
 *     beqz $a0, .Leboot_001B67F0
 *     nop
 *     lw $a0, 0x14($s1)
 *     lbu $a0, 0x50($a0)
 *     addiu $a0, $a0, -0x3
 *     sltiu $a1, $a0, 0x7
 *     beqz $a1, sym_001B67DC
 *     nop
 *     sll $a0, $a0, 2
 *     lui $at, %%hi(sym_001CAE18)
 *     addu $at, $at, $a0
 *     lw $at, %%lo(sym_001CAE18)($at)
 *     jr $at
 *     nop
 *   sym_001B6784
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
 *     ori $s2, $zero, 0x1
 *     sllv $a0, $s2, $a0
 *     or $a0, $a3, $a0
 *     sw $a0, 0x0($a2)
 *     b .Leboot_001B67E0
 *     sb $zero, 0x34($a1)
 *   sym_001B67CC
 *     ori $a0, $zero, 0x1
 *     sltu $s2, $zero, $s2
 *     b .Leboot_001B67E0
 *     sb $a0, 0x86($s1)
 *   sym_001B67DC
 *     ori $s2, $zero, 0x1
 *   .Leboot_001B67E0
 *     beqz $s2, .Leboot_001B67F0
 *     nop
 *     jal updateNodeGraph_0CE0
 *     or $a0, $s1, $zero
 *   .Leboot_001B67F0
 *     andi $a0, $s0, 0x1
 *     beqz $a0, .Leboot_001B6814
 *     nop
 *     addiu $s0, $s1, 0x30
 *     jal updateNodeGraph_0420
 *     or $a0, $s1, $zero
 *     or $a0, $s0, $zero
 *     jal updateNodeGraph_036C
 *     mov.s $f12, $f0
 *   .Leboot_001B6814
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $ra, 0x1C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0E74(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "andi $s2, $a1, 0x3FE\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x1C($sp)\n\t"
        "beqz $s2, .Leboot_001B6740\n\t"
        "or $s0, $a1, $zero\n\t"
        "lbu $a0, 0x84($s1)\n\t"
        "bnez $a0, .Leboot_001B6740\n\t"
        "nop\n\t"
        "lw $a0, 0x18($s1)\n\t"
        "addiu $a1, $s1, 0x90\n\t"
        "addiu $a0, $a0, 0xD0\n\t"
        "lh $a2, 0x0($a0)\n\t"
        "lw $a3, 0x4($a0)\n\t"
        "jalr $a3\n\t"
        "addu $a0, $s1, $a2\n\t"
        ".Leboot_001B6740:\n\t"
        "lui $a0, %%hi(D_400003FE)\n\t"
        "addiu $a0, $a0, %%lo(D_400003FE)\n\t"
        "and $a0, $s0, $a0\n\t"
        "beqz $a0, .Leboot_001B67F0\n\t"
        "nop\n\t"
        "lw $a0, 0x14($s1)\n\t"
        "lbu $a0, 0x50($a0)\n\t"
        "addiu $a0, $a0, -0x3\n\t"
        "sltiu $a1, $a0, 0x7\n\t"
        "beqz $a1, sym_001B67DC\n\t"
        "nop\n\t"
        "sll $a0, $a0, 2\n\t"
        "lui $at, %%hi(sym_001CAE18)\n\t"
        "addu $at, $at, $a0\n\t"
        "lw $at, %%lo(sym_001CAE18)($at)\n\t"
        "jr $at\n\t"
        "nop\n\t"
        "sym_001B6784:\n\t"
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
        "ori $s2, $zero, 0x1\n\t"
        "sllv $a0, $s2, $a0\n\t"
        "or $a0, $a3, $a0\n\t"
        "sw $a0, 0x0($a2)\n\t"
        "b .Leboot_001B67E0\n\t"
        "sb $zero, 0x34($a1)\n\t"
        "sym_001B67CC:\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "sltu $s2, $zero, $s2\n\t"
        "b .Leboot_001B67E0\n\t"
        "sb $a0, 0x86($s1)\n\t"
        "sym_001B67DC:\n\t"
        "ori $s2, $zero, 0x1\n\t"
        ".Leboot_001B67E0:\n\t"
        "beqz $s2, .Leboot_001B67F0\n\t"
        "nop\n\t"
        "jal updateNodeGraph_0CE0\n\t"
        "or $a0, $s1, $zero\n\t"
        ".Leboot_001B67F0:\n\t"
        "andi $a0, $s0, 0x1\n\t"
        "beqz $a0, .Leboot_001B6814\n\t"
        "nop\n\t"
        "addiu $s0, $s1, 0x30\n\t"
        "jal updateNodeGraph_0420\n\t"
        "or $a0, $s1, $zero\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal updateNodeGraph_036C\n\t"
        "mov.s $f12, $f0\n\t"
        ".Leboot_001B6814:\n\t"
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

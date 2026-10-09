/**
 * The Sims 2 PSP - updateNodeGraph_079C (0x1B601C, 0x7C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x10($sp)
 *     or $s0, $a0, $zero
 *     lw $a0, 0x24($s0)
 *     sw $ra, 0x14($sp)
 *     andi $a1, $a0, 0x1
 *     bnez $a1, .Leboot_001B6050
 *     nop
 *     andi $a0, $a0, 0x4
 *     bnel $a0, $zero, .Leboot_001B6058
 *     lw $a0, 0x68($s0)
 *     b .Leboot_001B6078
 *     nop
 *   .Leboot_001B6050
 *     b .Leboot_001B6088
 *     nop
 *   .Leboot_001B6058
 *     bnez $a0, .Leboot_001B6078
 *     nop
 *     or $a0, $s0, $zero
 *     jal func_000BAA6C
 *     ori $a1, $zero, 0x4
 *     or $a0, $s0, $zero
 *     jal func_000A8D7C
 *     or $a1, $zero, $zero
 *   .Leboot_001B6078
 *     jal updateNodeGraph_0440
 *     or $a0, $s0, $zero
 *     jal updateNodeGraph_0980
 *     or $a0, $s0, $zero
 *   .Leboot_001B6088
 *     lw $s0, 0x10($sp)
 *     lw $ra, 0x14($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_079C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "lw $a0, 0x24($s0)\n\t"
        "sw $ra, 0x14($sp)\n\t"
        "andi $a1, $a0, 0x1\n\t"
        "bnez $a1, .Leboot_001B6050\n\t"
        "nop\n\t"
        "andi $a0, $a0, 0x4\n\t"
        "bnel $a0, $zero, .Leboot_001B6058\n\t"
        "lw $a0, 0x68($s0)\n\t"
        "b .Leboot_001B6078\n\t"
        "nop\n\t"
        ".Leboot_001B6050:\n\t"
        "b .Leboot_001B6088\n\t"
        "nop\n\t"
        ".Leboot_001B6058:\n\t"
        "bnez $a0, .Leboot_001B6078\n\t"
        "nop\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal func_000BAA6C\n\t"
        "ori $a1, $zero, 0x4\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal func_000A8D7C\n\t"
        "or $a1, $zero, $zero\n\t"
        ".Leboot_001B6078:\n\t"
        "jal updateNodeGraph_0440\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal updateNodeGraph_0980\n\t"
        "or $a0, $s0, $zero\n\t"
        ".Leboot_001B6088:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $ra, 0x14($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

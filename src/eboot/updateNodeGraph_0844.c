/**
 * The Sims 2 PSP - updateNodeGraph_0844 (0x1B60C4, 0x80 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s3, 0x1C($sp)
 *     andi $s3, $a1, 0xF00
 *     sw $s0, 0x10($sp)
 *     sw $s1, 0x14($sp)
 *     sw $s2, 0x18($sp)
 *     sra $s3, $s3, 8
 *     andi $s2, $a1, 0xF0FF
 *     or $s1, $a0, $zero
 *     or $s0, $a2, $zero
 *     sw $ra, 0x20($sp)
 *     jal func_000AAD7C
 *     or $a1, $s3, $zero
 *     bnez $v0, .Leboot_001B6108
 *     nop
 *     b .Leboot_001B6128
 *     or $v0, $zero, $zero
 *   .Leboot_001B6108
 *     or $a0, $s1, $zero
 *     jal func_000AAD7C
 *     or $a1, $s3, $zero
 *     or $a0, $v0, $zero
 *     or $a1, $s2, $zero
 *     jal updateNodeGraph_0000
 *     or $a2, $s0, $zero
 *     ori $v0, $zero, 0x1
 *   .Leboot_001B6128
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0844(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "andi $s3, $a1, 0xF00\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sra $s3, $s3, 8\n\t"
        "andi $s2, $a1, 0xF0FF\n\t"
        "or $s1, $a0, $zero\n\t"
        "or $s0, $a2, $zero\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "jal func_000AAD7C\n\t"
        "or $a1, $s3, $zero\n\t"
        "bnez $v0, .Leboot_001B6108\n\t"
        "nop\n\t"
        "b .Leboot_001B6128\n\t"
        "or $v0, $zero, $zero\n\t"
        ".Leboot_001B6108:\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal func_000AAD7C\n\t"
        "or $a1, $s3, $zero\n\t"
        "or $a0, $v0, $zero\n\t"
        "or $a1, $s2, $zero\n\t"
        "jal updateNodeGraph_0000\n\t"
        "or $a2, $s0, $zero\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".Leboot_001B6128:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

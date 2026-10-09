/**
 * The Sims 2 PSP - updateNodeGraph_0CE0 (0x1B6560, 0x94 bytes)
 *
 *     addiu $sp, $sp, -0xB0
 *     lbu $a1, 0x86($a0)
 *     sw $s1, 0x94($sp)
 *     addiu $s1, $a0, 0x30
 *     sw $s0, 0x90($sp)
 *     sw $s2, 0x98($sp)
 *     sw $s3, 0x9C($sp)
 *     sw $ra, 0xA0($sp)
 *     beqz $a1, .Leboot_001B659C
 *     addiu $s0, $a0, 0x90
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_02A0
 *     or $a1, $s0, $zero
 *     b .Leboot_001B65D8
 *     nop
 *   .Leboot_001B659C
 *     lw $a2, 0x8($a0)
 *     addiu $a1, $a0, 0x80
 *     addiu $s2, $sp, 0x10
 *     lw $a0, 0x14($a0)
 *     addiu $a2, $a2, 0x4
 *     jal updateNodeGraph_21A0
 *     or $a3, $s2, $zero
 *     addiu $s3, $sp, 0x50
 *     or $a0, $s0, $zero
 *     or $a1, $s2, $zero
 *     jal func_0012E284
 *     or $a2, $s3, $zero
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_02A0
 *     or $a1, $s3, $zero
 *   .Leboot_001B65D8
 *     lw $s0, 0x90($sp)
 *     lw $s1, 0x94($sp)
 *     lw $s2, 0x98($sp)
 *     lw $s3, 0x9C($sp)
 *     lw $ra, 0xA0($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0xB0
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0CE0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0xB0\n\t"
        "lbu $a1, 0x86($a0)\n\t"
        "sw $s1, 0x94($sp)\n\t"
        "addiu $s1, $a0, 0x30\n\t"
        "sw $s0, 0x90($sp)\n\t"
        "sw $s2, 0x98($sp)\n\t"
        "sw $s3, 0x9C($sp)\n\t"
        "sw $ra, 0xA0($sp)\n\t"
        "beqz $a1, .Leboot_001B659C\n\t"
        "addiu $s0, $a0, 0x90\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_02A0\n\t"
        "or $a1, $s0, $zero\n\t"
        "b .Leboot_001B65D8\n\t"
        "nop\n\t"
        ".Leboot_001B659C:\n\t"
        "lw $a2, 0x8($a0)\n\t"
        "addiu $a1, $a0, 0x80\n\t"
        "addiu $s2, $sp, 0x10\n\t"
        "lw $a0, 0x14($a0)\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        "jal updateNodeGraph_21A0\n\t"
        "or $a3, $s2, $zero\n\t"
        "addiu $s3, $sp, 0x50\n\t"
        "or $a0, $s0, $zero\n\t"
        "or $a1, $s2, $zero\n\t"
        "jal func_0012E284\n\t"
        "or $a2, $s3, $zero\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_02A0\n\t"
        "or $a1, $s3, $zero\n\t"
        ".Leboot_001B65D8:\n\t"
        "lw $s0, 0x90($sp)\n\t"
        "lw $s1, 0x94($sp)\n\t"
        "lw $s2, 0x98($sp)\n\t"
        "lw $s3, 0x9C($sp)\n\t"
        "lw $ra, 0xA0($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0xB0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

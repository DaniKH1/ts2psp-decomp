/**
 * The Sims 2 PSP - updateNodeGraph_0980 (0x1B6200, 0x64 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x14($sp)
 *     jal updateNodeGraph_09E4
 *     or $s0, $a0, $zero
 *     lw $a0, 0x8($s0)
 *     lw $a1, 0x0($a0)
 *     addiu $a1, $a1, 0x1
 *     andi $a2, $a1, 0x3F
 *     sll $a2, $a2, 3
 *     addu $a3, $a2, $a2
 *     addu $a2, $a2, $a3
 *     addu $a0, $a0, $a2
 *     sw $a1, 0xC($s0)
 *     addiu $a0, $a0, 0x324
 *     sw $a0, 0x0($s0)
 *     lw $a0, 0x4($a0)
 *     sw $a0, 0x4($s0)
 *     sw $s0, 0x0($a0)
 *     lw $a0, 0x0($s0)
 *     sw $s0, 0x4($a0)
 *     lw $s0, 0x10($sp)
 *     lw $ra, 0x14($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0980(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x14($sp)\n\t"
        "jal updateNodeGraph_09E4\n\t"
        "or $s0, $a0, $zero\n\t"
        "lw $a0, 0x8($s0)\n\t"
        "lw $a1, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "andi $a2, $a1, 0x3F\n\t"
        "sll $a2, $a2, 3\n\t"
        "addu $a3, $a2, $a2\n\t"
        "addu $a2, $a2, $a3\n\t"
        "addu $a0, $a0, $a2\n\t"
        "sw $a1, 0xC($s0)\n\t"
        "addiu $a0, $a0, 0x324\n\t"
        "sw $a0, 0x0($s0)\n\t"
        "lw $a0, 0x4($a0)\n\t"
        "sw $a0, 0x4($s0)\n\t"
        "sw $s0, 0x0($a0)\n\t"
        "lw $a0, 0x0($s0)\n\t"
        "sw $s0, 0x4($a0)\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $ra, 0x14($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

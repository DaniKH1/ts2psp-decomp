/**
 * The Sims 2 PSP - updateNodeGraph_0FAC (0x1B682C, 0x4C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a0, 0x70($a0)
 *     sw $ra, 0x10($sp)
 *     beqz $a0, .Leboot_001B684C
 *     nop
 *     lw $a1, 0x20($a0)
 *     beqz $a1, .Leboot_001B6854
 *     nop
 *   .Leboot_001B684C
 *     b .Leboot_001B686C
 *     or $v0, $zero, $zero
 *   .Leboot_001B6854
 *     lw $a1, 0x18($a0)
 *     addiu $a1, $a1, 0x50
 *     lh $a2, 0x0($a1)
 *     lw $a1, 0x4($a1)
 *     jalr $a1
 *     addu $a0, $a0, $a2
 *   .Leboot_001B686C
 *     lw $ra, 0x10($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0FAC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a0, 0x70($a0)\n\t"
        "sw $ra, 0x10($sp)\n\t"
        "beqz $a0, .Leboot_001B684C\n\t"
        "nop\n\t"
        "lw $a1, 0x20($a0)\n\t"
        "beqz $a1, .Leboot_001B6854\n\t"
        "nop\n\t"
        ".Leboot_001B684C:\n\t"
        "b .Leboot_001B686C\n\t"
        "or $v0, $zero, $zero\n\t"
        ".Leboot_001B6854:\n\t"
        "lw $a1, 0x18($a0)\n\t"
        "addiu $a1, $a1, 0x50\n\t"
        "lh $a2, 0x0($a1)\n\t"
        "lw $a1, 0x4($a1)\n\t"
        "jalr $a1\n\t"
        "addu $a0, $a0, $a2\n\t"
        ".Leboot_001B686C:\n\t"
        "lw $ra, 0x10($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

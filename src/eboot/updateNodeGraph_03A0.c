/**
 * The Sims 2 PSP - updateNodeGraph_03A0 (0x1B5C20, 0x5C bytes)
 *
 *     lw $a0, 0x40($a0)
 *     beqz $a0, .Leboot_001B5C74
 *     ori $a2, $zero, 0x1
 *     lw $a3, 0xC($a0)
 *   .Leboot_001B5C30
 *     lw $t0, 0x0($a0)
 *     or $a3, $a3, $a1
 *     sw $a3, 0xC($a0)
 *     lw $a3, 0x4($a0)
 *     lw $t1, 0x38($t0)
 *     sra $t2, $a3, 5
 *     sll $t2, $t2, 2
 *     addu $t1, $t1, $t2
 *     lw $t2, 0x0($t1)
 *     andi $a3, $a3, 0x1F
 *     sllv $a3, $a2, $a3
 *     or $a3, $t2, $a3
 *     sw $a3, 0x0($t1)
 *     sb $zero, 0x34($t0)
 *     lw $a0, 0x20($a0)
 *     bnel $a0, $zero, .Leboot_001B5C30
 *     lw $a3, 0xC($a0)
 *   .Leboot_001B5C74
 *     jr $ra
 *     nop
 *
 * updateNodeGraph: walk the node at 0x40, OR a flag into its 0xC field, and loop.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_03A0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $a0, 0x40($a0)\n\t"
        "beqz $a0, .Leboot_001B5C74\n\t"
        "ori $a2, $zero, 0x1\n\t"
        "lw $a3, 0xC($a0)\n\t"
        ".Leboot_001B5C30:\n\t"
        "lw $t0, 0x0($a0)\n\t"
        "or $a3, $a3, $a1\n\t"
        "sw $a3, 0xC($a0)\n\t"
        "lw $a3, 0x4($a0)\n\t"
        "lw $t1, 0x38($t0)\n\t"
        "sra $t2, $a3, 5\n\t"
        "sll $t2, $t2, 2\n\t"
        "addu $t1, $t1, $t2\n\t"
        "lw $t2, 0x0($t1)\n\t"
        "andi $a3, $a3, 0x1F\n\t"
        "sllv $a3, $a2, $a3\n\t"
        "or $a3, $t2, $a3\n\t"
        "sw $a3, 0x0($t1)\n\t"
        "sb $zero, 0x34($t0)\n\t"
        "lw $a0, 0x20($a0)\n\t"
        "bnel $a0, $zero, .Leboot_001B5C30\n\t"
        "lw $a3, 0xC($a0)\n\t"
        ".Leboot_001B5C74:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

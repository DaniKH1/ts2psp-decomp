/**
 * The Sims 2 PSP - updateNodeGraph_0FF8 (0x1B6878, 0x124 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lbu $a2, 0x6C($a1)
 *     sw $s1, 0x20($sp)
 *     or $s1, $a0, $zero
 *     sw $s0, 0x1C($sp)
 *     sw $s2, 0x24($sp)
 *     sw $ra, 0x28($sp)
 *     beqz $a2, .Leboot_001B6960
 *     or $s0, $a1, $zero
 *     addiu $s2, $s0, 0x50
 *     lwc1 $f12, 0x0($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x1
 *     lwc1 $f12, 0x4($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x2
 *     lwc1 $f12, 0x8($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x3
 *     addiu $s2, $s0, 0x60
 *     lwc1 $f12, 0x0($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x7
 *     lwc1 $f12, 0x4($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x8
 *     lwc1 $f12, 0x8($s2)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x9
 *     lbu $a0, 0x85($s1)
 *     bnez $a0, .Leboot_001B691C
 *     nop
 *     lbu $a0, 0x6D($s0)
 *     bnez $a0, .Leboot_001B6960
 *     nop
 *   .Leboot_001B691C
 *     lw $a1, 0x14($s1)
 *     addiu $a0, $sp, 0x10
 *     lbu $a2, 0x51($a1)
 *     jal updateNodeGraph_2F70
 *     addiu $a1, $s0, 0x40
 *     lwc1 $f12, 0x10($sp)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x4
 *     lwc1 $f12, 0x14($sp)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x5
 *     lwc1 $f12, 0x18($sp)
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_08C4
 *     ori $a1, $zero, 0x6
 *   .Leboot_001B6960
 *     lbu $a0, 0x6D($s0)
 *     beqz $a0, .Leboot_001B6980
 *     nop
 *     or $a0, $s1, $zero
 *     jal updateNodeGraph_111C
 *     or $a1, $s0, $zero
 *     b .Leboot_001B6984
 *     nop
 *   .Leboot_001B6980
 *     sb $zero, 0x84($s1)
 *   .Leboot_001B6984
 *     lw $s0, 0x1C($sp)
 *     lw $s1, 0x20($sp)
 *     lw $s2, 0x24($sp)
 *     lw $ra, 0x28($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0FF8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lbu $a2, 0x6C($a1)\n\t"
        "sw $s1, 0x20($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x1C($sp)\n\t"
        "sw $s2, 0x24($sp)\n\t"
        "sw $ra, 0x28($sp)\n\t"
        "beqz $a2, .Leboot_001B6960\n\t"
        "or $s0, $a1, $zero\n\t"
        "addiu $s2, $s0, 0x50\n\t"
        "lwc1 $f12, 0x0($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x1\n\t"
        "lwc1 $f12, 0x4($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x2\n\t"
        "lwc1 $f12, 0x8($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x3\n\t"
        "addiu $s2, $s0, 0x60\n\t"
        "lwc1 $f12, 0x0($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x7\n\t"
        "lwc1 $f12, 0x4($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x8\n\t"
        "lwc1 $f12, 0x8($s2)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x9\n\t"
        "lbu $a0, 0x85($s1)\n\t"
        "bnez $a0, .Leboot_001B691C\n\t"
        "nop\n\t"
        "lbu $a0, 0x6D($s0)\n\t"
        "bnez $a0, .Leboot_001B6960\n\t"
        "nop\n\t"
        ".Leboot_001B691C:\n\t"
        "lw $a1, 0x14($s1)\n\t"
        "addiu $a0, $sp, 0x10\n\t"
        "lbu $a2, 0x51($a1)\n\t"
        "jal updateNodeGraph_2F70\n\t"
        "addiu $a1, $s0, 0x40\n\t"
        "lwc1 $f12, 0x10($sp)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x4\n\t"
        "lwc1 $f12, 0x14($sp)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x5\n\t"
        "lwc1 $f12, 0x18($sp)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_08C4\n\t"
        "ori $a1, $zero, 0x6\n\t"
        ".Leboot_001B6960:\n\t"
        "lbu $a0, 0x6D($s0)\n\t"
        "beqz $a0, .Leboot_001B6980\n\t"
        "nop\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal updateNodeGraph_111C\n\t"
        "or $a1, $s0, $zero\n\t"
        "b .Leboot_001B6984\n\t"
        "nop\n\t"
        ".Leboot_001B6980:\n\t"
        "sb $zero, 0x84($s1)\n\t"
        ".Leboot_001B6984:\n\t"
        "lw $s0, 0x1C($sp)\n\t"
        "lw $s1, 0x20($sp)\n\t"
        "lw $s2, 0x24($sp)\n\t"
        "lw $ra, 0x28($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - drawing_0000 (0x1BB280, 0x74 bytes)
 *
 *     lbu $a3, 0x0($a1)
 *     slti $a2, $a3, 0x80
 *     bnez $a2, .Leboot_001BB2B8
 *     nop
 *     lbu $a2, 0x1($a1)
 *     slti $t0, $a3, 0xE0
 *     beqz $t0, .Leboot_001BB2C8
 *     xori $a2, $a2, 0x80
 *     andi $a1, $a3, 0x1F
 *     sll $a1, $a1, 6
 *     or $a1, $a1, $a2
 *     sh $a1, 0x0($a0)
 *     b .Leboot_001BB2EC
 *     ori $v0, $zero, 0x2
 *   .Leboot_001BB2B8
 *     andi $a1, $a3, 0xFF
 *     sh $a1, 0x0($a0)
 *     b .Leboot_001BB2EC
 *     ori $v0, $zero, 0x1
 *   .Leboot_001BB2C8
 *     lbu $a1, 0x2($a1)
 *     andi $a3, $a3, 0xF
 *     sll $a3, $a3, 12
 *     sll $a2, $a2, 6
 *     or $a2, $a3, $a2
 *     xori $a1, $a1, 0x80
 *     or $a1, $a2, $a1
 *     sh $a1, 0x0($a0)
 *     ori $v0, $zero, 0x3
 *   .Leboot_001BB2EC
 *     jr $ra
 *     nop
 *
 * drawing: the entry point of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0000(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lbu $a3, 0x0($a1)\n\t"
        "slti $a2, $a3, 0x80\n\t"
        "bnez $a2, .Leboot_001BB2B8\n\t"
        "nop\n\t"
        "lbu $a2, 0x1($a1)\n\t"
        "slti $t0, $a3, 0xE0\n\t"
        "beqz $t0, .Leboot_001BB2C8\n\t"
        "xori $a2, $a2, 0x80\n\t"
        "andi $a1, $a3, 0x1F\n\t"
        "sll $a1, $a1, 6\n\t"
        "or $a1, $a1, $a2\n\t"
        "sh $a1, 0x0($a0)\n\t"
        "b .Leboot_001BB2EC\n\t"
        "ori $v0, $zero, 0x2\n\t"
        ".Leboot_001BB2B8:\n\t"
        "andi $a1, $a3, 0xFF\n\t"
        "sh $a1, 0x0($a0)\n\t"
        "b .Leboot_001BB2EC\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".Leboot_001BB2C8:\n\t"
        "lbu $a1, 0x2($a1)\n\t"
        "andi $a3, $a3, 0xF\n\t"
        "sll $a3, $a3, 12\n\t"
        "sll $a2, $a2, 6\n\t"
        "or $a2, $a3, $a2\n\t"
        "xori $a1, $a1, 0x80\n\t"
        "or $a1, $a2, $a1\n\t"
        "sh $a1, 0x0($a0)\n\t"
        "ori $v0, $zero, 0x3\n\t"
        ".Leboot_001BB2EC:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

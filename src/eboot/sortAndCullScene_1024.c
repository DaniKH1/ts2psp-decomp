/**
 * The Sims 2 PSP - sortAndCullScene_1024 (0x1B4C40, 0x54 bytes)
 *
 *     lw $a3, 0x108($a0)
 *     addiu $a2, $a0, 0xF0
 *     subu $a3, $a3, $a2
 *     sra $t0, $a3, 2
 *     srl $t0, $t0, 30
 *     addu $a3, $a3, $t0
 *     sra $a3, $a3, 2
 *     sltiu $a3, $a3, 0x2
 *     bnez $a3, .Leboot_001B4C88
 *     nop
 *     sw $a2, 0x0($a1)
 *     lw $a0, 0x108($a0)
 *     subu $a0, $a0, $a2
 *     sra $a1, $a0, 2
 *     srl $a1, $a1, 30
 *     addu $v0, $a0, $a1
 *     b .Leboot_001B4C8C
 *     sra $v0, $v0, 2
 *   .Leboot_001B4C88
 *     ori $v0, $zero, 0x1
 *   .Leboot_001B4C8C
 *     jr $ra
 *     nop
 *
 * sortAndCullScene: signed-divide (0x108 - (this+0xF0)) by 2 and branch when the answer is at least 2.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1024(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $a3, 0x108($a0)\n\t"
        "addiu $a2, $a0, 0xF0\n\t"
        "subu $a3, $a3, $a2\n\t"
        "sra $t0, $a3, 2\n\t"
        "srl $t0, $t0, 30\n\t"
        "addu $a3, $a3, $t0\n\t"
        "sra $a3, $a3, 2\n\t"
        "sltiu $a3, $a3, 0x2\n\t"
        "bnez $a3, .Leboot_001B4C88\n\t"
        "nop\n\t"
        "sw $a2, 0x0($a1)\n\t"
        "lw $a0, 0x108($a0)\n\t"
        "subu $a0, $a0, $a2\n\t"
        "sra $a1, $a0, 2\n\t"
        "srl $a1, $a1, 30\n\t"
        "addu $v0, $a0, $a1\n\t"
        "b .Leboot_001B4C8C\n\t"
        "sra $v0, $v0, 2\n\t"
        ".Leboot_001B4C88:\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".Leboot_001B4C8C:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

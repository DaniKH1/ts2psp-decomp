/**
 * The Sims 2 PSP - collision_011C (0x1B093C, 0x7C bytes)
 *
 *     slt $t0, $a2, $a3
 *     beqz $t0, .Leboot_001B09B0
 *     nop
 *     lw $t0, 0x20($a0)
 *     addiu $t1, $a0, 0x20
 *     lw $t2, 0x18($a0)
 *     addiu $a0, $a0, 0x18
 *     addu $t0, $t1, $t0
 *     addu $a0, $a0, $t2
 *     addu $t1, $a3, $a2
 *   .Leboot_001B0964
 *     sra $t1, $t1, 1
 *     sll $t2, $t1, 2
 *     addu $t2, $t0, $t2
 *     lbu $t2, 0x0($t2)
 *     addu $t3, $t2, $t2
 *     addu $t2, $t2, $t3
 *     addu $t2, $t2, $t2
 *     addu $t2, $a0, $t2
 *     lh $t2, 0x0($t2)
 *     slt $t2, $a1, $t2
 *     beqz $t2, .Leboot_001B09A0
 *     nop
 *     or $a3, $t1, $zero
 *     b .Leboot_001B09A8
 *     slt $t1, $a2, $t1
 *   .Leboot_001B09A0
 *     addiu $a2, $t1, 0x1
 *     slt $t1, $a2, $a3
 *   .Leboot_001B09A8
 *     bnez $t1, .Leboot_001B0964
 *     addu $t1, $a3, $a2
 *   .Leboot_001B09B0
 *     jr $ra
 *     or $v0, $a2, $zero
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_011C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "slt $t0, $a2, $a3\n\t"
        "beqz $t0, .Leboot_001B09B0\n\t"
        "nop\n\t"
        "lw $t0, 0x20($a0)\n\t"
        "addiu $t1, $a0, 0x20\n\t"
        "lw $t2, 0x18($a0)\n\t"
        "addiu $a0, $a0, 0x18\n\t"
        "addu $t0, $t1, $t0\n\t"
        "addu $a0, $a0, $t2\n\t"
        "addu $t1, $a3, $a2\n\t"
        ".Leboot_001B0964:\n\t"
        "sra $t1, $t1, 1\n\t"
        "sll $t2, $t1, 2\n\t"
        "addu $t2, $t0, $t2\n\t"
        "lbu $t2, 0x0($t2)\n\t"
        "addu $t3, $t2, $t2\n\t"
        "addu $t2, $t2, $t3\n\t"
        "addu $t2, $t2, $t2\n\t"
        "addu $t2, $a0, $t2\n\t"
        "lh $t2, 0x0($t2)\n\t"
        "slt $t2, $a1, $t2\n\t"
        "beqz $t2, .Leboot_001B09A0\n\t"
        "nop\n\t"
        "or $a3, $t1, $zero\n\t"
        "b .Leboot_001B09A8\n\t"
        "slt $t1, $a2, $t1\n\t"
        ".Leboot_001B09A0:\n\t"
        "addiu $a2, $t1, 0x1\n\t"
        "slt $t1, $a2, $a3\n\t"
        ".Leboot_001B09A8:\n\t"
        "bnez $t1, .Leboot_001B0964\n\t"
        "addu $t1, $a3, $a2\n\t"
        ".Leboot_001B09B0:\n\t"
        "jr $ra\n\t"
        "or $v0, $a2, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

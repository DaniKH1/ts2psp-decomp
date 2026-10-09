/**
 * The Sims 2 PSP - drawing_0810 (0x1BBA90, 0x104 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     or $s2, $a1, $zero
 *     or $s3, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $s1, 0x14($sp)
 *     or $s0, $a2, $zero
 *     or $s1, $a3, $zero
 *     ori $a0, $zero, 0x2
 *     ori $a1, $zero, 0x2
 *     sw $ra, 0x20($sp)
 *     jal drawing_0480
 *     ori $a2, $zero, 0x2
 *     lui $a1, %%hi(sym_001DAF44)
 *     lw $a2, %%lo(sym_001DAF44)($a1)
 *     lui $a3, %%hi(sym_00061B38)
 *     addiu $a3, $a3, %%lo(sym_00061B38)
 *     ori $a0, $zero, 0x2C
 *     subu $a3, $a2, $a3
 *     div $zero, $a3, $a0
 *     lw $a0, 0x0($s3)
 *     lw $a3, 0x4($s3)
 *     lw $t0, 0x8($s3)
 *     lui $t1, %%hi(sym_001DAF48)
 *     lw $t1, %%lo(sym_001DAF48)($t1)
 *     sw $a0, 0x0($a2)
 *     sw $a3, 0x4($a2)
 *     sw $t0, 0x8($a2)
 *     lw $a0, 0x0($s2)
 *     addiu $a3, $a2, 0x1C
 *     lw $t0, 0x4($s2)
 *     sw $a0, 0x0($a3)
 *     sw $t0, 0x4($a3)
 *     sw $t1, 0x18($a2)
 *     lw $a0, 0x0($s0)
 *     lw $t0, 0x4($s0)
 *     addiu $a3, $a2, 0x2C
 *     lw $t2, 0x8($s0)
 *     sw $a0, 0x0($a3)
 *     sw $t0, 0x4($a3)
 *     sw $t2, 0x8($a3)
 *     lw $a0, 0x0($s1)
 *     addiu $a3, $a2, 0x48
 *     lw $t0, 0x4($s1)
 *     sw $a0, 0x0($a3)
 *     sw $t0, 0x4($a3)
 *     sw $t1, 0x44($a2)
 *     addiu $a0, $a2, 0x58
 *     sw $a0, %%lo(sym_001DAF44)($a1)
 *     lui $a0, %%hi(sym_001DAF4C)
 *     lw $a1, %%lo(sym_001DAF4C)($a0)
 *     mflo $a3
 *     sh $a3, 0x0($a1)
 *     addiu $a3, $a3, 0x1
 *     addiu $a2, $a1, 0x4
 *     sh $a3, 0x2($a1)
 *     sw $a2, %%lo(sym_001DAF4C)($a0)
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0810(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "or $s2, $a1, $zero\n\t"
        "or $s3, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s0, $a2, $zero\n\t"
        "or $s1, $a3, $zero\n\t"
        "ori $a0, $zero, 0x2\n\t"
        "ori $a1, $zero, 0x2\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "jal drawing_0480\n\t"
        "ori $a2, $zero, 0x2\n\t"
        "lui $a1, %%hi(sym_001DAF44)\n\t"
        "lw $a2, %%lo(sym_001DAF44)($a1)\n\t"
        "lui $a3, %%hi(sym_00061B38)\n\t"
        "addiu $a3, $a3, %%lo(sym_00061B38)\n\t"
        "ori $a0, $zero, 0x2C\n\t"
        "subu $a3, $a2, $a3\n\t"
        "div $zero, $a3, $a0\n\t"
        "lw $a0, 0x0($s3)\n\t"
        "lw $a3, 0x4($s3)\n\t"
        "lw $t0, 0x8($s3)\n\t"
        "lui $t1, %%hi(sym_001DAF48)\n\t"
        "lw $t1, %%lo(sym_001DAF48)($t1)\n\t"
        "sw $a0, 0x0($a2)\n\t"
        "sw $a3, 0x4($a2)\n\t"
        "sw $t0, 0x8($a2)\n\t"
        "lw $a0, 0x0($s2)\n\t"
        "addiu $a3, $a2, 0x1C\n\t"
        "lw $t0, 0x4($s2)\n\t"
        "sw $a0, 0x0($a3)\n\t"
        "sw $t0, 0x4($a3)\n\t"
        "sw $t1, 0x18($a2)\n\t"
        "lw $a0, 0x0($s0)\n\t"
        "lw $t0, 0x4($s0)\n\t"
        "addiu $a3, $a2, 0x2C\n\t"
        "lw $t2, 0x8($s0)\n\t"
        "sw $a0, 0x0($a3)\n\t"
        "sw $t0, 0x4($a3)\n\t"
        "sw $t2, 0x8($a3)\n\t"
        "lw $a0, 0x0($s1)\n\t"
        "addiu $a3, $a2, 0x48\n\t"
        "lw $t0, 0x4($s1)\n\t"
        "sw $a0, 0x0($a3)\n\t"
        "sw $t0, 0x4($a3)\n\t"
        "sw $t1, 0x44($a2)\n\t"
        "addiu $a0, $a2, 0x58\n\t"
        "sw $a0, %%lo(sym_001DAF44)($a1)\n\t"
        "lui $a0, %%hi(sym_001DAF4C)\n\t"
        "lw $a1, %%lo(sym_001DAF4C)($a0)\n\t"
        "mflo $a3\n\t"
        "sh $a3, 0x0($a1)\n\t"
        "addiu $a3, $a3, 0x1\n\t"
        "addiu $a2, $a1, 0x4\n\t"
        "sh $a3, 0x2($a1)\n\t"
        "sw $a2, %%lo(sym_001DAF4C)($a0)\n\t"
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

/**
 * The Sims 2 PSP - drawing_051C (0x1BB79C, 0xC8 bytes)
 *
 *     addiu $sp, $sp, -0x40
 *     sw $s5, 0x34($sp)
 *     lui $s5, %%hi(sym_001DAF44)
 *     sw $s2, 0x28($sp)
 *     sw $s4, 0x30($sp)
 *     lw $a2, %%lo(sym_001DAF44)($s5)
 *     lui $s4, %%hi(sym_00061B38)
 *     lui $s2, %%hi(sym_00063D98)
 *     sw $s0, 0x20($sp)
 *     sw $s3, 0x2C($sp)
 *     addiu $s0, $zero, -0x1
 *     addiu $s4, $s4, %%lo(sym_00061B38)
 *     addiu $s2, $s2, %%lo(sym_00063D98)
 *     lui $s3, %%hi(sym_001DAF4C)
 *     sw $s1, 0x24($sp)
 *     sw $ra, 0x38($sp)
 *     beq $a2, $s4, .Leboot_001BB834
 *     lui $s1, %%hi(sym_001DAF40)
 *     lw $a1, %%lo(sym_001DAF4C)($s3)
 *     beq $a1, $s2, .Leboot_001BB834
 *     nop
 *     lw $a0, %%lo(sym_001DAF40)($s1)
 *     beq $a0, $s0, .Leboot_001BB834
 *     or $t1, $a0, $zero
 *     subu $a0, $a2, $s4
 *     ori $a2, $zero, 0x2C
 *     div $zero, $a0, $a2
 *     subu $a0, $a1, $s2
 *     sra $a1, $a0, 1
 *     srl $a1, $a1, 31
 *     addu $a0, $a0, $a1
 *     sra $a0, $a0, 1
 *     or $a1, $s2, $zero
 *     or $a3, $s4, $zero
 *     or $t0, $zero, $zero
 *     mflo $a2
 *     jal drawing_0C04
 *     ori $t2, $zero, 0x1
 *   .Leboot_001BB834
 *     sw $s4, %%lo(sym_001DAF44)($s5)
 *     sw $s2, %%lo(sym_001DAF4C)($s3)
 *     sw $s0, %%lo(sym_001DAF40)($s1)
 *     lw $s0, 0x20($sp)
 *     lw $s1, 0x24($sp)
 *     lw $s2, 0x28($sp)
 *     lw $s3, 0x2C($sp)
 *     lw $s4, 0x30($sp)
 *     lw $s5, 0x34($sp)
 *     lw $ra, 0x38($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x40
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_051C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "sw $s5, 0x34($sp)\n\t"
        "lui $s5, %%hi(sym_001DAF44)\n\t"
        "sw $s2, 0x28($sp)\n\t"
        "sw $s4, 0x30($sp)\n\t"
        "lw $a2, %%lo(sym_001DAF44)($s5)\n\t"
        "lui $s4, %%hi(sym_00061B38)\n\t"
        "lui $s2, %%hi(sym_00063D98)\n\t"
        "sw $s0, 0x20($sp)\n\t"
        "sw $s3, 0x2C($sp)\n\t"
        "addiu $s0, $zero, -0x1\n\t"
        "addiu $s4, $s4, %%lo(sym_00061B38)\n\t"
        "addiu $s2, $s2, %%lo(sym_00063D98)\n\t"
        "lui $s3, %%hi(sym_001DAF4C)\n\t"
        "sw $s1, 0x24($sp)\n\t"
        "sw $ra, 0x38($sp)\n\t"
        "beq $a2, $s4, .Leboot_001BB834\n\t"
        "lui $s1, %%hi(sym_001DAF40)\n\t"
        "lw $a1, %%lo(sym_001DAF4C)($s3)\n\t"
        "beq $a1, $s2, .Leboot_001BB834\n\t"
        "nop\n\t"
        "lw $a0, %%lo(sym_001DAF40)($s1)\n\t"
        "beq $a0, $s0, .Leboot_001BB834\n\t"
        "or $t1, $a0, $zero\n\t"
        "subu $a0, $a2, $s4\n\t"
        "ori $a2, $zero, 0x2C\n\t"
        "div $zero, $a0, $a2\n\t"
        "subu $a0, $a1, $s2\n\t"
        "sra $a1, $a0, 1\n\t"
        "srl $a1, $a1, 31\n\t"
        "addu $a0, $a0, $a1\n\t"
        "sra $a0, $a0, 1\n\t"
        "or $a1, $s2, $zero\n\t"
        "or $a3, $s4, $zero\n\t"
        "or $t0, $zero, $zero\n\t"
        "mflo $a2\n\t"
        "jal drawing_0C04\n\t"
        "ori $t2, $zero, 0x1\n\t"
        ".Leboot_001BB834:\n\t"
        "sw $s4, %%lo(sym_001DAF44)($s5)\n\t"
        "sw $s2, %%lo(sym_001DAF4C)($s3)\n\t"
        "sw $s0, %%lo(sym_001DAF40)($s1)\n\t"
        "lw $s0, 0x20($sp)\n\t"
        "lw $s1, 0x24($sp)\n\t"
        "lw $s2, 0x28($sp)\n\t"
        "lw $s3, 0x2C($sp)\n\t"
        "lw $s4, 0x30($sp)\n\t"
        "lw $s5, 0x34($sp)\n\t"
        "lw $ra, 0x38($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

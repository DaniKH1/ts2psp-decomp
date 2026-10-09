/**
 * The Sims 2 PSP - drawing_0480 (0x1BB700, 0x9C bytes)
 *
 *     addiu $sp, $sp, -0x10
 *     sll $a3, $a2, 4
 *     subu $a2, $a3, $a2
 *     sll $a2, $a2, 2
 *     subu $a2, $a2, $a3
 *     lui $a3, %%hi(sym_001DAF44)
 *     lw $a3, %%lo(sym_001DAF44)($a3)
 *     lui $t0, %%hi(sym_00061B38)
 *     addiu $t0, $t0, %%lo(sym_00061B38)
 *     addiu $t0, $t0, 0x2260
 *     addu $a2, $a3, $a2
 *     sw $s0, 0x0($sp)
 *     sltu $a2, $t0, $a2
 *     or $s0, $a0, $zero
 *     sw $s1, 0x4($sp)
 *     sw $ra, 0x8($sp)
 *     bnez $a2, .Leboot_001BB77C
 *     lui $s1, %%hi(sym_001DAF40)
 *     lui $a0, %%hi(sym_001DAF4C)
 *     lw $a0, %%lo(sym_001DAF4C)($a0)
 *     lui $a2, %%hi(sym_00063D98)
 *     addu $a1, $a1, $a1
 *     addiu $a2, $a2, %%lo(sym_00063D98)
 *     addiu $a2, $a2, 0x258
 *     addu $a0, $a0, $a1
 *     sltu $a0, $a2, $a0
 *     bnez $a0, .Leboot_001BB77C
 *     nop
 *     lw $a0, %%lo(sym_001DAF40)($s1)
 *     beq $s0, $a0, .Leboot_001BB788
 *     nop
 *   .Leboot_001BB77C
 *     jal drawing_051C
 *     nop
 *     sw $s0, %%lo(sym_001DAF40)($s1)
 *   .Leboot_001BB788
 *     lw $s0, 0x0($sp)
 *     lw $s1, 0x4($sp)
 *     lw $ra, 0x8($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x10
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0480(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x10\n\t"
        "sll $a3, $a2, 4\n\t"
        "subu $a2, $a3, $a2\n\t"
        "sll $a2, $a2, 2\n\t"
        "subu $a2, $a2, $a3\n\t"
        "lui $a3, %%hi(sym_001DAF44)\n\t"
        "lw $a3, %%lo(sym_001DAF44)($a3)\n\t"
        "lui $t0, %%hi(sym_00061B38)\n\t"
        "addiu $t0, $t0, %%lo(sym_00061B38)\n\t"
        "addiu $t0, $t0, 0x2260\n\t"
        "addu $a2, $a3, $a2\n\t"
        "sw $s0, 0x0($sp)\n\t"
        "sltu $a2, $t0, $a2\n\t"
        "or $s0, $a0, $zero\n\t"
        "sw $s1, 0x4($sp)\n\t"
        "sw $ra, 0x8($sp)\n\t"
        "bnez $a2, .Leboot_001BB77C\n\t"
        "lui $s1, %%hi(sym_001DAF40)\n\t"
        "lui $a0, %%hi(sym_001DAF4C)\n\t"
        "lw $a0, %%lo(sym_001DAF4C)($a0)\n\t"
        "lui $a2, %%hi(sym_00063D98)\n\t"
        "addu $a1, $a1, $a1\n\t"
        "addiu $a2, $a2, %%lo(sym_00063D98)\n\t"
        "addiu $a2, $a2, 0x258\n\t"
        "addu $a0, $a0, $a1\n\t"
        "sltu $a0, $a2, $a0\n\t"
        "bnez $a0, .Leboot_001BB77C\n\t"
        "nop\n\t"
        "lw $a0, %%lo(sym_001DAF40)($s1)\n\t"
        "beq $s0, $a0, .Leboot_001BB788\n\t"
        "nop\n\t"
        ".Leboot_001BB77C:\n\t"
        "jal drawing_051C\n\t"
        "nop\n\t"
        "sw $s0, %%lo(sym_001DAF40)($s1)\n\t"
        ".Leboot_001BB788:\n\t"
        "lw $s0, 0x0($sp)\n\t"
        "lw $s1, 0x4($sp)\n\t"
        "lw $ra, 0x8($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - drawing_0074 (0x1BB2F4, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lw $a2, 0x0($a0)
 *     ori $a3, $zero, 0x0
 *     ori $t0, $zero, 0x1
 *     sw $ra, 0x20($sp)
 *     bnel $a2, $t0, .Leboot_001BB310
 *     addu $a3, $a0, $a2
 *   .Leboot_001BB310
 *     or $a0, $a1, $zero
 *     mtc1 $zero, $f12
 *     or $a1, $a3, $zero
 *     or $a2, $zero, $zero
 *     jal drawing_05E4
 *     or $a3, $zero, $zero
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0074(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lw $a2, 0x0($a0)\n\t"
        "ori $a3, $zero, 0x0\n\t"
        "ori $t0, $zero, 0x1\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "bnel $a2, $t0, .Leboot_001BB310\n\t"
        "addu $a3, $a0, $a2\n\t"
        ".Leboot_001BB310:\n\t"
        "or $a0, $a1, $zero\n\t"
        "mtc1 $zero, $f12\n\t"
        "or $a1, $a3, $zero\n\t"
        "or $a2, $zero, $zero\n\t"
        "jal drawing_05E4\n\t"
        "or $a3, $zero, $zero\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

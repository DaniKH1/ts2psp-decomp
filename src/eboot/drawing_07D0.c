/**
 * The Sims 2 PSP - drawing_07D0 (0x1BBA50, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     mtc1 $zero, $f12
 *     swc1 $f12, 0x10($sp)
 *     lui $a3, 0x3F80
 *     swc1 $f12, 0x14($sp)
 *     mtc1 $a3, $f13
 *     or $a2, $a1, $zero
 *     swc1 $f12, 0x1C($sp)
 *     addiu $a1, $sp, 0x10
 *     swc1 $f13, 0x18($sp)
 *     sw $ra, 0x20($sp)
 *     jal drawing_0780
 *     addiu $a3, $sp, 0x18
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_07D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "mtc1 $zero, $f12\n\t"
        "swc1 $f12, 0x10($sp)\n\t"
        "lui $a3, 0x3F80\n\t"
        "swc1 $f12, 0x14($sp)\n\t"
        "mtc1 $a3, $f13\n\t"
        "or $a2, $a1, $zero\n\t"
        "swc1 $f12, 0x1C($sp)\n\t"
        "addiu $a1, $sp, 0x10\n\t"
        "swc1 $f13, 0x18($sp)\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "jal drawing_0780\n\t"
        "addiu $a3, $sp, 0x18\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

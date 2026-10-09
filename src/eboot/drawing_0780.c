/**
 * The Sims 2 PSP - drawing_0780 (0x1BBA00, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lwc1 $f12, 0x0($a0)
 *     lui $t0, %%hi(sym_001DAF50)
 *     lwc1 $f13, %%lo(sym_001DAF50)($t0)
 *     swc1 $f12, 0x10($sp)
 *     lwc1 $f12, 0x4($a0)
 *     swc1 $f13, 0x18($sp)
 *     swc1 $f12, 0x14($sp)
 *     lwc1 $f12, 0x0($a2)
 *     swc1 $f12, 0x1C($sp)
 *     lwc1 $f12, 0x4($a2)
 *     swc1 $f13, 0x24($sp)
 *     addiu $a0, $sp, 0x10
 *     swc1 $f12, 0x20($sp)
 *     sw $ra, 0x28($sp)
 *     jal drawing_0810
 *     addiu $a2, $sp, 0x1C
 *     lw $ra, 0x28($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0780(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lui $t0, %%hi(sym_001DAF50)\n\t"
        "lwc1 $f13, %%lo(sym_001DAF50)($t0)\n\t"
        "swc1 $f12, 0x10($sp)\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "swc1 $f13, 0x18($sp)\n\t"
        "swc1 $f12, 0x14($sp)\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "swc1 $f12, 0x1C($sp)\n\t"
        "lwc1 $f12, 0x4($a2)\n\t"
        "swc1 $f13, 0x24($sp)\n\t"
        "addiu $a0, $sp, 0x10\n\t"
        "swc1 $f12, 0x20($sp)\n\t"
        "sw $ra, 0x28($sp)\n\t"
        "jal drawing_0810\n\t"
        "addiu $a2, $sp, 0x1C\n\t"
        "lw $ra, 0x28($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

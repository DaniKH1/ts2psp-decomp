/**
 * The Sims 2 PSP - func_0016BC70 (0x16BC70, 0x18 bytes)
 *
 *     lw $a2, 0x0($a1)
 *     addiu $a0, $a0, 0x48
 *     lw $a1, 0x4($a1)
 *     sw $a2, 0x0($a0)
 *     jr $ra
 *     sw $a1, 0x4($a0)
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_0016BC70(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $a2, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x48\n\t"
        "lw $a1, 0x4($a1)\n\t"
        "sw $a2, 0x0($a0)\n\t"
        "jr $ra\n\t"
        "sw $a1, 0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

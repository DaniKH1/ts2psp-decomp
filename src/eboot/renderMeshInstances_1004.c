/**
 * The Sims 2 PSP - renderMeshInstances_1004 (0x1BEA04, 0x30 bytes)
 *
 *     sll $a0, $a0, 24
 *     mfc1 $a1, $f12
 *     lui $a2, 0x2D00
 *     addu $a0, $a0, $a2
 *     srl $a1, $a1, 8
 *     or $a0, $a0, $a1
 *     lui $a1, %%hi(sym_001DB710)
 *     lw $a2, %%lo(sym_001DB710)($a1)
 *     sw $a0, 0x0($a2)
 *     addiu $a0, $a2, 0x4
 *     jr $ra
 *     sw $a0, %%lo(sym_001DB710)($a1)
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_1004(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "sll $a0, $a0, 24\n\t"
        "mfc1 $a1, $f12\n\t"
        "lui $a2, 0x2D00\n\t"
        "addu $a0, $a0, $a2\n\t"
        "srl $a1, $a1, 8\n\t"
        "or $a0, $a0, $a1\n\t"
        "lui $a1, %%hi(sym_001DB710)\n\t"
        "lw $a2, %%lo(sym_001DB710)($a1)\n\t"
        "sw $a0, 0x0($a2)\n\t"
        "addiu $a0, $a2, 0x4\n\t"
        "jr $ra\n\t"
        "sw $a0, %%lo(sym_001DB710)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - renderMeshInstances_113C (0x1BEB3C, 0xE8 bytes)
 *
 *     addiu $sp, $sp, -0x40
 *     lv.q R000, 0x0($a1)
 *     lv.q R001, 0x10($a1)
 *     lv.q R002, 0x20($a1)
 *     lv.q R003, 0x30($a1)
 *     lv.q R100, 0x0($a2)
 *     lv.q R101, 0x10($a2)
 *     lv.q R102, 0x20($a2)
 *     lv.q R103, 0x30($a2)
 *     viim.s S400, 32
 *     vtfm4.q R200, M100, R000
 *     vtfm4.q R201, M100, R001
 *     vtfm4.q R202, M100, R002
 *     vtfm4.q R203, M100, R003
 *     vpfxt X, -X, -X, W
 *     vmul.t R200, R200, R400
 *     vpfxt -X, X, X, W
 *     vmul.t R201, R201, R400
 *     vpfxt -X, X, X, W
 *     vmul.t R202, R202, R400
 *     vneg.s S203, S203
 *     sv.q R200, 0x0($sp)
 *     sv.q R201, 0x10($sp)
 *     sv.q R202, 0x20($sp)
 *     sv.q R203, 0x30($sp)
 *     lui $a0, %%hi(sym_001DB710)
 *     lw $a1, %%lo(sym_001DB710)($a0)
 *     lui $a2, 0x2B00
 *     lui $a3, 0x2B00
 *     lui $t0, 0x2B00
 *     lwr $a2, 0x1($sp)
 *     lwr $a3, 0x5($sp)
 *     lwr $t0, 0x9($sp)
 *     sw $a2, 0x0($a1)
 *     sw $a3, 0x4($a1)
 *     sw $t0, 0x8($a1)
 *     lwr $a2, 0x11($sp)
 *     lwr $a3, 0x15($sp)
 *     lwr $t0, 0x19($sp)
 *     sw $a2, 0xC($a1)
 *     sw $a3, 0x10($a1)
 *     sw $t0, 0x14($a1)
 *     lwr $a2, 0x21($sp)
 *     lwr $a3, 0x25($sp)
 *     lwr $t0, 0x29($sp)
 *     sw $a2, 0x18($a1)
 *     sw $a3, 0x1C($a1)
 *     sw $t0, 0x20($a1)
 *     lwr $a2, 0x31($sp)
 *     lwr $a3, 0x35($sp)
 *     lwr $t0, 0x39($sp)
 *     sw $a2, 0x24($a1)
 *     sw $a3, 0x28($a1)
 *     sw $t0, 0x2C($a1)
 *     addiu $a1, $a1, 0x30
 *     sw $a1, %%lo(sym_001DB710)($a0)
 *     jr $ra
 *     addiu $sp, $sp, 0x40
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_113C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "lv.q R000, 0x0($a1)\n\t"
        "lv.q R001, 0x10($a1)\n\t"
        "lv.q R002, 0x20($a1)\n\t"
        "lv.q R003, 0x30($a1)\n\t"
        "lv.q R100, 0x0($a2)\n\t"
        "lv.q R101, 0x10($a2)\n\t"
        "lv.q R102, 0x20($a2)\n\t"
        "lv.q R103, 0x30($a2)\n\t"
        "viim.s S400, 32\n\t"
        "vtfm4.q R200, M100, R000\n\t"
        "vtfm4.q R201, M100, R001\n\t"
        "vtfm4.q R202, M100, R002\n\t"
        "vtfm4.q R203, M100, R003\n\t"
        "vpfxt X, -X, -X, W\n\t"
        "vmul.t R200, R200, R400\n\t"
        "vpfxt -X, X, X, W\n\t"
        "vmul.t R201, R201, R400\n\t"
        "vpfxt -X, X, X, W\n\t"
        "vmul.t R202, R202, R400\n\t"
        "vneg.s S203, S203\n\t"
        "sv.q R200, 0x0($sp)\n\t"
        "sv.q R201, 0x10($sp)\n\t"
        "sv.q R202, 0x20($sp)\n\t"
        "sv.q R203, 0x30($sp)\n\t"
        "lui $a0, %%hi(sym_001DB710)\n\t"
        "lw $a1, %%lo(sym_001DB710)($a0)\n\t"
        "lui $a2, 0x2B00\n\t"
        "lui $a3, 0x2B00\n\t"
        "lui $t0, 0x2B00\n\t"
        "lwr $a2, 0x1($sp)\n\t"
        "lwr $a3, 0x5($sp)\n\t"
        "lwr $t0, 0x9($sp)\n\t"
        "sw $a2, 0x0($a1)\n\t"
        "sw $a3, 0x4($a1)\n\t"
        "sw $t0, 0x8($a1)\n\t"
        "lwr $a2, 0x11($sp)\n\t"
        "lwr $a3, 0x15($sp)\n\t"
        "lwr $t0, 0x19($sp)\n\t"
        "sw $a2, 0xC($a1)\n\t"
        "sw $a3, 0x10($a1)\n\t"
        "sw $t0, 0x14($a1)\n\t"
        "lwr $a2, 0x21($sp)\n\t"
        "lwr $a3, 0x25($sp)\n\t"
        "lwr $t0, 0x29($sp)\n\t"
        "sw $a2, 0x18($a1)\n\t"
        "sw $a3, 0x1C($a1)\n\t"
        "sw $t0, 0x20($a1)\n\t"
        "lwr $a2, 0x31($sp)\n\t"
        "lwr $a3, 0x35($sp)\n\t"
        "lwr $t0, 0x39($sp)\n\t"
        "sw $a2, 0x24($a1)\n\t"
        "sw $a3, 0x28($a1)\n\t"
        "sw $t0, 0x2C($a1)\n\t"
        "addiu $a1, $a1, 0x30\n\t"
        "sw $a1, %%lo(sym_001DB710)($a0)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - renderCommon_1398 (0x1BD598, 0x168 bytes)
 *
 *     lui $a0, %%hi(sym_000EC7E8)
 *     addiu $a0, $a0, %%lo(sym_000EC7E8)
 *     lv.q R300, 0x0($a0)
 *     lui $a0, %%hi(sym_000E69A8)
 *     addiu $a0, $a0, %%lo(sym_000E69A8)
 *     lv.q R500, 0x0($a0)
 *     lv.q R501, 0x10($a0)
 *     lv.q R502, 0x20($a0)
 *     lv.q R503, 0x30($a0)
 *     lui $a0, %%hi(sym_000E22E8)
 *     addiu $a0, $a0, %%lo(sym_000E22E8)
 *     lv.q R600, 0x0($a0)
 *     lv.q R601, 0x10($a0)
 *     lv.q R602, 0x20($a0)
 *     lv.q R603, 0x30($a0)
 *     vzero.t C430
 *     vmscl.t M400, M500, S330
 *     vhtfm4.q R403, M500, R300
 *     lui $a0, %%hi(sym_000EC728)
 *     addiu $a0, $a0, %%lo(sym_000EC728)
 *     lv.q R200, 0x0($a0)
 *     lv.q R201, 0x10($a0)
 *     lv.q R202, 0x20($a0)
 *     lv.q R203, 0x30($a0)
 *     vmmul.q M100, M400, M600
 *     vcmp.q ne, R100, R200
 *     vsync
 *     mfvc $a1, $131
 *     vcmp.q ne, R101, R201
 *     vsync
 *     mfvc $a2, $131
 *     vcmp.q ne, R102, R202
 *     or $a1, $a1, $a2
 *     vsync
 *     mfvc $a2, $131
 *     vcmp.q ne, R103, R203
 *     or $a1, $a1, $a2
 *     vsync
 *     mfvc $a2, $131
 *     or $a1, $a1, $a2
 *     beqz $a1, .Leboot_001BD6F8
 *     nop
 *     sv.q R100, 0x0($a0)
 *     sv.q R101, 0x10($a0)
 *     sv.q R102, 0x20($a0)
 *     sv.q R103, 0x30($a0)
 *     lui $a1, %%hi(sym_000EC6E8)
 *     addiu $a1, $a1, %%lo(sym_000EC6E8)
 *     sv.q R400, 0x0($a1)
 *     sv.q R401, 0x10($a1)
 *     sv.q R402, 0x20($a1)
 *     sv.q R403, 0x30($a1)
 *     lui $a1, %%hi(sym_001DB710)
 *     lw $a2, %%lo(sym_001DB710)($a1)
 *     lui $a3, 0x3A00
 *     lui $t0, 0x3B00
 *     lui $t1, 0x3B00
 *     lui $t2, 0x3B00
 *     sw $a3, 0x0($a2)
 *     lwr $t0, 0x1($a0)
 *     lwr $t1, 0x5($a0)
 *     lwr $t2, 0x9($a0)
 *     sw $t0, 0x4($a2)
 *     sw $t1, 0x8($a2)
 *     sw $t2, 0xC($a2)
 *     lwr $t0, 0x11($a0)
 *     lwr $t1, 0x15($a0)
 *     lwr $t2, 0x19($a0)
 *     sw $t0, 0x10($a2)
 *     sw $t1, 0x14($a2)
 *     sw $t2, 0x18($a2)
 *     lwr $t0, 0x21($a0)
 *     lwr $t1, 0x25($a0)
 *     lwr $t2, 0x29($a0)
 *     sw $t0, 0x1C($a2)
 *     sw $t1, 0x20($a2)
 *     sw $t2, 0x24($a2)
 *     lwr $t0, 0x31($a0)
 *     lwr $t1, 0x35($a0)
 *     lwr $t2, 0x39($a0)
 *     sw $t0, 0x28($a2)
 *     sw $t1, 0x2C($a2)
 *     sw $t2, 0x30($a2)
 *     addiu $a0, $a2, 0x34
 *     sw $a0, %%lo(sym_001DB710)($a1)
 *     ori $a0, $zero, 0x1
 *     lui $a1, %%hi(sym_001DB700)
 *     sb $a0, %%lo(sym_001DB700)($a1)
 *   .Leboot_001BD6F8
 *     jr $ra
 *     nop
 *
 * renderCommon: one phase of the common render path.
 */

#include "types.h"

__attribute__((noreturn)) void renderCommon_1398(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui $a0, %%hi(sym_000EC7E8)\n\t"
        "addiu $a0, $a0, %%lo(sym_000EC7E8)\n\t"
        "lv.q R300, 0x0($a0)\n\t"
        "lui $a0, %%hi(sym_000E69A8)\n\t"
        "addiu $a0, $a0, %%lo(sym_000E69A8)\n\t"
        "lv.q R500, 0x0($a0)\n\t"
        "lv.q R501, 0x10($a0)\n\t"
        "lv.q R502, 0x20($a0)\n\t"
        "lv.q R503, 0x30($a0)\n\t"
        "lui $a0, %%hi(sym_000E22E8)\n\t"
        "addiu $a0, $a0, %%lo(sym_000E22E8)\n\t"
        "lv.q R600, 0x0($a0)\n\t"
        "lv.q R601, 0x10($a0)\n\t"
        "lv.q R602, 0x20($a0)\n\t"
        "lv.q R603, 0x30($a0)\n\t"
        "vzero.t C430\n\t"
        "vmscl.t M400, M500, S330\n\t"
        "vhtfm4.q R403, M500, R300\n\t"
        "lui $a0, %%hi(sym_000EC728)\n\t"
        "addiu $a0, $a0, %%lo(sym_000EC728)\n\t"
        "lv.q R200, 0x0($a0)\n\t"
        "lv.q R201, 0x10($a0)\n\t"
        "lv.q R202, 0x20($a0)\n\t"
        "lv.q R203, 0x30($a0)\n\t"
        "vmmul.q M100, M400, M600\n\t"
        "vcmp.q ne, R100, R200\n\t"
        "vsync\n\t"
        "mfvc $a1, $131\n\t"
        "vcmp.q ne, R101, R201\n\t"
        "vsync\n\t"
        "mfvc $a2, $131\n\t"
        "vcmp.q ne, R102, R202\n\t"
        "or $a1, $a1, $a2\n\t"
        "vsync\n\t"
        "mfvc $a2, $131\n\t"
        "vcmp.q ne, R103, R203\n\t"
        "or $a1, $a1, $a2\n\t"
        "vsync\n\t"
        "mfvc $a2, $131\n\t"
        "or $a1, $a1, $a2\n\t"
        "beqz $a1, .Leboot_001BD6F8\n\t"
        "nop\n\t"
        "sv.q R100, 0x0($a0)\n\t"
        "sv.q R101, 0x10($a0)\n\t"
        "sv.q R102, 0x20($a0)\n\t"
        "sv.q R103, 0x30($a0)\n\t"
        "lui $a1, %%hi(sym_000EC6E8)\n\t"
        "addiu $a1, $a1, %%lo(sym_000EC6E8)\n\t"
        "sv.q R400, 0x0($a1)\n\t"
        "sv.q R401, 0x10($a1)\n\t"
        "sv.q R402, 0x20($a1)\n\t"
        "sv.q R403, 0x30($a1)\n\t"
        "lui $a1, %%hi(sym_001DB710)\n\t"
        "lw $a2, %%lo(sym_001DB710)($a1)\n\t"
        "lui $a3, 0x3A00\n\t"
        "lui $t0, 0x3B00\n\t"
        "lui $t1, 0x3B00\n\t"
        "lui $t2, 0x3B00\n\t"
        "sw $a3, 0x0($a2)\n\t"
        "lwr $t0, 0x1($a0)\n\t"
        "lwr $t1, 0x5($a0)\n\t"
        "lwr $t2, 0x9($a0)\n\t"
        "sw $t0, 0x4($a2)\n\t"
        "sw $t1, 0x8($a2)\n\t"
        "sw $t2, 0xC($a2)\n\t"
        "lwr $t0, 0x11($a0)\n\t"
        "lwr $t1, 0x15($a0)\n\t"
        "lwr $t2, 0x19($a0)\n\t"
        "sw $t0, 0x10($a2)\n\t"
        "sw $t1, 0x14($a2)\n\t"
        "sw $t2, 0x18($a2)\n\t"
        "lwr $t0, 0x21($a0)\n\t"
        "lwr $t1, 0x25($a0)\n\t"
        "lwr $t2, 0x29($a0)\n\t"
        "sw $t0, 0x1C($a2)\n\t"
        "sw $t1, 0x20($a2)\n\t"
        "sw $t2, 0x24($a2)\n\t"
        "lwr $t0, 0x31($a0)\n\t"
        "lwr $t1, 0x35($a0)\n\t"
        "lwr $t2, 0x39($a0)\n\t"
        "sw $t0, 0x28($a2)\n\t"
        "sw $t1, 0x2C($a2)\n\t"
        "sw $t2, 0x30($a2)\n\t"
        "addiu $a0, $a2, 0x34\n\t"
        "sw $a0, %%lo(sym_001DB710)($a1)\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "lui $a1, %%hi(sym_001DB700)\n\t"
        "sb $a0, %%lo(sym_001DB700)($a1)\n\t"
        ".Leboot_001BD6F8:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

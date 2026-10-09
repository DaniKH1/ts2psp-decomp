/**
 * The Sims 2 PSP - renderCommon_01B8 (0x1BC3B8, 0xEC bytes)
 *
 *     andi $a0, $a0, 0xFF
 *     vidt.q R103
 *     lui $a2, %%hi(sym_000E22E8)
 *     addiu $a2, $a2, %%lo(sym_000E22E8)
 *     lv.q R100, 0x0($a2)
 *     lv.q R101, 0x10($a2)
 *     lv.q R102, 0x20($a2)
 *     sv.q R103, 0x30($a1)
 *     beqz $a0, .Leboot_001BC490
 *     nop
 *     lui $a0, %%hi(sym_000EC768)
 *     addiu $a0, $a0, %%lo(sym_000EC768)
 *     lv.q R300, 0x0($a0)
 *     lui $a0, %%hi(sym_000E69A8)
 *     addiu $a0, $a0, %%lo(sym_000E69A8)
 *     lv.q R200, 0x0($a0)
 *     lv.q R201, 0x10($a0)
 *     lv.q R202, 0x20($a0)
 *     lv.q R203, 0x30($a0)
 *     lui $a0, %%hi(sym_000E22A8)
 *     addiu $a0, $a0, %%lo(sym_000E22A8)
 *     lv.q R001, 0x20($a0)
 *     lv.q R002, 0x30($a0)
 *     vhtfm4.q R000, M200, R300
 *     vsub.t R000, R002, R000
 *     vdot.t S030, R000, R000
 *     vrsq.s S030, S030
 *     vscl.t R000, R000, S030
 *     vcrsp.t R403, R000, R001
 *     vdot.t S430, R000, R001
 *     vdot.t S433, R403, R403
 *     vrsq.s S433, S433
 *     vscl.t R403, R403, S433
 *     vocp.s S432, S430
 *     vmul.s S002, S430, S430
 *     vocp.s S002, S002
 *     vsqrt.s S002, S002
 *     vscl.t R401, R403, S432
 *     vscl.t R400, R403, S002
 *     vscl.t R000, R401, S403
 *     vscl.t R001, R401, S413
 *     vscl.t R002, R401, S423
 *     vpfxt W, Z, -Y, X
 *     vadd.q R000, R000, R400
 *     vpfxt -Z, W, X, X
 *     vadd.q R001, R001, R400
 *     vpfxt Y, -X, W, X
 *     vadd.q R002, R002, R400
 *     vmmul.t M200, M000, M100
 *     sv.q R200, 0x0($a1)
 *     sv.q R201, 0x10($a1)
 *     sv.q R202, 0x20($a1)
 *     b .Leboot_001BC49C
 *     nop
 *   .Leboot_001BC490
 *     sv.q R100, 0x0($a1)
 *     sv.q R101, 0x10($a1)
 *     sv.q R102, 0x20($a1)
 *   .Leboot_001BC49C
 *     jr $ra
 *     nop
 *
 * renderCommon: one phase of the common render path.
 */

#include "types.h"

__attribute__((noreturn)) void renderCommon_01B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "vidt.q R103\n\t"
        "lui $a2, %%hi(sym_000E22E8)\n\t"
        "addiu $a2, $a2, %%lo(sym_000E22E8)\n\t"
        "lv.q R100, 0x0($a2)\n\t"
        "lv.q R101, 0x10($a2)\n\t"
        "lv.q R102, 0x20($a2)\n\t"
        "sv.q R103, 0x30($a1)\n\t"
        "beqz $a0, .Leboot_001BC490\n\t"
        "nop\n\t"
        "lui $a0, %%hi(sym_000EC768)\n\t"
        "addiu $a0, $a0, %%lo(sym_000EC768)\n\t"
        "lv.q R300, 0x0($a0)\n\t"
        "lui $a0, %%hi(sym_000E69A8)\n\t"
        "addiu $a0, $a0, %%lo(sym_000E69A8)\n\t"
        "lv.q R200, 0x0($a0)\n\t"
        "lv.q R201, 0x10($a0)\n\t"
        "lv.q R202, 0x20($a0)\n\t"
        "lv.q R203, 0x30($a0)\n\t"
        "lui $a0, %%hi(sym_000E22A8)\n\t"
        "addiu $a0, $a0, %%lo(sym_000E22A8)\n\t"
        "lv.q R001, 0x20($a0)\n\t"
        "lv.q R002, 0x30($a0)\n\t"
        "vhtfm4.q R000, M200, R300\n\t"
        "vsub.t R000, R002, R000\n\t"
        "vdot.t S030, R000, R000\n\t"
        "vrsq.s S030, S030\n\t"
        "vscl.t R000, R000, S030\n\t"
        "vcrsp.t R403, R000, R001\n\t"
        "vdot.t S430, R000, R001\n\t"
        "vdot.t S433, R403, R403\n\t"
        "vrsq.s S433, S433\n\t"
        "vscl.t R403, R403, S433\n\t"
        "vocp.s S432, S430\n\t"
        "vmul.s S002, S430, S430\n\t"
        "vocp.s S002, S002\n\t"
        "vsqrt.s S002, S002\n\t"
        "vscl.t R401, R403, S432\n\t"
        "vscl.t R400, R403, S002\n\t"
        "vscl.t R000, R401, S403\n\t"
        "vscl.t R001, R401, S413\n\t"
        "vscl.t R002, R401, S423\n\t"
        "vpfxt W, Z, -Y, X\n\t"
        "vadd.q R000, R000, R400\n\t"
        "vpfxt -Z, W, X, X\n\t"
        "vadd.q R001, R001, R400\n\t"
        "vpfxt Y, -X, W, X\n\t"
        "vadd.q R002, R002, R400\n\t"
        "vmmul.t M200, M000, M100\n\t"
        "sv.q R200, 0x0($a1)\n\t"
        "sv.q R201, 0x10($a1)\n\t"
        "sv.q R202, 0x20($a1)\n\t"
        "b .Leboot_001BC49C\n\t"
        "nop\n\t"
        ".Leboot_001BC490:\n\t"
        "sv.q R100, 0x0($a1)\n\t"
        "sv.q R101, 0x10($a1)\n\t"
        "sv.q R102, 0x20($a1)\n\t"
        ".Leboot_001BC49C:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

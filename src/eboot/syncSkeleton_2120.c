/**
 * The Sims 2 PSP - syncSkeleton_2120 (0x1BAB3C, 0x80 bytes)
 *
 *     lui $a2, 0x80
 *     lv.s S032, 0xC($a0)
 *     vidt.q R003
 *     mfc1 $a3, $f12
 *     mtv $a2, S030
 *     vsat1.s S001, S032
 *     vcmp.s lt, S032, S003
 *     mtv $a3, S020
 *     lv.s S002, 0x0($a0)
 *     vmul.s S001, S001, S001
 *     lv.s S012, 0x4($a0)
 *     vneg.s S010, S020
 *     lv.s S022, 0x8($a0)
 *     vocp.s S001, S001
 *     vsqrt.s S001, S001
 *     vcmovt.s S020, S010, 0
 *     vasin.s S001, S001
 *     vmul.s S001, S001, S020
 *     vdot.t S021, R002, R002
 *     vcos.s S130, S001
 *     vsin.s S011, S001
 *     vmul.s S031, S021, S001
 *     vrsq.s S021, S021
 *     vabs.s S031, S031
 *     vcmp.s lt, S031, S030
 *     vscl.t R100, R002, S011
 *     vscl.t R100, R100, S021
 *     vcmovt.q R100, R003, 0
 *     svr.q R100, 0x0($a1)
 *     svl.q R100, 0xC($a1)
 *     jr $ra
 *     nop
 *
 * syncSkeleton: VFPU path - lv.s, vidt.q, vsat1.s, vcmp.s and vmul.s over the object's floats.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_2120(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui $a2, 0x80\n\t"
        "lv.s S032, 0xC($a0)\n\t"
        "vidt.q R003\n\t"
        "mfc1 $a3, $f12\n\t"
        "mtv $a2, S030\n\t"
        "vsat1.s S001, S032\n\t"
        "vcmp.s lt, S032, S003\n\t"
        "mtv $a3, S020\n\t"
        "lv.s S002, 0x0($a0)\n\t"
        "vmul.s S001, S001, S001\n\t"
        "lv.s S012, 0x4($a0)\n\t"
        "vneg.s S010, S020\n\t"
        "lv.s S022, 0x8($a0)\n\t"
        "vocp.s S001, S001\n\t"
        "vsqrt.s S001, S001\n\t"
        "vcmovt.s S020, S010, 0\n\t"
        "vasin.s S001, S001\n\t"
        "vmul.s S001, S001, S020\n\t"
        "vdot.t S021, R002, R002\n\t"
        "vcos.s S130, S001\n\t"
        "vsin.s S011, S001\n\t"
        "vmul.s S031, S021, S001\n\t"
        "vrsq.s S021, S021\n\t"
        "vabs.s S031, S031\n\t"
        "vcmp.s lt, S031, S030\n\t"
        "vscl.t R100, R002, S011\n\t"
        "vscl.t R100, R100, S021\n\t"
        "vcmovt.q R100, R003, 0\n\t"
        "svr.q R100, 0x0($a1)\n\t"
        "svl.q R100, 0xC($a1)\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - syncSkeleton_21A0 (0x1BABBC, 0x60 bytes)
 *
 *     lv.s S130, 0x0($a0)
 *     lv.s S131, 0x4($a0)
 *     lv.s S132, 0x8($a0)
 *     lv.s S133, 0xC($a0)
 *     vpfxs W, Z, -Y, -X
 *     vmov.q C100, C130
 *     vpfxs -Z, W, X, -Y
 *     vmov.q C110, C130
 *     vpfxs Y, -X, W, -Z
 *     vmov.q C120, C130
 *     vpfxs X, W, Z, -Y
 *     vmov.q C200, C130
 *     vpfxs Y, -Z, W, X
 *     vmov.q C210, C130
 *     vpfxs Z, Y, -X, W
 *     vmov.q C220, C130
 *     vpfxs W, -X, -Y, -Z
 *     vmov.q C230, C130
 *     vmmul.q E000, E100, E200
 *     svl.q C000, 0x8($a1)
 *     svl.q C010, 0x18($a1)
 *     svl.q C020, 0x28($a1)
 *     jr $ra
 *     nop
 *
 * syncSkeleton: VFPU path - lv.s loads four floats from the object, vpfxs swizzles, vmov.q moves a quad into a scratch register. The only use of the PSP vector unit in this batch.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_21A0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lv.s S130, 0x0($a0)\n\t"
        "lv.s S131, 0x4($a0)\n\t"
        "lv.s S132, 0x8($a0)\n\t"
        "lv.s S133, 0xC($a0)\n\t"
        "vpfxs W, Z, -Y, -X\n\t"
        "vmov.q C100, C130\n\t"
        "vpfxs -Z, W, X, -Y\n\t"
        "vmov.q C110, C130\n\t"
        "vpfxs Y, -X, W, -Z\n\t"
        "vmov.q C120, C130\n\t"
        "vpfxs X, W, Z, -Y\n\t"
        "vmov.q C200, C130\n\t"
        "vpfxs Y, -Z, W, X\n\t"
        "vmov.q C210, C130\n\t"
        "vpfxs Z, Y, -X, W\n\t"
        "vmov.q C220, C130\n\t"
        "vpfxs W, -X, -Y, -Z\n\t"
        "vmov.q C230, C130\n\t"
        "vmmul.q E000, E100, E200\n\t"
        "svl.q C000, 0x8($a1)\n\t"
        "svl.q C010, 0x18($a1)\n\t"
        "svl.q C020, 0x28($a1)\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - syncSkeleton_2260 (0x1BAC7C, 0x58 bytes)
 *
 *     lwc1 $f12, 0x4($a0)
 *     lwc1 $f13, 0x8($a0)
 *     add.s $f14, $f12, $f12
 *     lwc1 $f15, 0x0($a0)
 *     lwc1 $f17, 0xC($a0)
 *     add.s $f16, $f13, $f13
 *     lui $a0, 0x3F80
 *     mul.s $f18, $f14, $f17
 *     mul.s $f12, $f14, $f12
 *     mul.s $f13, $f16, $f13
 *     mtc1 $a0, $f19
 *     mul.s $f17, $f16, $f17
 *     mul.s $f14, $f14, $f15
 *     add.s $f12, $f12, $f13
 *     mul.s $f15, $f16, $f15
 *     add.s $f14, $f17, $f14
 *     sub.s $f12, $f19, $f12
 *     sub.s $f15, $f15, $f18
 *     swc1 $f14, 0x4($a1)
 *     swc1 $f15, 0x8($a1)
 *     jr $ra
 *     swc1 $f12, 0x0($a1)
 *
 * syncSkeleton: read four floats from 0x0/0x4/0x8/0xC, double two of them, and compare against 1.0f.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_2260(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "lwc1 $f13, 0x8($a0)\n\t"
        "add.s $f14, $f12, $f12\n\t"
        "lwc1 $f15, 0x0($a0)\n\t"
        "lwc1 $f17, 0xC($a0)\n\t"
        "add.s $f16, $f13, $f13\n\t"
        "lui $a0, 0x3F80\n\t"
        "mul.s $f18, $f14, $f17\n\t"
        "mul.s $f12, $f14, $f12\n\t"
        "mul.s $f13, $f16, $f13\n\t"
        "mtc1 $a0, $f19\n\t"
        "mul.s $f17, $f16, $f17\n\t"
        "mul.s $f14, $f14, $f15\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "mul.s $f15, $f16, $f15\n\t"
        "add.s $f14, $f17, $f14\n\t"
        "sub.s $f12, $f19, $f12\n\t"
        "sub.s $f15, $f15, $f18\n\t"
        "swc1 $f14, 0x4($a1)\n\t"
        "swc1 $f15, 0x8($a1)\n\t"
        "jr $ra\n\t"
        "swc1 $f12, 0x0($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - sortAndCullScene_1844 (0x1B5460, 0x98 bytes)
 *
 *     lwc1 $f12, 0x0($a0)
 *     lwc1 $f14, 0x0($a2)
 *     addiu $a3, $a0, 0xC
 *     c.le.s $f14, $f12
 *     nop
 *     bc1t .Leboot_001B5490
 *     lwc1 $f13, 0x0($a3)
 *     mov.s $f12, $f13
 *     c.lt.s $f14, $f12
 *     nop
 *     bc1tl .Leboot_001B5490
 *     mov.s $f12, $f14
 *   .Leboot_001B5490
 *     lwc1 $f13, 0x4($a0)
 *     lwc1 $f15, 0x4($a2)
 *     c.le.s $f15, $f13
 *     nop
 *     bc1t .Leboot_001B54BC
 *     lwc1 $f14, 0x4($a3)
 *     mov.s $f13, $f14
 *     c.lt.s $f15, $f13
 *     nop
 *     bc1tl .Leboot_001B54BC
 *     mov.s $f13, $f15
 *   .Leboot_001B54BC
 *     lwc1 $f14, 0x8($a0)
 *     lwc1 $f16, 0x8($a2)
 *     c.le.s $f16, $f14
 *     nop
 *     bc1t .Leboot_001B54E8
 *     lwc1 $f15, 0x8($a3)
 *     mov.s $f14, $f15
 *     c.lt.s $f16, $f14
 *     nop
 *     bc1tl .Leboot_001B54E8
 *     mov.s $f14, $f16
 *   .Leboot_001B54E8
 *     swc1 $f12, 0x0($a1)
 *     swc1 $f13, 0x4($a1)
 *     jr $ra
 *     swc1 $f14, 0x8($a1)
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1844(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f14, 0x0($a2)\n\t"
        "addiu $a3, $a0, 0xC\n\t"
        "c.le.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B5490\n\t"
        "lwc1 $f13, 0x0($a3)\n\t"
        "mov.s $f12, $f13\n\t"
        "c.lt.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B5490\n\t"
        "mov.s $f12, $f14\n\t"
        ".Leboot_001B5490:\n\t"
        "lwc1 $f13, 0x4($a0)\n\t"
        "lwc1 $f15, 0x4($a2)\n\t"
        "c.le.s $f15, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B54BC\n\t"
        "lwc1 $f14, 0x4($a3)\n\t"
        "mov.s $f13, $f14\n\t"
        "c.lt.s $f15, $f13\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B54BC\n\t"
        "mov.s $f13, $f15\n\t"
        ".Leboot_001B54BC:\n\t"
        "lwc1 $f14, 0x8($a0)\n\t"
        "lwc1 $f16, 0x8($a2)\n\t"
        "c.le.s $f16, $f14\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B54E8\n\t"
        "lwc1 $f15, 0x8($a3)\n\t"
        "mov.s $f14, $f15\n\t"
        "c.lt.s $f16, $f14\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B54E8\n\t"
        "mov.s $f14, $f16\n\t"
        ".Leboot_001B54E8:\n\t"
        "swc1 $f12, 0x0($a1)\n\t"
        "swc1 $f13, 0x4($a1)\n\t"
        "jr $ra\n\t"
        "swc1 $f14, 0x8($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

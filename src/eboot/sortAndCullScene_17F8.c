/**
 * The Sims 2 PSP - sortAndCullScene_17F8 (0x1B5414, 0x4C bytes)
 *
 *     lui $a1, 0x3780
 *     mtc1 $a1, $f13
 *     c.le.s $f12, $f13
 *     nop
 *     bc1t .Leboot_001B544C
 *     nop
 *     mul.s $f12, $f12, $f12
 *     lwc1 $f13, 0x1C($a0)
 *     lui $a0, 0x3F80
 *     mtc1 $a0, $f0
 *     mul.s $f12, $f12, $f13
 *     div.s $f0, $f0, $f12
 *     b .Leboot_001B5458
 *     nop
 *   .Leboot_001B544C
 *     lui $a0, 0x7F7F
 *     ori $a0, $a0, (0x7F7FFFFF & 0xFFFF)
 *     mtc1 $a0, $f0
 *   .Leboot_001B5458
 *     jr $ra
 *     nop
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_17F8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui $a1, 0x3780\n\t"
        "mtc1 $a1, $f13\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B544C\n\t"
        "nop\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "lwc1 $f13, 0x1C($a0)\n\t"
        "lui $a0, 0x3F80\n\t"
        "mtc1 $a0, $f0\n\t"
        "mul.s $f12, $f12, $f13\n\t"
        "div.s $f0, $f0, $f12\n\t"
        "b .Leboot_001B5458\n\t"
        "nop\n\t"
        ".Leboot_001B544C:\n\t"
        "lui $a0, 0x7F7F\n\t"
        "ori $a0, $a0, (0x7F7FFFFF & 0xFFFF)\n\t"
        "mtc1 $a0, $f0\n\t"
        ".Leboot_001B5458:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

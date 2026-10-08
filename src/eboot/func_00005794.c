/**
 * The Sims 2 PSP - func_00005794 (0x00005794, 0x3C bytes)
 *
 *     lw    $a1, 0xC($a0)
 *     lw    $a1, 0x128($a1)
 *     lwc1  $f13, 0x484($a1)
 *     c.eq.s $f12, $f13
 *     nop
 *     bc1fl .Leboot_000057C0
 *       lwc1  $f13, 0x47C($a1)
 *     lui   $a1, (0x3F800000 >> 16)
 *     mtc1  $a1, $f12
 *     b     .Leboot_000057C8
 *       swc1 $f12, 0x18($a0)
 *   .Leboot_000057C0:
 *     div.s $f12, $f12, $f13
 *     swc1  $f12, 0x18($a0)
 *   .Leboot_000057C8:
 *     jr    $ra
 *     nop
 *
 * This function compares $f12 with a float loaded from a nested
 * structure. If equal, it loads 1.0f and stores it. Otherwise,
 * it divides $f12 by the loaded value and stores the result.
 */
#include "types.h"

__attribute__((noreturn)) void func_00005794(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw    $a1, 0xC($a0)\n\t"
        "lw    $a1, 0x128($a1)\n\t"
        "lwc1  $f13, 0x484($a1)\n\t"
        "c.eq.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1fl .Leboot_000057C0\n\t"
        "lwc1  $f13, 0x47C($a1)\n\t"
        "lui   $a1, (0x3F800000 >> 16)\n\t"
        "mtc1  $a1, $f12\n\t"
        "b     .Leboot_000057C8\n\t"
        "swc1  $f12, 0x18($a0)\n\t"
        ".Leboot_000057C0:\n\t"
        "div.s $f12, $f12, $f13\n\t"
        "swc1  $f12, 0x18($a0)\n\t"
        ".Leboot_000057C8:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
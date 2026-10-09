/**
 * The Sims 2 PSP - collision_1384 (0x1B1BA4, 0xAC bytes)
 *
 *     lwc1 $f14, 0x0($a0)
 *     lwc1 $f13, 0x0($a1)
 *     c.lt.s $f13, $f14
 *     nop
 *     bc1tl .Leboot_001B1BBC
 *     mov.s $f14, $f13
 *   .Leboot_001B1BBC
 *     swc1 $f14, 0x0($a0)
 *     lwc1 $f13, 0x4($a0)
 *     lwc1 $f14, 0x4($a1)
 *     c.lt.s $f14, $f13
 *     nop
 *     bc1tl .Leboot_001B1BD8
 *     mov.s $f13, $f14
 *   .Leboot_001B1BD8
 *     swc1 $f13, 0x4($a0)
 *     lwc1 $f12, 0x8($a0)
 *     lwc1 $f13, 0x8($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl .Leboot_001B1BF4
 *     mov.s $f12, $f13
 *   .Leboot_001B1BF4
 *     swc1 $f12, 0x8($a0)
 *     lwc1 $f12, 0xC($a0)
 *     lwc1 $f13, 0x0($a1)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B1C10
 *     mov.s $f12, $f13
 *   .Leboot_001B1C10
 *     swc1 $f12, 0xC($a0)
 *     lwc1 $f12, 0x10($a0)
 *     lwc1 $f14, 0x4($a1)
 *     c.le.s $f14, $f12
 *     nop
 *     bc1fl .Leboot_001B1C2C
 *     mov.s $f12, $f14
 *   .Leboot_001B1C2C
 *     swc1 $f12, 0x10($a0)
 *     lwc1 $f12, 0x14($a0)
 *     lwc1 $f13, 0x8($a1)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B1C48
 *     mov.s $f12, $f13
 *   .Leboot_001B1C48
 *     jr $ra
 *     swc1 $f12, 0x14($a0)
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_1384(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1 $f14, 0x0($a0)\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "c.lt.s $f13, $f14\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1BBC\n\t"
        "mov.s $f14, $f13\n\t"
        ".Leboot_001B1BBC:\n\t"
        "swc1 $f14, 0x0($a0)\n\t"
        "lwc1 $f13, 0x4($a0)\n\t"
        "lwc1 $f14, 0x4($a1)\n\t"
        "c.lt.s $f14, $f13\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1BD8\n\t"
        "mov.s $f13, $f14\n\t"
        ".Leboot_001B1BD8:\n\t"
        "swc1 $f13, 0x4($a0)\n\t"
        "lwc1 $f12, 0x8($a0)\n\t"
        "lwc1 $f13, 0x8($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1BF4\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1BF4:\n\t"
        "swc1 $f12, 0x8($a0)\n\t"
        "lwc1 $f12, 0xC($a0)\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1C10\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1C10:\n\t"
        "swc1 $f12, 0xC($a0)\n\t"
        "lwc1 $f12, 0x10($a0)\n\t"
        "lwc1 $f14, 0x4($a1)\n\t"
        "c.le.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1C2C\n\t"
        "mov.s $f12, $f14\n\t"
        ".Leboot_001B1C2C:\n\t"
        "swc1 $f12, 0x10($a0)\n\t"
        "lwc1 $f12, 0x14($a0)\n\t"
        "lwc1 $f13, 0x8($a1)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1C48\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1C48:\n\t"
        "jr $ra\n\t"
        "swc1 $f12, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

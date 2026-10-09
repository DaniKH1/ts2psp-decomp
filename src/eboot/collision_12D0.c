/**
 * The Sims 2 PSP - collision_12D0 (0x1B1AF0, 0xB4 bytes)
 *
 *     lwc1 $f12, 0x0($a2)
 *     lwc1 $f13, 0x0($a1)
 *     addiu $t0, $a1, 0xC
 *     addiu $a3, $a2, 0xC
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl .Leboot_001B1B10
 *     mov.s $f12, $f13
 *   .Leboot_001B1B10
 *     swc1 $f12, 0x0($a0)
 *     lwc1 $f12, 0x4($a2)
 *     lwc1 $f13, 0x4($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl .Leboot_001B1B2C
 *     mov.s $f12, $f13
 *   .Leboot_001B1B2C
 *     swc1 $f12, 0x4($a0)
 *     lwc1 $f12, 0x8($a2)
 *     lwc1 $f13, 0x8($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl .Leboot_001B1B48
 *     mov.s $f12, $f13
 *   .Leboot_001B1B48
 *     swc1 $f12, 0x8($a0)
 *     lwc1 $f12, 0x0($a3)
 *     lwc1 $f13, 0x0($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B1B64
 *     mov.s $f12, $f13
 *   .Leboot_001B1B64
 *     swc1 $f12, 0xC($a0)
 *     lwc1 $f12, 0x4($a3)
 *     lwc1 $f13, 0x4($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B1B80
 *     mov.s $f12, $f13
 *   .Leboot_001B1B80
 *     swc1 $f12, 0x10($a0)
 *     lwc1 $f12, 0x8($a3)
 *     lwc1 $f13, 0x8($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_001B1B9C
 *     mov.s $f12, $f13
 *   .Leboot_001B1B9C
 *     jr $ra
 *     swc1 $f12, 0x14($a0)
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_12D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "lwc1 $f13, 0x0($a1)\n\t"
        "addiu $t0, $a1, 0xC\n\t"
        "addiu $a3, $a2, 0xC\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1B10\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B10:\n\t"
        "swc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f12, 0x4($a2)\n\t"
        "lwc1 $f13, 0x4($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1B2C\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B2C:\n\t"
        "swc1 $f12, 0x4($a0)\n\t"
        "lwc1 $f12, 0x8($a2)\n\t"
        "lwc1 $f13, 0x8($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B1B48\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B48:\n\t"
        "swc1 $f12, 0x8($a0)\n\t"
        "lwc1 $f12, 0x0($a3)\n\t"
        "lwc1 $f13, 0x0($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1B64\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B64:\n\t"
        "swc1 $f12, 0xC($a0)\n\t"
        "lwc1 $f12, 0x4($a3)\n\t"
        "lwc1 $f13, 0x4($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1B80\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B80:\n\t"
        "swc1 $f12, 0x10($a0)\n\t"
        "lwc1 $f12, 0x8($a3)\n\t"
        "lwc1 $f13, 0x8($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001B1B9C\n\t"
        "mov.s $f12, $f13\n\t"
        ".Leboot_001B1B9C:\n\t"
        "jr $ra\n\t"
        "swc1 $f12, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - collision_1430 (0x1B1C50, 0xA4 bytes)
 *
 *     addiu $a3, $a1, 0xC
 *     lwc1 $f12, 0x0($a0)
 *     lwc1 $f13, 0x0($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     ori $t0, $zero, 0x0
 *     addiu $a2, $a0, 0xC
 *     lwc1 $f12, 0x0($a1)
 *     lwc1 $f13, 0x0($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     nop
 *     lwc1 $f12, 0x4($a0)
 *     lwc1 $f13, 0x4($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     nop
 *     lwc1 $f12, 0x4($a1)
 *     lwc1 $f13, 0x4($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     nop
 *     lwc1 $f12, 0x8($a0)
 *     lwc1 $f13, 0x8($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     nop
 *     lwc1 $f12, 0x8($a1)
 *     lwc1 $f13, 0x8($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f .Leboot_001B1CEC
 *     nop
 *     ori $t0, $zero, 0x1
 *   .Leboot_001B1CEC
 *     jr $ra
 *     andi $v0, $t0, 0xFF
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_1430(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $a3, $a1, 0xC\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f13, 0x0($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "ori $t0, $zero, 0x0\n\t"
        "addiu $a2, $a0, 0xC\n\t"
        "lwc1 $f12, 0x0($a1)\n\t"
        "lwc1 $f13, 0x0($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "lwc1 $f13, 0x4($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x4($a1)\n\t"
        "lwc1 $f13, 0x4($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x8($a0)\n\t"
        "lwc1 $f13, 0x8($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1 $f12, 0x8($a1)\n\t"
        "lwc1 $f13, 0x8($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "ori $t0, $zero, 0x1\n\t"
        ".Leboot_001B1CEC:\n\t"
        "jr $ra\n\t"
        "andi $v0, $t0, 0xFF\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

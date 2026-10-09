/**
 * The Sims 2 PSP - updateNodeGraph_036C (0x1B5BEC, 0x34 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lwc1 $f13, 0x44($a0)
 *     sw $ra, 0x10($sp)
 *     c.eq.s $f12, $f13
 *     nop
 *     bc1t .Leboot_001B5C14
 *     nop
 *     swc1 $f12, 0x44($a0)
 *     jal updateNodeGraph_03A0
 *     ori $a1, $zero, 0x1
 *   .Leboot_001B5C14
 *     lw $ra, 0x10($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: compare the float at 0x44 against the incoming one and store it if it differs.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_036C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lwc1 $f13, 0x44($a0)\n\t"
        "sw $ra, 0x10($sp)\n\t"
        "c.eq.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B5C14\n\t"
        "nop\n\t"
        "swc1 $f12, 0x44($a0)\n\t"
        "jal updateNodeGraph_03A0\n\t"
        "ori $a1, $zero, 0x1\n\t"
        ".Leboot_001B5C14:\n\t"
        "lw $ra, 0x10($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

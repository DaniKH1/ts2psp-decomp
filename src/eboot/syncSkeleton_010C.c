/**
 * The Sims 2 PSP - syncSkeleton_010C (0x1B8B28, 0xE0 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a3, 0x40($a0)
 *     sw $ra, 0x10($sp)
 *     lw $a2, 0x0($a3)
 *     ori $a1, $zero, 0x0
 *     slt $t0, $a1, $a2
 *     beqz $t0, .Leboot_001B8B94
 *     ori $t0, $zero, 0x0
 *   .Leboot_001B8B48
 *     ori $t1, $zero, 0x0
 *     ori $a3, $zero, 0x0
 *   .Leboot_001B8B50
 *     lw $t2, 0x48($a0)
 *     addu $t2, $t2, $t0
 *     lw $t3, 0xB0($t2)
 *     addu $t2, $t2, $a3
 *     addiu $t3, $t3, 0x30
 *     addu $t3, $t3, $a3
 *     lwc1 $f12, 0x0($t3)
 *     addiu $t1, $t1, 0x1
 *     swc1 $f12, 0x80($t2)
 *     slti $t2, $t1, 0xC
 *     bnez $t2, .Leboot_001B8B50
 *     addiu $a3, $a3, 0x4
 *     addiu $a1, $a1, 0x1
 *     slt $a3, $a1, $a2
 *     bnez $a3, .Leboot_001B8B48
 *     addiu $t0, $t0, 0xC0
 *     lw $a3, 0x40($a0)
 *   .Leboot_001B8B94
 *     addiu $a1, $a3, 0x10
 *     lw $a2, 0x0($a1)
 *     ori $a1, $zero, 0x0
 *     slt $a3, $a1, $a2
 *     beqz $a3, .Leboot_001B8BF4
 *     ori $t0, $zero, 0x0
 *   .Leboot_001B8BAC
 *     ori $t1, $zero, 0x0
 *     ori $a3, $zero, 0x0
 *   .Leboot_001B8BB4
 *     lw $t2, 0x4C($a0)
 *     addu $t2, $t2, $t0
 *     lw $t3, 0x0($t2)
 *     addu $t2, $t2, $a3
 *     addiu $t3, $t3, 0x4
 *     addu $t3, $t3, $a3
 *     lwc1 $f12, 0x0($t3)
 *     addiu $t1, $t1, 0x1
 *     swc1 $f12, 0x4($t2)
 *     slti $t2, $t1, 0xC
 *     bnez $t2, .Leboot_001B8BB4
 *     addiu $a3, $a3, 0x4
 *     addiu $a1, $a1, 0x1
 *     slt $a3, $a1, $a2
 *     bnez $a3, .Leboot_001B8BAC
 *     addiu $t0, $t0, 0x34
 *   .Leboot_001B8BF4
 *     jal syncSkeleton_01EC
 *     nop
 *     lw $ra, 0x10($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_010C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a3, 0x40($a0)\n\t"
        "sw $ra, 0x10($sp)\n\t"
        "lw $a2, 0x0($a3)\n\t"
        "ori $a1, $zero, 0x0\n\t"
        "slt $t0, $a1, $a2\n\t"
        "beqz $t0, .Leboot_001B8B94\n\t"
        "ori $t0, $zero, 0x0\n\t"
        ".Leboot_001B8B48:\n\t"
        "ori $t1, $zero, 0x0\n\t"
        "ori $a3, $zero, 0x0\n\t"
        ".Leboot_001B8B50:\n\t"
        "lw $t2, 0x48($a0)\n\t"
        "addu $t2, $t2, $t0\n\t"
        "lw $t3, 0xB0($t2)\n\t"
        "addu $t2, $t2, $a3\n\t"
        "addiu $t3, $t3, 0x30\n\t"
        "addu $t3, $t3, $a3\n\t"
        "lwc1 $f12, 0x0($t3)\n\t"
        "addiu $t1, $t1, 0x1\n\t"
        "swc1 $f12, 0x80($t2)\n\t"
        "slti $t2, $t1, 0xC\n\t"
        "bnez $t2, .Leboot_001B8B50\n\t"
        "addiu $a3, $a3, 0x4\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "slt $a3, $a1, $a2\n\t"
        "bnez $a3, .Leboot_001B8B48\n\t"
        "addiu $t0, $t0, 0xC0\n\t"
        "lw $a3, 0x40($a0)\n\t"
        ".Leboot_001B8B94:\n\t"
        "addiu $a1, $a3, 0x10\n\t"
        "lw $a2, 0x0($a1)\n\t"
        "ori $a1, $zero, 0x0\n\t"
        "slt $a3, $a1, $a2\n\t"
        "beqz $a3, .Leboot_001B8BF4\n\t"
        "ori $t0, $zero, 0x0\n\t"
        ".Leboot_001B8BAC:\n\t"
        "ori $t1, $zero, 0x0\n\t"
        "ori $a3, $zero, 0x0\n\t"
        ".Leboot_001B8BB4:\n\t"
        "lw $t2, 0x4C($a0)\n\t"
        "addu $t2, $t2, $t0\n\t"
        "lw $t3, 0x0($t2)\n\t"
        "addu $t2, $t2, $a3\n\t"
        "addiu $t3, $t3, 0x4\n\t"
        "addu $t3, $t3, $a3\n\t"
        "lwc1 $f12, 0x0($t3)\n\t"
        "addiu $t1, $t1, 0x1\n\t"
        "swc1 $f12, 0x4($t2)\n\t"
        "slti $t2, $t1, 0xC\n\t"
        "bnez $t2, .Leboot_001B8BB4\n\t"
        "addiu $a3, $a3, 0x4\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "slt $a3, $a1, $a2\n\t"
        "bnez $a3, .Leboot_001B8BAC\n\t"
        "addiu $t0, $t0, 0x34\n\t"
        ".Leboot_001B8BF4:\n\t"
        "jal syncSkeleton_01EC\n\t"
        "nop\n\t"
        "lw $ra, 0x10($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

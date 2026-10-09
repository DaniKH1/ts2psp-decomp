/**
 * The Sims 2 PSP - collision_0198 (0x1B09B8, 0x124 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s0, 0x20($sp)
 *     sw $s1, 0x24($sp)
 *     sw $s2, 0x28($sp)
 *     or $s1, $a0, $zero
 *     or $s2, $a1, $zero
 *     or $s0, $a2, $zero
 *     sw $ra, 0x2C($sp)
 *     lwc1 $f12, 0x0($s2)
 *     floor.w.s $f12, $f12
 *     mfc1 $a0, $f12
 *     sh $a0, 0x10($sp)
 *     lwc1 $f13, 0x4($s2)
 *     floor.w.s $f13, $f13
 *     mfc1 $a0, $f13
 *     sh $a0, 0x12($sp)
 *     lwc1 $f14, 0x8($s2)
 *     floor.w.s $f14, $f14
 *     mfc1 $a0, $f14
 *     sh $a0, 0x14($sp)
 *     sh $zero, 0x16($sp)
 *     addiu $a0, $s2, 0xC
 *     lwc1 $f12, 0x0($a0)
 *     floor.w.s $f12, $f12
 *     mfc1 $a1, $f12
 *     addiu $a1, $a1, 0x1
 *     sh $a1, 0x18($sp)
 *     lwc1 $f13, 0x4($a0)
 *     floor.w.s $f13, $f13
 *     mfc1 $a1, $f13
 *     addiu $a1, $a1, 0x1
 *     sh $a1, 0x1A($sp)
 *     lwc1 $f14, 0x8($a0)
 *     floor.w.s $f12, $f14
 *     mfc1 $a0, $f12
 *     or $s2, $s0, $zero
 *     addiu $a0, $a0, 0x1
 *     sh $a0, 0x1C($sp)
 *     sh $zero, 0x1E($sp)
 *     lw $a1, 0x8($s1)
 *     addiu $a0, $s1, 0x8
 *     addu $a0, $a0, $a1
 *     addiu $a1, $sp, 0x10
 *     jal collision_0F08
 *     or $a2, $s2, $zero
 *     lw $a1, 0xC($s2)
 *     ori $a0, $zero, 0x0
 *     slt $a2, $a0, $a1
 *     beqz $a2, .Leboot_001B0AC4
 *     addiu $a2, $s1, 0x4
 *     ori $a3, $zero, 0x0
 *   .Leboot_001B0A84
 *     lw $t0, 0x4($s2)
 *     lw $t1, 0x4($s1)
 *     addu $t0, $t0, $a3
 *     lw $t0, 0x0($t0)
 *     addu $t1, $a2, $t1
 *     sll $t2, $t0, 5
 *     sll $t0, $t0, 2
 *     addu $t0, $t2, $t0
 *     lw $t2, 0x4($s0)
 *     addu $t0, $t1, $t0
 *     addu $t1, $t2, $a3
 *     sw $t0, 0x0($t1)
 *     addiu $a0, $a0, 0x1
 *     slt $t0, $a0, $a1
 *     bnez $t0, .Leboot_001B0A84
 *     addiu $a3, $a3, 0x4
 *   .Leboot_001B0AC4
 *     lw $s0, 0x20($sp)
 *     lw $s1, 0x24($sp)
 *     lw $s2, 0x28($sp)
 *     lw $ra, 0x2C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_0198(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s0, 0x20($sp)\n\t"
        "sw $s1, 0x24($sp)\n\t"
        "sw $s2, 0x28($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "or $s2, $a1, $zero\n\t"
        "or $s0, $a2, $zero\n\t"
        "sw $ra, 0x2C($sp)\n\t"
        "lwc1 $f12, 0x0($s2)\n\t"
        "floor.w.s $f12, $f12\n\t"
        "mfc1 $a0, $f12\n\t"
        "sh $a0, 0x10($sp)\n\t"
        "lwc1 $f13, 0x4($s2)\n\t"
        "floor.w.s $f13, $f13\n\t"
        "mfc1 $a0, $f13\n\t"
        "sh $a0, 0x12($sp)\n\t"
        "lwc1 $f14, 0x8($s2)\n\t"
        "floor.w.s $f14, $f14\n\t"
        "mfc1 $a0, $f14\n\t"
        "sh $a0, 0x14($sp)\n\t"
        "sh $zero, 0x16($sp)\n\t"
        "addiu $a0, $s2, 0xC\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "floor.w.s $f12, $f12\n\t"
        "mfc1 $a1, $f12\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "sh $a1, 0x18($sp)\n\t"
        "lwc1 $f13, 0x4($a0)\n\t"
        "floor.w.s $f13, $f13\n\t"
        "mfc1 $a1, $f13\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "sh $a1, 0x1A($sp)\n\t"
        "lwc1 $f14, 0x8($a0)\n\t"
        "floor.w.s $f12, $f14\n\t"
        "mfc1 $a0, $f12\n\t"
        "or $s2, $s0, $zero\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "sh $a0, 0x1C($sp)\n\t"
        "sh $zero, 0x1E($sp)\n\t"
        "lw $a1, 0x8($s1)\n\t"
        "addiu $a0, $s1, 0x8\n\t"
        "addu $a0, $a0, $a1\n\t"
        "addiu $a1, $sp, 0x10\n\t"
        "jal collision_0F08\n\t"
        "or $a2, $s2, $zero\n\t"
        "lw $a1, 0xC($s2)\n\t"
        "ori $a0, $zero, 0x0\n\t"
        "slt $a2, $a0, $a1\n\t"
        "beqz $a2, .Leboot_001B0AC4\n\t"
        "addiu $a2, $s1, 0x4\n\t"
        "ori $a3, $zero, 0x0\n\t"
        ".Leboot_001B0A84:\n\t"
        "lw $t0, 0x4($s2)\n\t"
        "lw $t1, 0x4($s1)\n\t"
        "addu $t0, $t0, $a3\n\t"
        "lw $t0, 0x0($t0)\n\t"
        "addu $t1, $a2, $t1\n\t"
        "sll $t2, $t0, 5\n\t"
        "sll $t0, $t0, 2\n\t"
        "addu $t0, $t2, $t0\n\t"
        "lw $t2, 0x4($s0)\n\t"
        "addu $t0, $t1, $t0\n\t"
        "addu $t1, $t2, $a3\n\t"
        "sw $t0, 0x0($t1)\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "slt $t0, $a0, $a1\n\t"
        "bnez $t0, .Leboot_001B0A84\n\t"
        "addiu $a3, $a3, 0x4\n\t"
        ".Leboot_001B0AC4:\n\t"
        "lw $s0, 0x20($sp)\n\t"
        "lw $s1, 0x24($sp)\n\t"
        "lw $s2, 0x28($sp)\n\t"
        "lw $ra, 0x2C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

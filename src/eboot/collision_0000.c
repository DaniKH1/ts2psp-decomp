/**
 * The Sims 2 PSP - collision_0000 (0x1B0820, 0x11C bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s2, 0x18($sp)
 *     or $s2, $a0, $zero
 *     addiu $a0, $s2, 0xE
 *     lh $a0, 0x0($a0)
 *     mtc1 $a0, $f13
 *     cvt.s.w $f13, $f13
 *     lwc1 $f12, 0x0($a2)
 *     sub.s $f12, $f12, $f13
 *     lui $a0, 0x4380
 *     mtc1 $a0, $f14
 *     mul.s $f12, $f12, $f14
 *     sw $s0, 0x10($sp)
 *     sw $s1, 0x14($sp)
 *     sw $s3, 0x1C($sp)
 *     sw $s4, 0x20($sp)
 *     sw $ra, 0x24($sp)
 *     floor.w.s $f12, $f12
 *     lwc1 $f15, 0x0($a1)
 *     sub.s $f13, $f15, $f13
 *     mul.s $f13, $f13, $f14
 *     mfc1 $a0, $f12
 *     addiu $a0, $a0, 0x1
 *     floor.w.s $f13, $f13
 *     mfc1 $s4, $f13
 *     slti $a1, $a0, 0x7FFF
 *     lw $s3, 0x1C($s2)
 *     or $s1, $a3, $zero
 *     bnez $a1, .Leboot_001B08A0
 *     or $s0, $t0, $zero
 *     b .Leboot_001B08B8
 *     nop
 *   .Leboot_001B08A0
 *     or $a1, $a0, $zero
 *     or $a0, $s2, $zero
 *     or $a2, $zero, $zero
 *     jal collision_011C
 *     or $a3, $s3, $zero
 *     or $s3, $v0, $zero
 *   .Leboot_001B08B8
 *     slti $a0, $s4, -0x7FFF
 *     beqz $a0, .Leboot_001B08CC
 *     nop
 *     b .Leboot_001B08E4
 *     ori $s4, $zero, 0x0
 *   .Leboot_001B08CC
 *     or $a0, $s2, $zero
 *     or $a1, $s4, $zero
 *     or $a2, $zero, $zero
 *     jal collision_011C
 *     or $a3, $s3, $zero
 *     or $s4, $v0, $zero
 *   .Leboot_001B08E4
 *     blez $s4, .Leboot_001B0904
 *     nop
 *     lw $a0, 0x20($s2)
 *     addiu $a1, $s2, 0x20
 *     addu $a0, $a1, $a0
 *     sll $a1, $s4, 2
 *     addu $a0, $a0, $a1
 *     lbu $s4, -0x1($a0)
 *   .Leboot_001B0904
 *     beqz $s1, .Leboot_001B0910
 *     nop
 *     sw $s4, 0x0($s1)
 *   .Leboot_001B0910
 *     beqz $s0, .Leboot_001B091C
 *     nop
 *     sw $s3, 0x0($s0)
 *   .Leboot_001B091C
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $s4, 0x20($sp)
 *     lw $ra, 0x24($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * collision: the entry point of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_0000(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "or $s2, $a0, $zero\n\t"
        "addiu $a0, $s2, 0xE\n\t"
        "lh $a0, 0x0($a0)\n\t"
        "mtc1 $a0, $f13\n\t"
        "cvt.s.w $f13, $f13\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "sub.s $f12, $f12, $f13\n\t"
        "lui $a0, 0x4380\n\t"
        "mtc1 $a0, $f14\n\t"
        "mul.s $f12, $f12, $f14\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "sw $s4, 0x20($sp)\n\t"
        "sw $ra, 0x24($sp)\n\t"
        "floor.w.s $f12, $f12\n\t"
        "lwc1 $f15, 0x0($a1)\n\t"
        "sub.s $f13, $f15, $f13\n\t"
        "mul.s $f13, $f13, $f14\n\t"
        "mfc1 $a0, $f12\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "floor.w.s $f13, $f13\n\t"
        "mfc1 $s4, $f13\n\t"
        "slti $a1, $a0, 0x7FFF\n\t"
        "lw $s3, 0x1C($s2)\n\t"
        "or $s1, $a3, $zero\n\t"
        "bnez $a1, .Leboot_001B08A0\n\t"
        "or $s0, $t0, $zero\n\t"
        "b .Leboot_001B08B8\n\t"
        "nop\n\t"
        ".Leboot_001B08A0:\n\t"
        "or $a1, $a0, $zero\n\t"
        "or $a0, $s2, $zero\n\t"
        "or $a2, $zero, $zero\n\t"
        "jal collision_011C\n\t"
        "or $a3, $s3, $zero\n\t"
        "or $s3, $v0, $zero\n\t"
        ".Leboot_001B08B8:\n\t"
        "slti $a0, $s4, -0x7FFF\n\t"
        "beqz $a0, .Leboot_001B08CC\n\t"
        "nop\n\t"
        "b .Leboot_001B08E4\n\t"
        "ori $s4, $zero, 0x0\n\t"
        ".Leboot_001B08CC:\n\t"
        "or $a0, $s2, $zero\n\t"
        "or $a1, $s4, $zero\n\t"
        "or $a2, $zero, $zero\n\t"
        "jal collision_011C\n\t"
        "or $a3, $s3, $zero\n\t"
        "or $s4, $v0, $zero\n\t"
        ".Leboot_001B08E4:\n\t"
        "blez $s4, .Leboot_001B0904\n\t"
        "nop\n\t"
        "lw $a0, 0x20($s2)\n\t"
        "addiu $a1, $s2, 0x20\n\t"
        "addu $a0, $a1, $a0\n\t"
        "sll $a1, $s4, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        "lbu $s4, -0x1($a0)\n\t"
        ".Leboot_001B0904:\n\t"
        "beqz $s1, .Leboot_001B0910\n\t"
        "nop\n\t"
        "sw $s4, 0x0($s1)\n\t"
        ".Leboot_001B0910:\n\t"
        "beqz $s0, .Leboot_001B091C\n\t"
        "nop\n\t"
        "sw $s3, 0x0($s0)\n\t"
        ".Leboot_001B091C:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $s4, 0x20($sp)\n\t"
        "lw $ra, 0x24($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

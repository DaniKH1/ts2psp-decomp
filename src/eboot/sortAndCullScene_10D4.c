/**
 * The Sims 2 PSP - sortAndCullScene_10D4 (0x1B4CF0, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lbu $a1, 0xE8($a0)
 *     sw $s1, 0x14($sp)
 *     addiu $s1, $a0, 0x60
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x18($sp)
 *     bnez $a1, .Leboot_001B4D28
 *     or $s0, $a0, $zero
 *     addiu $a0, $s0, 0xC0
 *     addiu $a1, $s0, 0x80
 *     jal collision_14D4
 *     or $a2, $s1, $zero
 *     ori $a0, $zero, 0x1
 *     sb $a0, 0xE8($s0)
 *   .Leboot_001B4D28
 *     or $v0, $s1, $zero
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * sortAndCullScene: gate on the byte at 0xE8, then run collision_14D4 over 0x60/0x80/0xC0 of the object.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_10D4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lbu $a1, 0xE8($a0)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "addiu $s1, $a0, 0x60\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "bnez $a1, .Leboot_001B4D28\n\t"
        "or $s0, $a0, $zero\n\t"
        "addiu $a0, $s0, 0xC0\n\t"
        "addiu $a1, $s0, 0x80\n\t"
        "jal collision_14D4\n\t"
        "or $a2, $s1, $zero\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "sb $a0, 0xE8($s0)\n\t"
        ".Leboot_001B4D28:\n\t"
        "or $v0, $s1, $zero\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $ra, 0x18($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

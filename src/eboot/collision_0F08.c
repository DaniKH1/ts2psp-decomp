/**
 * The Sims 2 PSP - collision_0F08 (0x1B1728, 0xF0 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x10($sp)
 *     or $s0, $a0, $zero
 *     lw $a0, 0xC($a2)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a1, $zero
 *     sw $s2, 0x18($sp)
 *     sw $ra, 0x1C($sp)
 *     beqz $a0, .Leboot_001B178C
 *     or $s2, $a2, $zero
 *     or $a1, $a0, $zero
 *     lw $a0, 0x4($s2)
 *     sll $a1, $a1, 2
 *     addu $a1, $a0, $a1
 *     beq $a1, $a0, .Leboot_001B1774
 *     nop
 *     addiu $a1, $a1, -0x4
 *   .Leboot_001B176C
 *     bne $a1, $a0, .Leboot_001B176C
 *     addiu $a1, $a1, -0x4
 *   .Leboot_001B1774
 *     or $a0, $s2, $zero
 *     jal func_0012C914
 *     or $a1, $zero, $zero
 *     sw $zero, 0xC($s2)
 *     b .Leboot_001B17D8
 *     lw $a1, 0x0($s0)
 *   .Leboot_001B178C
 *     or $a0, $s2, $zero
 *     jal func_0012C914
 *     or $a1, $zero, $zero
 *     srl $a1, $v0, 2
 *     lw $a0, 0xC($s2)
 *     bnel $a1, $zero, .Leboot_001B17A8
 *     ori $a1, $zero, 0x0
 *   .Leboot_001B17A8
 *     lw $a3, 0x4($s2)
 *     sll $a2, $a0, 2
 *     sll $a0, $a1, 2
 *     addu $a2, $a3, $a2
 *     addu $a0, $a3, $a0
 *     beq $a2, $a0, .Leboot_001B17D0
 *     nop
 *     addiu $a2, $a2, 0x4
 *   .Leboot_001B17C8
 *     bne $a2, $a0, .Leboot_001B17C8
 *     addiu $a2, $a2, 0x4
 *   .Leboot_001B17D0
 *     sw $a1, 0xC($s2)
 *     lw $a1, 0x0($s0)
 *   .Leboot_001B17D8
 *     bnez $a1, .Leboot_001B17E8
 *     nop
 *     b .Leboot_001B1800
 *     or $v0, $zero, $zero
 *   .Leboot_001B17E8
 *     or $a0, $s0, $zero
 *     or $a1, $s1, $zero
 *     or $a2, $zero, $zero
 *     jal collision_0C18
 *     or $a3, $s2, $zero
 *     lw $v0, 0xC($s2)
 *   .Leboot_001B1800
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $ra, 0x1C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_0F08(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "lw $a0, 0xC($a2)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a1, $zero\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $ra, 0x1C($sp)\n\t"
        "beqz $a0, .Leboot_001B178C\n\t"
        "or $s2, $a2, $zero\n\t"
        "or $a1, $a0, $zero\n\t"
        "lw $a0, 0x4($s2)\n\t"
        "sll $a1, $a1, 2\n\t"
        "addu $a1, $a0, $a1\n\t"
        "beq $a1, $a0, .Leboot_001B1774\n\t"
        "nop\n\t"
        "addiu $a1, $a1, -0x4\n\t"
        ".Leboot_001B176C:\n\t"
        "bne $a1, $a0, .Leboot_001B176C\n\t"
        "addiu $a1, $a1, -0x4\n\t"
        ".Leboot_001B1774:\n\t"
        "or $a0, $s2, $zero\n\t"
        "jal func_0012C914\n\t"
        "or $a1, $zero, $zero\n\t"
        "sw $zero, 0xC($s2)\n\t"
        "b .Leboot_001B17D8\n\t"
        "lw $a1, 0x0($s0)\n\t"
        ".Leboot_001B178C:\n\t"
        "or $a0, $s2, $zero\n\t"
        "jal func_0012C914\n\t"
        "or $a1, $zero, $zero\n\t"
        "srl $a1, $v0, 2\n\t"
        "lw $a0, 0xC($s2)\n\t"
        "bnel $a1, $zero, .Leboot_001B17A8\n\t"
        "ori $a1, $zero, 0x0\n\t"
        ".Leboot_001B17A8:\n\t"
        "lw $a3, 0x4($s2)\n\t"
        "sll $a2, $a0, 2\n\t"
        "sll $a0, $a1, 2\n\t"
        "addu $a2, $a3, $a2\n\t"
        "addu $a0, $a3, $a0\n\t"
        "beq $a2, $a0, .Leboot_001B17D0\n\t"
        "nop\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        ".Leboot_001B17C8:\n\t"
        "bne $a2, $a0, .Leboot_001B17C8\n\t"
        "addiu $a2, $a2, 0x4\n\t"
        ".Leboot_001B17D0:\n\t"
        "sw $a1, 0xC($s2)\n\t"
        "lw $a1, 0x0($s0)\n\t"
        ".Leboot_001B17D8:\n\t"
        "bnez $a1, .Leboot_001B17E8\n\t"
        "nop\n\t"
        "b .Leboot_001B1800\n\t"
        "or $v0, $zero, $zero\n\t"
        ".Leboot_001B17E8:\n\t"
        "or $a0, $s0, $zero\n\t"
        "or $a1, $s1, $zero\n\t"
        "or $a2, $zero, $zero\n\t"
        "jal collision_0C18\n\t"
        "or $a3, $s2, $zero\n\t"
        "lw $v0, 0xC($s2)\n\t"
        ".Leboot_001B1800:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $ra, 0x1C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

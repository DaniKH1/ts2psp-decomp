/**
 * The Sims 2 PSP - renderMeshInstances_0F64 (0x1BE964, 0xA0 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s1, 0x14($sp)
 *     or $s1, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     addiu $s3, $s1, 0x80
 *     or $s0, $a2, $zero
 *     or $s2, $a1, $zero
 *     sw $ra, 0x20($sp)
 *     jal func_00102BF8
 *     or $a0, $s3, $zero
 *     ori $a0, $zero, 0x40
 *     bne $s2, $a0, .Leboot_001BE9CC
 *     sw $s3, 0x158($s1)
 *     lw $a0, 0xE4($s1)
 *     andi $a0, $a0, 0x2
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BE9E8
 *     nop
 *     addiu $a0, $s1, 0x150
 *     jal renderMeshInstances_0000
 *     or $a1, $zero, $zero
 *     b .Leboot_001BE9E8
 *     nop
 *   .Leboot_001BE9CC
 *     lw $a0, 0x5C($s1)
 *     addiu $a1, $s1, 0x150
 *     andi $a0, $a0, 0x1000
 *     sltu $a2, $zero, $a0
 *     andi $a2, $a2, 0xFF
 *     jal renderMeshInstances_0B1C
 *     or $a0, $s0, $zero
 *   .Leboot_001BE9E8
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0F64(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "addiu $s3, $s1, 0x80\n\t"
        "or $s0, $a2, $zero\n\t"
        "or $s2, $a1, $zero\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "jal func_00102BF8\n\t"
        "or $a0, $s3, $zero\n\t"
        "ori $a0, $zero, 0x40\n\t"
        "bne $s2, $a0, .Leboot_001BE9CC\n\t"
        "sw $s3, 0x158($s1)\n\t"
        "lw $a0, 0xE4($s1)\n\t"
        "andi $a0, $a0, 0x2\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE9E8\n\t"
        "nop\n\t"
        "addiu $a0, $s1, 0x150\n\t"
        "jal renderMeshInstances_0000\n\t"
        "or $a1, $zero, $zero\n\t"
        "b .Leboot_001BE9E8\n\t"
        "nop\n\t"
        ".Leboot_001BE9CC:\n\t"
        "lw $a0, 0x5C($s1)\n\t"
        "addiu $a1, $s1, 0x150\n\t"
        "andi $a0, $a0, 0x1000\n\t"
        "sltu $a2, $zero, $a0\n\t"
        "andi $a2, $a2, 0xFF\n\t"
        "jal renderMeshInstances_0B1C\n\t"
        "or $a0, $s0, $zero\n\t"
        ".Leboot_001BE9E8:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

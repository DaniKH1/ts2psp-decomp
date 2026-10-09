/**
 * The Sims 2 PSP - renderMeshInstances_0C1C (0x1BE61C, 0x64 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lbu $a2, 0x11C($a0)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x18($sp)
 *     bnez $a2, .Leboot_001BE644
 *     or $s0, $a1, $zero
 *     b .Leboot_001BE66C
 *     nop
 *   .Leboot_001BE644
 *     lw $a0, 0x4($s0)
 *     jal func_00102C84
 *     addiu $a0, $a0, 0x28
 *     jal renderCommon_17CC
 *     addiu $a0, $s1, 0x180
 *     jal renderCommon_0F7C
 *     or $a0, $s0, $zero
 *     or $a0, $s1, $zero
 *     jal renderMeshInstances_0C80
 *     or $a1, $s0, $zero
 *   .Leboot_001BE66C
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0C1C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lbu $a2, 0x11C($a0)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "bnez $a2, .Leboot_001BE644\n\t"
        "or $s0, $a1, $zero\n\t"
        "b .Leboot_001BE66C\n\t"
        "nop\n\t"
        ".Leboot_001BE644:\n\t"
        "lw $a0, 0x4($s0)\n\t"
        "jal func_00102C84\n\t"
        "addiu $a0, $a0, 0x28\n\t"
        "jal renderCommon_17CC\n\t"
        "addiu $a0, $s1, 0x180\n\t"
        "jal renderCommon_0F7C\n\t"
        "or $a0, $s0, $zero\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal renderMeshInstances_0C80\n\t"
        "or $a1, $s0, $zero\n\t"
        ".Leboot_001BE66C:\n\t"
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

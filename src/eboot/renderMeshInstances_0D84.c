/**
 * The Sims 2 PSP - renderMeshInstances_0D84 (0x1BE784, 0xEC bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s0, 0x10($sp)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a1, $zero
 *     or $s0, $a2, $zero
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     sw $s4, 0x20($sp)
 *     sw $s5, 0x24($sp)
 *     sw $s6, 0x28($sp)
 *     sw $ra, 0x2C($sp)
 *     lw $a1, 0x4($s0)
 *     lw $a2, 0x0($s0)
 *     ori $s5, $zero, 0x0
 *     subu $a1, $a1, $a2
 *     sra $a2, $a1, 4
 *     srl $a2, $a2, 28
 *     addu $s6, $a1, $a2
 *     sra $s6, $s6, 4
 *     slt $a1, $s5, $s6
 *     beqz $a1, .Leboot_001BE848
 *     ori $a0, $zero, 0x0
 *     ori $s4, $zero, 0x0
 *     lui $s2, %%hi(sym_000E69EC)
 *   .Leboot_001BE7E4
 *     lw $s3, 0x0($s0)
 *     addu $s3, $s3, $s4
 *     lw $a1, 0x0($s3)
 *     lw $a1, 0x5C($a1)
 *     and $a1, $a1, $s1
 *     beqz $a1, .Leboot_001BE838
 *     nop
 *     lw $a1, 0x4($s3)
 *     xor $a0, $a1, $a0
 *     sltiu $a0, $a0, 0x1
 *     sb $a0, %%lo(sym_000E69EC)($s2)
 *     lw $a0, 0x0($s3)
 *     or $a1, $s1, $zero
 *     lw $a2, 0x78($a0)
 *     addiu $a2, $a2, 0x70
 *     lh $a3, 0x0($a2)
 *     lw $t0, 0x4($a2)
 *     addu $a0, $a0, $a3
 *     jalr $t0
 *     or $a2, $s3, $zero
 *     lw $a0, 0x4($s3)
 *   .Leboot_001BE838
 *     addiu $s5, $s5, 0x1
 *     slt $a1, $s5, $s6
 *     bnez $a1, .Leboot_001BE7E4
 *     addiu $s4, $s4, 0x10
 *   .Leboot_001BE848
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $s4, 0x20($sp)
 *     lw $s5, 0x24($sp)
 *     lw $s6, 0x28($sp)
 *     lw $ra, 0x2C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0D84(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a1, $zero\n\t"
        "or $s0, $a2, $zero\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "sw $s4, 0x20($sp)\n\t"
        "sw $s5, 0x24($sp)\n\t"
        "sw $s6, 0x28($sp)\n\t"
        "sw $ra, 0x2C($sp)\n\t"
        "lw $a1, 0x4($s0)\n\t"
        "lw $a2, 0x0($s0)\n\t"
        "ori $s5, $zero, 0x0\n\t"
        "subu $a1, $a1, $a2\n\t"
        "sra $a2, $a1, 4\n\t"
        "srl $a2, $a2, 28\n\t"
        "addu $s6, $a1, $a2\n\t"
        "sra $s6, $s6, 4\n\t"
        "slt $a1, $s5, $s6\n\t"
        "beqz $a1, .Leboot_001BE848\n\t"
        "ori $a0, $zero, 0x0\n\t"
        "ori $s4, $zero, 0x0\n\t"
        "lui $s2, %%hi(sym_000E69EC)\n\t"
        ".Leboot_001BE7E4:\n\t"
        "lw $s3, 0x0($s0)\n\t"
        "addu $s3, $s3, $s4\n\t"
        "lw $a1, 0x0($s3)\n\t"
        "lw $a1, 0x5C($a1)\n\t"
        "and $a1, $a1, $s1\n\t"
        "beqz $a1, .Leboot_001BE838\n\t"
        "nop\n\t"
        "lw $a1, 0x4($s3)\n\t"
        "xor $a0, $a1, $a0\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "sb $a0, %%lo(sym_000E69EC)($s2)\n\t"
        "lw $a0, 0x0($s3)\n\t"
        "or $a1, $s1, $zero\n\t"
        "lw $a2, 0x78($a0)\n\t"
        "addiu $a2, $a2, 0x70\n\t"
        "lh $a3, 0x0($a2)\n\t"
        "lw $t0, 0x4($a2)\n\t"
        "addu $a0, $a0, $a3\n\t"
        "jalr $t0\n\t"
        "or $a2, $s3, $zero\n\t"
        "lw $a0, 0x4($s3)\n\t"
        ".Leboot_001BE838:\n\t"
        "addiu $s5, $s5, 0x1\n\t"
        "slt $a1, $s5, $s6\n\t"
        "bnez $a1, .Leboot_001BE7E4\n\t"
        "addiu $s4, $s4, 0x10\n\t"
        ".Leboot_001BE848:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $s4, 0x20($sp)\n\t"
        "lw $s5, 0x24($sp)\n\t"
        "lw $s6, 0x28($sp)\n\t"
        "lw $ra, 0x2C($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

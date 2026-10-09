/**
 * The Sims 2 PSP - renderMeshInstances_0C80 (0x1BE680, 0x104 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lui $a2, %%hi(sym_000E2908)
 *     lw $a2, %%lo(sym_000E2908)($a2)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a0, $zero
 *     lw $a0, 0x4($a2)
 *     sw $s0, 0x10($sp)
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     sw $s4, 0x20($sp)
 *     sw $ra, 0x24($sp)
 *     beqz $a0, .Leboot_001BE6FC
 *     or $s0, $a1, $zero
 *     lui $a0, %%hi(sym_000E2968)
 *     lw $a0, %%lo(sym_000E2968)($a0)
 *     lw $a0, 0x4($a0)
 *     beqz $a0, .Leboot_001BE6FC
 *     nop
 *     lbu $a0, 0x400($s1)
 *     bnez $a0, .Leboot_001BE6F4
 *     nop
 *     lw $a0, 0x3F0($s1)
 *     andi $a0, $a0, 0x1
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     bnez $a0, .Leboot_001BE704
 *     nop
 *     b .Leboot_001BE764
 *     nop
 *   .Leboot_001BE6F4
 *     b .Leboot_001BE764
 *     nop
 *   .Leboot_001BE6FC
 *     b .Leboot_001BE764
 *     nop
 *   .Leboot_001BE704
 *     lw $a0, 0x150($s1)
 *     beqz $a0, .Leboot_001BE764
 *     nop
 *     lw $a0, 0x148($s1)
 *     addiu $s4, $s1, 0x120
 *     beq $s4, $a0, .Leboot_001BE764
 *     ori $s2, $zero, 0x1
 *     subu $s3, $s4, $s4
 *   .Leboot_001BE724
 *     sra $a0, $s3, 2
 *     srl $a0, $a0, 30
 *     addu $a0, $s3, $a0
 *     lw $a1, 0x150($s1)
 *     sra $a0, $a0, 2
 *     sllv $a0, $s2, $a0
 *     and $a0, $a1, $a0
 *     beqz $a0, .Leboot_001BE754
 *     nop
 *     lw $a0, 0x0($s4)
 *     jal func_000AC80C
 *     or $a1, $s0, $zero
 *   .Leboot_001BE754
 *     lw $a0, 0x148($s1)
 *     addiu $s4, $s4, 0x4
 *     bne $s4, $a0, .Leboot_001BE724
 *     addiu $s3, $s3, 0x4
 *   .Leboot_001BE764
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $s4, 0x20($sp)
 *     lw $ra, 0x24($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0C80(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lui $a2, %%hi(sym_000E2908)\n\t"
        "lw $a2, %%lo(sym_000E2908)($a2)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "lw $a0, 0x4($a2)\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "sw $s4, 0x20($sp)\n\t"
        "sw $ra, 0x24($sp)\n\t"
        "beqz $a0, .Leboot_001BE6FC\n\t"
        "or $s0, $a1, $zero\n\t"
        "lui $a0, %%hi(sym_000E2968)\n\t"
        "lw $a0, %%lo(sym_000E2968)($a0)\n\t"
        "lw $a0, 0x4($a0)\n\t"
        "beqz $a0, .Leboot_001BE6FC\n\t"
        "nop\n\t"
        "lbu $a0, 0x400($s1)\n\t"
        "bnez $a0, .Leboot_001BE6F4\n\t"
        "nop\n\t"
        "lw $a0, 0x3F0($s1)\n\t"
        "andi $a0, $a0, 0x1\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "bnez $a0, .Leboot_001BE704\n\t"
        "nop\n\t"
        "b .Leboot_001BE764\n\t"
        "nop\n\t"
        ".Leboot_001BE6F4:\n\t"
        "b .Leboot_001BE764\n\t"
        "nop\n\t"
        ".Leboot_001BE6FC:\n\t"
        "b .Leboot_001BE764\n\t"
        "nop\n\t"
        ".Leboot_001BE704:\n\t"
        "lw $a0, 0x150($s1)\n\t"
        "beqz $a0, .Leboot_001BE764\n\t"
        "nop\n\t"
        "lw $a0, 0x148($s1)\n\t"
        "addiu $s4, $s1, 0x120\n\t"
        "beq $s4, $a0, .Leboot_001BE764\n\t"
        "ori $s2, $zero, 0x1\n\t"
        "subu $s3, $s4, $s4\n\t"
        ".Leboot_001BE724:\n\t"
        "sra $a0, $s3, 2\n\t"
        "srl $a0, $a0, 30\n\t"
        "addu $a0, $s3, $a0\n\t"
        "lw $a1, 0x150($s1)\n\t"
        "sra $a0, $a0, 2\n\t"
        "sllv $a0, $s2, $a0\n\t"
        "and $a0, $a1, $a0\n\t"
        "beqz $a0, .Leboot_001BE754\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s4)\n\t"
        "jal func_000AC80C\n\t"
        "or $a1, $s0, $zero\n\t"
        ".Leboot_001BE754:\n\t"
        "lw $a0, 0x148($s1)\n\t"
        "addiu $s4, $s4, 0x4\n\t"
        "bne $s4, $a0, .Leboot_001BE724\n\t"
        "addiu $s3, $s3, 0x4\n\t"
        ".Leboot_001BE764:\n\t"
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

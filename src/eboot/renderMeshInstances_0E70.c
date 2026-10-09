/**
 * The Sims 2 PSP - renderMeshInstances_0E70 (0x1BE870, 0xF4 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a2, 0x14C($a0)
 *     sw $s0, 0x10($sp)
 *     or $s0, $a0, $zero
 *     sw $s1, 0x14($sp)
 *     sw $s2, 0x18($sp)
 *     sw $ra, 0x1C($sp)
 *     beqz $a2, .Leboot_001BE8B4
 *     or $s1, $a1, $zero
 *     addiu $s2, $s0, 0x80
 *     jal func_00102BF8
 *     or $a0, $s2, $zero
 *     ori $a0, $zero, 0x40
 *     beq $s1, $a0, .Leboot_001BE8BC
 *     sw $s2, 0x158($s0)
 *     b .Leboot_001BE8E8
 *     nop
 *   .Leboot_001BE8B4
 *     b .Leboot_001BE94C
 *     nop
 *   .Leboot_001BE8BC
 *     lw $a0, 0xE4($s0)
 *     andi $a0, $a0, 0x2
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BE8E0
 *     nop
 *     addiu $a0, $s0, 0x150
 *     jal renderMeshInstances_0000
 *     or $a1, $zero, $zero
 *   .Leboot_001BE8E0
 *     b .Leboot_001BE94C
 *     nop
 *   .Leboot_001BE8E8
 *     lui $a0, %%hi(sym_001D99E0)
 *     lw $a0, %%lo(sym_001D99E0)($a0)
 *     lw $a1, 0xEC($s0)
 *     lw $a3, 0x4($a0)
 *     addiu $a2, $s0, 0x150
 *     lw $a0, 0x68($a1)
 *     beqz $a3, .Leboot_001BE924
 *     lui $s1, %%hi(sym_000E28EC)
 *     lw $a1, 0x60($a1)
 *     lw $a3, %%lo(sym_000E28EC)($s1)
 *     bne $a1, $a3, .Leboot_001BE924
 *     lui $a1, %%hi(sym_000E69EC)
 *     lbu $a1, %%lo(sym_000E69EC)($a1)
 *     bnez $a1, .Leboot_001BE940
 *     nop
 *   .Leboot_001BE924
 *     or $a1, $a2, $zero
 *     jal renderMeshInstances_0B1C
 *     ori $a2, $zero, 0x1
 *     lw $a0, %%lo(sym_000E28EC)($s1)
 *     lw $a1, 0xEC($s0)
 *     b .Leboot_001BE94C
 *     sw $a0, 0x60($a1)
 *   .Leboot_001BE940
 *     or $a1, $a2, $zero
 *     jal renderMeshInstances_0C1C
 *     ori $a2, $zero, 0x1
 *   .Leboot_001BE94C
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $ra, 0x1C($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0E70(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a2, 0x14C($a0)\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $ra, 0x1C($sp)\n\t"
        "beqz $a2, .Leboot_001BE8B4\n\t"
        "or $s1, $a1, $zero\n\t"
        "addiu $s2, $s0, 0x80\n\t"
        "jal func_00102BF8\n\t"
        "or $a0, $s2, $zero\n\t"
        "ori $a0, $zero, 0x40\n\t"
        "beq $s1, $a0, .Leboot_001BE8BC\n\t"
        "sw $s2, 0x158($s0)\n\t"
        "b .Leboot_001BE8E8\n\t"
        "nop\n\t"
        ".Leboot_001BE8B4:\n\t"
        "b .Leboot_001BE94C\n\t"
        "nop\n\t"
        ".Leboot_001BE8BC:\n\t"
        "lw $a0, 0xE4($s0)\n\t"
        "andi $a0, $a0, 0x2\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE8E0\n\t"
        "nop\n\t"
        "addiu $a0, $s0, 0x150\n\t"
        "jal renderMeshInstances_0000\n\t"
        "or $a1, $zero, $zero\n\t"
        ".Leboot_001BE8E0:\n\t"
        "b .Leboot_001BE94C\n\t"
        "nop\n\t"
        ".Leboot_001BE8E8:\n\t"
        "lui $a0, %%hi(sym_001D99E0)\n\t"
        "lw $a0, %%lo(sym_001D99E0)($a0)\n\t"
        "lw $a1, 0xEC($s0)\n\t"
        "lw $a3, 0x4($a0)\n\t"
        "addiu $a2, $s0, 0x150\n\t"
        "lw $a0, 0x68($a1)\n\t"
        "beqz $a3, .Leboot_001BE924\n\t"
        "lui $s1, %%hi(sym_000E28EC)\n\t"
        "lw $a1, 0x60($a1)\n\t"
        "lw $a3, %%lo(sym_000E28EC)($s1)\n\t"
        "bne $a1, $a3, .Leboot_001BE924\n\t"
        "lui $a1, %%hi(sym_000E69EC)\n\t"
        "lbu $a1, %%lo(sym_000E69EC)($a1)\n\t"
        "bnez $a1, .Leboot_001BE940\n\t"
        "nop\n\t"
        ".Leboot_001BE924:\n\t"
        "or $a1, $a2, $zero\n\t"
        "jal renderMeshInstances_0B1C\n\t"
        "ori $a2, $zero, 0x1\n\t"
        "lw $a0, %%lo(sym_000E28EC)($s1)\n\t"
        "lw $a1, 0xEC($s0)\n\t"
        "b .Leboot_001BE94C\n\t"
        "sw $a0, 0x60($a1)\n\t"
        ".Leboot_001BE940:\n\t"
        "or $a1, $a2, $zero\n\t"
        "jal renderMeshInstances_0C1C\n\t"
        "ori $a2, $zero, 0x1\n\t"
        ".Leboot_001BE94C:\n\t"
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

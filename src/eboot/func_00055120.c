/**
 * The Sims 2 PSP - func_00055120 (0x055120, 0xC0 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s1, 0x14($sp)
 *     or $s1, $a0, $zero
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x18($sp)
 *     beqz $a0, .Leboot_000551CC
 *     or $s0, $a1, $zero
 *     lui $a0, %%hi(sym_001E56A8)
 *     addiu $a0, $a0, %%lo(sym_001E56A8)
 *     sw $a0, 0x38($s1)
 *     lui $a0, %%hi(sym_001E5718)
 *     addiu $a0, $a0, %%lo(sym_001E5718)
 *     beqz $s1, .Leboot_000551B8
 *     sw $a0, 0xC($s1)
 *     lui $a0, %%hi(sym_001E82E0)
 *     addiu $a0, $a0, %%lo(sym_001E82E0)
 *     sw $a0, 0x38($s1)
 *     lui $a0, %%hi(sym_001E8350)
 *     addiu $a0, $a0, %%lo(sym_001E8350)
 *     addiu $a3, $s1, 0x3C
 *     beqz $a3, .Leboot_000551AC
 *     sw $a0, 0xC($s1)
 *     beqz $a3, .Leboot_000551B0
 *     or $a0, $s1, $zero
 *     lw $a2, 0x3C($s1)
 *     beqz $a2, .Leboot_000551B0
 *     or $a0, $s1, $zero
 *     lw $a1, 0x0($a2)
 *     beq $a1, $a3, .Leboot_000551A8
 *     lw $a0, 0x40($s1)
 *     addiu $a2, $a1, 0x4
 *   .Leboot_0005519C
 *     lw $a1, 0x0($a2)
 *     bnel $a1, $a3, .Leboot_0005519C
 *     addiu $a2, $a1, 0x4
 *   .Leboot_000551A8
 *     sw $a0, 0x0($a2)
 *   .Leboot_000551AC
 *     or $a0, $s1, $zero
 *   .Leboot_000551B0
 *     jal func_0008598C
 *     or $a1, $zero, $zero
 *   .Leboot_000551B8
 *     andi $a0, $s0, 0x1
 *     beqz $a0, .Leboot_000551CC
 *     nop
 *     jal func_0012771C
 *     or $a0, $s1, $zero
 *   .Leboot_000551CC
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_00055120(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a0, $zero\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "beqz $a0, .Leboot_000551CC\n\t"
        "or $s0, $a1, $zero\n\t"
        "lui $a0, %%hi(sym_001E56A8)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E56A8)\n\t"
        "sw $a0, 0x38($s1)\n\t"
        "lui $a0, %%hi(sym_001E5718)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E5718)\n\t"
        "beqz $s1, .Leboot_000551B8\n\t"
        "sw $a0, 0xC($s1)\n\t"
        "lui $a0, %%hi(sym_001E82E0)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E82E0)\n\t"
        "sw $a0, 0x38($s1)\n\t"
        "lui $a0, %%hi(sym_001E8350)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E8350)\n\t"
        "addiu $a3, $s1, 0x3C\n\t"
        "beqz $a3, .Leboot_000551AC\n\t"
        "sw $a0, 0xC($s1)\n\t"
        "beqz $a3, .Leboot_000551B0\n\t"
        "or $a0, $s1, $zero\n\t"
        "lw $a2, 0x3C($s1)\n\t"
        "beqz $a2, .Leboot_000551B0\n\t"
        "or $a0, $s1, $zero\n\t"
        "lw $a1, 0x0($a2)\n\t"
        "beq $a1, $a3, .Leboot_000551A8\n\t"
        "lw $a0, 0x40($s1)\n\t"
        "addiu $a2, $a1, 0x4\n\t"
        ".Leboot_0005519C:\n\t"
        "lw $a1, 0x0($a2)\n\t"
        "bnel $a1, $a3, .Leboot_0005519C\n\t"
        "addiu $a2, $a1, 0x4\n\t"
        ".Leboot_000551A8:\n\t"
        "sw $a0, 0x0($a2)\n\t"
        ".Leboot_000551AC:\n\t"
        "or $a0, $s1, $zero\n\t"
        ".Leboot_000551B0:\n\t"
        "jal func_0008598C\n\t"
        "or $a1, $zero, $zero\n\t"
        ".Leboot_000551B8:\n\t"
        "andi $a0, $s0, 0x1\n\t"
        "beqz $a0, .Leboot_000551CC\n\t"
        "nop\n\t"
        "jal func_0012771C\n\t"
        "or $a0, $s1, $zero\n\t"
        ".Leboot_000551CC:\n\t"
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

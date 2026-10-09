/**
 * The Sims 2 PSP - renderCommon_1500 (0x1BD700, 0x118 bytes)
 *
 *     lui $a3, %%hi(D_0001039C)
 *     lui $a2, %%hi(sym_001DB710)
 *     lw $a0, 0x270($a0)
 *     lui $t1, 0xE700
 *     andi $t0, $a0, 0x1
 *     or $t0, $t0, $t1
 *     lw $t1, %%lo(D_0001039C)($a3)
 *     beq $t1, $t0, .Leboot_001BD734
 *     lw $a1, %%lo(sym_001DB710)($a2)
 *     lui $t1, %%hi(D_0001039C)
 *     sw $t0, %%lo(D_0001039C)($t1)
 *     sw $t0, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD734
 *     andi $t1, $a0, 0x10
 *     bnez $t1, .Leboot_001BD798
 *     lui $t0, 0x2300
 *     andi $t0, $a0, 0x20
 *     beqz $t0, .Leboot_001BD758
 *     nop
 *     lui $t0, %%hi(D_DE000007)
 *     b .Leboot_001BD778
 *     addiu $t0, $t0, %%lo(D_DE000007)
 *   .Leboot_001BD758
 *     andi $t0, $a0, 0x40
 *     beqz $t0, .Leboot_001BD770
 *     nop
 *     lui $t0, %%hi(D_DE000005)
 *     b .Leboot_001BD778
 *     addiu $t0, $t0, %%lo(D_DE000005)
 *   .Leboot_001BD770
 *     lui $t0, %%hi(D_DE000002)
 *     addiu $t0, $t0, %%lo(D_DE000002)
 *   .Leboot_001BD778
 *     lw $t1, %%lo(D_00010378)($a3)
 *     beq $t1, $t0, .Leboot_001BD790
 *     lui $t1, %%hi(D_00010378)
 *     sw $t0, %%lo(D_00010378)($t1)
 *     sw $t0, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD790
 *     lui $t0, %%hi(D_23000001)
 *     addiu $t0, $t0, %%lo(D_23000001)
 *   .Leboot_001BD798
 *     lw $t1, %%lo(func_00010064 + 0x28)($a3)
 *     beq $t1, $t0, .Leboot_001BD7B0
 *     lui $t1, %%hi(func_00010064 + 0x28)
 *     sw $t0, %%lo(func_00010064 + 0x28)($t1)
 *     sw $t0, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD7B0
 *     lui $t0, 0x1
 *     and $t1, $a0, $t0
 *     bnez $t1, .Leboot_001BD7EC
 *     lui $t0, 0x1D00
 *     lui $t0, 0x9B00
 *     ext $t1, $a0, 18, 1
 *     or $t0, $t0, $t1
 *     lw $t1, %%lo(func_0001025C + 0x10)($a3)
 *     beq $t1, $t0, .Leboot_001BD7E4
 *     lui $t1, %%hi(func_0001025C + 0x10)
 *     sw $t0, %%lo(func_0001025C + 0x10)($t1)
 *     sw $t0, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD7E4
 *     lui $t0, %%hi(D_1D000001)
 *     addiu $t0, $t0, %%lo(D_1D000001)
 *   .Leboot_001BD7EC
 *     lw $a3, %%lo(func_00010064 + 0x10)($a3)
 *     beq $a3, $t0, .Leboot_001BD804
 *     lui $a3, %%hi(func_00010064 + 0x10)
 *     sw $t0, %%lo(func_00010064 + 0x10)($a3)
 *     sw $t0, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD804
 *     sw $a1, %%lo(sym_001DB710)($a2)
 *     ext $a0, $a0, 8, 1
 *     lui $a1, %%hi(sym_000EC52C)
 *     jr $ra
 *     sb $a0, %%lo(sym_000EC52C)($a1)
 *
 * renderCommon: one phase of the common render path.
 */

#include "types.h"

__attribute__((noreturn)) void renderCommon_1500(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui $a3, %%hi(D_0001039C)\n\t"
        "lui $a2, %%hi(sym_001DB710)\n\t"
        "lw $a0, 0x270($a0)\n\t"
        "lui $t1, 0xE700\n\t"
        "andi $t0, $a0, 0x1\n\t"
        "or $t0, $t0, $t1\n\t"
        "lw $t1, %%lo(D_0001039C)($a3)\n\t"
        "beq $t1, $t0, .Leboot_001BD734\n\t"
        "lw $a1, %%lo(sym_001DB710)($a2)\n\t"
        "lui $t1, %%hi(D_0001039C)\n\t"
        "sw $t0, %%lo(D_0001039C)($t1)\n\t"
        "sw $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD734:\n\t"
        "andi $t1, $a0, 0x10\n\t"
        "bnez $t1, .Leboot_001BD798\n\t"
        "lui $t0, 0x2300\n\t"
        "andi $t0, $a0, 0x20\n\t"
        "beqz $t0, .Leboot_001BD758\n\t"
        "nop\n\t"
        "lui $t0, %%hi(D_DE000007)\n\t"
        "b .Leboot_001BD778\n\t"
        "addiu $t0, $t0, %%lo(D_DE000007)\n\t"
        ".Leboot_001BD758:\n\t"
        "andi $t0, $a0, 0x40\n\t"
        "beqz $t0, .Leboot_001BD770\n\t"
        "nop\n\t"
        "lui $t0, %%hi(D_DE000005)\n\t"
        "b .Leboot_001BD778\n\t"
        "addiu $t0, $t0, %%lo(D_DE000005)\n\t"
        ".Leboot_001BD770:\n\t"
        "lui $t0, %%hi(D_DE000002)\n\t"
        "addiu $t0, $t0, %%lo(D_DE000002)\n\t"
        ".Leboot_001BD778:\n\t"
        "lw $t1, %%lo(D_00010378)($a3)\n\t"
        "beq $t1, $t0, .Leboot_001BD790\n\t"
        "lui $t1, %%hi(D_00010378)\n\t"
        "sw $t0, %%lo(D_00010378)($t1)\n\t"
        "sw $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD790:\n\t"
        "lui $t0, %%hi(D_23000001)\n\t"
        "addiu $t0, $t0, %%lo(D_23000001)\n\t"
        ".Leboot_001BD798:\n\t"
        "lw $t1, %%lo(func_00010064 + 0x28)($a3)\n\t"
        "beq $t1, $t0, .Leboot_001BD7B0\n\t"
        "lui $t1, %%hi(func_00010064 + 0x28)\n\t"
        "sw $t0, %%lo(func_00010064 + 0x28)($t1)\n\t"
        "sw $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD7B0:\n\t"
        "lui $t0, 0x1\n\t"
        "and $t1, $a0, $t0\n\t"
        "bnez $t1, .Leboot_001BD7EC\n\t"
        "lui $t0, 0x1D00\n\t"
        "lui $t0, 0x9B00\n\t"
        "ext $t1, $a0, 18, 1\n\t"
        "or $t0, $t0, $t1\n\t"
        "lw $t1, %%lo(func_0001025C + 0x10)($a3)\n\t"
        "beq $t1, $t0, .Leboot_001BD7E4\n\t"
        "lui $t1, %%hi(func_0001025C + 0x10)\n\t"
        "sw $t0, %%lo(func_0001025C + 0x10)($t1)\n\t"
        "sw $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD7E4:\n\t"
        "lui $t0, %%hi(D_1D000001)\n\t"
        "addiu $t0, $t0, %%lo(D_1D000001)\n\t"
        ".Leboot_001BD7EC:\n\t"
        "lw $a3, %%lo(func_00010064 + 0x10)($a3)\n\t"
        "beq $a3, $t0, .Leboot_001BD804\n\t"
        "lui $a3, %%hi(func_00010064 + 0x10)\n\t"
        "sw $t0, %%lo(func_00010064 + 0x10)($a3)\n\t"
        "sw $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD804:\n\t"
        "sw $a1, %%lo(sym_001DB710)($a2)\n\t"
        "ext $a0, $a0, 8, 1\n\t"
        "lui $a1, %%hi(sym_000EC52C)\n\t"
        "jr $ra\n\t"
        "sb $a0, %%lo(sym_000EC52C)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

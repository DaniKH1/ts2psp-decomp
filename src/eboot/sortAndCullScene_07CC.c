/**
 * The Sims 2 PSP - sortAndCullScene_07CC (0x1B43E8, 0x378 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a1, 0x110($a0)
 *     sw $s0, 0x10($sp)
 *     sw $ra, 0x14($sp)
 *     beqz $a1, .Leboot_001B4440
 *     or $s0, $a0, $zero
 *     ori $a0, $zero, 0x1
 *     sb $a0, 0x11C($s0)
 *     jal func_000AC1C0
 *     or $a0, $s0, $zero
 *     lw $a0, 0x110($s0)
 *     lwc1 $f12, 0x114($s0)
 *     lwc1 $f13, 0x58($a0)
 *     ori $a3, $zero, 0x1F4
 *     swc1 $f13, 0x3E0($s0)
 *     lwc1 $f13, 0x48($a0)
 *     lw $a1, 0x44($a0)
 *     mul.s $f12, $f13, $f12
 *     beq $a1, $a3, .Leboot_001B4448
 *     lw $a2, 0x3F0($s0)
 *     b .Leboot_001B444C
 *     nop
 *   .Leboot_001B4440
 *     b .Leboot_001B4750
 *     nop
 *   .Leboot_001B4448
 *     ori $a1, $zero, 0x5
 *   .Leboot_001B444C
 *     sw $a1, 0x3E8($s0)
 *     swc1 $f12, 0x3E4($s0)
 *     ori $a2, $a2, 0x1000
 *     sw $a2, 0x3F0($s0)
 *     lw $a1, 0x3C($a0)
 *     lui $a3, %%hi(D_FFF0FFFF)
 *     addiu $a3, $a3, %%lo(D_FFF0FFFF)
 *     andi $t0, $a1, 0x20
 *     beqz $t0, .Leboot_001B448C
 *     and $a2, $a2, $a3
 *     lui $a1, 0x1
 *     sw $a2, 0x3F0($s0)
 *     or $a1, $a2, $a1
 *     sw $a1, 0x3F0($s0)
 *     b .Leboot_001B44BC
 *     lw $a2, 0x50($a0)
 *   .Leboot_001B448C
 *     andi $a3, $a1, 0x80
 *     beqz $a3, .Leboot_001B44AC
 *     lui $a1, 0x2
 *     sw $a2, 0x3F0($s0)
 *     or $a1, $a2, $a1
 *     sw $a1, 0x3F0($s0)
 *     b .Leboot_001B44BC
 *     lw $a2, 0x50($a0)
 *   .Leboot_001B44AC
 *     sw $a2, 0x3F0($s0)
 *     or $a1, $a2, $a1
 *     sw $a1, 0x3F0($s0)
 *     lw $a2, 0x50($a0)
 *   .Leboot_001B44BC
 *     bltz $a2, .Leboot_001B4518
 *     slti $a1, $a2, 0x2
 *     beqz $a1, .Leboot_001B44DC
 *     slti $a1, $a2, 0x3
 *     blez $a2, .Leboot_001B44E4
 *     nop
 *     b .Leboot_001B4500
 *     nop
 *   .Leboot_001B44DC
 *     beqz $a1, .Leboot_001B4518
 *     nop
 *   .Leboot_001B44E4
 *     lw $a1, 0x3F0($s0)
 *     addiu $a2, $zero, -0xF1
 *     and $a1, $a1, $a2
 *     sw $a1, 0x3F0($s0)
 *     ori $a1, $a1, 0x20
 *     b .Leboot_001B4518
 *     sw $a1, 0x3F0($s0)
 *   .Leboot_001B4500
 *     lw $a1, 0x3F0($s0)
 *     addiu $a2, $zero, -0xF1
 *     and $a1, $a1, $a2
 *     sw $a1, 0x3F0($s0)
 *     ori $a1, $a1, 0x10
 *     sw $a1, 0x3F0($s0)
 *   .Leboot_001B4518
 *     ori $a1, $zero, 0x7FFF
 *     sh $a1, 0x3FA($s0)
 *     lw $a0, 0x4($a0)
 *     sltiu $a1, $a0, 0x1D
 *     beqz $a1, sym_001B473C
 *     nop
 *     sll $a0, $a0, 2
 *     lui $at, %%hi(sym_001C9278)
 *     addu $at, $at, $a0
 *     lw $at, %%lo(sym_001C9278)($at)
 *     jr $at
 *     nop
 *   sym_001B4548
 *     or $a0, $s0, $zero
 *     jal sortAndCullScene_06A4
 *     or $a1, $zero, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *   sym_001B4560
 *     or $a0, $s0, $zero
 *     jal sortAndCullScene_06A4
 *     ori $a1, $zero, 0x1
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ABAA4
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ABCB0
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *   sym_001B45A0
 *     or $a0, $s0, $zero
 *     jal sortAndCullScene_06A4
 *     or $a1, $zero, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB73C
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB764
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB80C
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB920
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ABCD8
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ABD6C
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ABE2C
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB9D8
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB4A0
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB4C8
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB5F0
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ACB20
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ACCE0
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000ACE74
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB478
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB700
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *     jal func_000AB700
 *     or $a0, $s0, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *   sym_001B470C
 *     or $a0, $s0, $zero
 *     jal sortAndCullScene_06A4
 *     or $a1, $zero, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *   sym_001B4724
 *     or $a0, $s0, $zero
 *     jal sortAndCullScene_06A4
 *     or $a1, $zero, $zero
 *     lw $a0, 0x3F4($s0)
 *     b .Leboot_001B4744
 *     lh $a1, 0x3F8($s0)
 *   sym_001B473C
 *     lw $a0, 0x3F4($s0)
 *     lh $a1, 0x3F8($s0)
 *   .Leboot_001B4744
 *     sw $a0, 0x154($s0)
 *     sw $a1, 0x158($s0)
 *     sb $zero, 0x11D($s0)
 *   .Leboot_001B4750
 *     lw $s0, 0x10($sp)
 *     lw $ra, 0x14($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_07CC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a1, 0x110($a0)\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $ra, 0x14($sp)\n\t"
        "beqz $a1, .Leboot_001B4440\n\t"
        "or $s0, $a0, $zero\n\t"
        "ori $a0, $zero, 0x1\n\t"
        "sb $a0, 0x11C($s0)\n\t"
        "jal func_000AC1C0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x110($s0)\n\t"
        "lwc1 $f12, 0x114($s0)\n\t"
        "lwc1 $f13, 0x58($a0)\n\t"
        "ori $a3, $zero, 0x1F4\n\t"
        "swc1 $f13, 0x3E0($s0)\n\t"
        "lwc1 $f13, 0x48($a0)\n\t"
        "lw $a1, 0x44($a0)\n\t"
        "mul.s $f12, $f13, $f12\n\t"
        "beq $a1, $a3, .Leboot_001B4448\n\t"
        "lw $a2, 0x3F0($s0)\n\t"
        "b .Leboot_001B444C\n\t"
        "nop\n\t"
        ".Leboot_001B4440:\n\t"
        "b .Leboot_001B4750\n\t"
        "nop\n\t"
        ".Leboot_001B4448:\n\t"
        "ori $a1, $zero, 0x5\n\t"
        ".Leboot_001B444C:\n\t"
        "sw $a1, 0x3E8($s0)\n\t"
        "swc1 $f12, 0x3E4($s0)\n\t"
        "ori $a2, $a2, 0x1000\n\t"
        "sw $a2, 0x3F0($s0)\n\t"
        "lw $a1, 0x3C($a0)\n\t"
        "lui $a3, %%hi(D_FFF0FFFF)\n\t"
        "addiu $a3, $a3, %%lo(D_FFF0FFFF)\n\t"
        "andi $t0, $a1, 0x20\n\t"
        "beqz $t0, .Leboot_001B448C\n\t"
        "and $a2, $a2, $a3\n\t"
        "lui $a1, 0x1\n\t"
        "sw $a2, 0x3F0($s0)\n\t"
        "or $a1, $a2, $a1\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        "b .Leboot_001B44BC\n\t"
        "lw $a2, 0x50($a0)\n\t"
        ".Leboot_001B448C:\n\t"
        "andi $a3, $a1, 0x80\n\t"
        "beqz $a3, .Leboot_001B44AC\n\t"
        "lui $a1, 0x2\n\t"
        "sw $a2, 0x3F0($s0)\n\t"
        "or $a1, $a2, $a1\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        "b .Leboot_001B44BC\n\t"
        "lw $a2, 0x50($a0)\n\t"
        ".Leboot_001B44AC:\n\t"
        "sw $a2, 0x3F0($s0)\n\t"
        "or $a1, $a2, $a1\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        "lw $a2, 0x50($a0)\n\t"
        ".Leboot_001B44BC:\n\t"
        "bltz $a2, .Leboot_001B4518\n\t"
        "slti $a1, $a2, 0x2\n\t"
        "beqz $a1, .Leboot_001B44DC\n\t"
        "slti $a1, $a2, 0x3\n\t"
        "blez $a2, .Leboot_001B44E4\n\t"
        "nop\n\t"
        "b .Leboot_001B4500\n\t"
        "nop\n\t"
        ".Leboot_001B44DC:\n\t"
        "beqz $a1, .Leboot_001B4518\n\t"
        "nop\n\t"
        ".Leboot_001B44E4:\n\t"
        "lw $a1, 0x3F0($s0)\n\t"
        "addiu $a2, $zero, -0xF1\n\t"
        "and $a1, $a1, $a2\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        "ori $a1, $a1, 0x20\n\t"
        "b .Leboot_001B4518\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        ".Leboot_001B4500:\n\t"
        "lw $a1, 0x3F0($s0)\n\t"
        "addiu $a2, $zero, -0xF1\n\t"
        "and $a1, $a1, $a2\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        "ori $a1, $a1, 0x10\n\t"
        "sw $a1, 0x3F0($s0)\n\t"
        ".Leboot_001B4518:\n\t"
        "ori $a1, $zero, 0x7FFF\n\t"
        "sh $a1, 0x3FA($s0)\n\t"
        "lw $a0, 0x4($a0)\n\t"
        "sltiu $a1, $a0, 0x1D\n\t"
        "beqz $a1, sym_001B473C\n\t"
        "nop\n\t"
        "sll $a0, $a0, 2\n\t"
        "lui $at, %%hi(sym_001C9278)\n\t"
        "addu $at, $at, $a0\n\t"
        "lw $at, %%lo(sym_001C9278)($at)\n\t"
        "jr $at\n\t"
        "nop\n\t"
        "sym_001B4548:\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal sortAndCullScene_06A4\n\t"
        "or $a1, $zero, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "sym_001B4560:\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal sortAndCullScene_06A4\n\t"
        "ori $a1, $zero, 0x1\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ABAA4\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ABCB0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "sym_001B45A0:\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal sortAndCullScene_06A4\n\t"
        "or $a1, $zero, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB73C\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB764\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB80C\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB920\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ABCD8\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ABD6C\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ABE2C\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB9D8\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB4A0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB4C8\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB5F0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ACB20\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ACCE0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000ACE74\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB478\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB700\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "jal func_000AB700\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "sym_001B470C:\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal sortAndCullScene_06A4\n\t"
        "or $a1, $zero, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "sym_001B4724:\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal sortAndCullScene_06A4\n\t"
        "or $a1, $zero, $zero\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "b .Leboot_001B4744\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        "sym_001B473C:\n\t"
        "lw $a0, 0x3F4($s0)\n\t"
        "lh $a1, 0x3F8($s0)\n\t"
        ".Leboot_001B4744:\n\t"
        "sw $a0, 0x154($s0)\n\t"
        "sw $a1, 0x158($s0)\n\t"
        "sb $zero, 0x11D($s0)\n\t"
        ".Leboot_001B4750:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $ra, 0x14($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

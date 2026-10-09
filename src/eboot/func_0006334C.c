/**
 * The Sims 2 PSP - func_0006334C (0x06334C, 0x1E4 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s3, 0x1C($sp)
 *     addiu $s3, $a0, 0x48
 *     lb $a1, 0x0($s3)
 *     sw $s1, 0x14($sp)
 *     sltiu $a1, $a1, 0x1
 *     sw $s2, 0x18($sp)
 *     addiu $s2, $a0, 0x14C
 *     andi $a1, $a1, 0xFF
 *     addiu $s1, $a0, 0x250
 *     sw $s0, 0x10($sp)
 *     sw $s4, 0x20($sp)
 *     sw $ra, 0x24($sp)
 *     bnez $a1, .Leboot_000633E4
 *     or $s0, $a0, $zero
 *     jal func_000D8A94
 *     or $a0, $s3, $zero
 *     or $s4, $v0, $zero
 *     jal func_000D8CB4
 *     or $a0, $s4, $zero
 *     xori $a0, $v0, 0x2
 *     xori $a1, $v0, 0x1
 *     sltu $a0, $zero, $a0
 *     sltu $a1, $zero, $a1
 *     and $a0, $a0, $a1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_000633CC
 *     nop
 *     lw $a0, 0x0($s4)
 *     ori $a1, $zero, 0x11
 *     bne $a0, $a1, .Leboot_000633E4
 *     nop
 *   .Leboot_000633CC
 *     lbu $a0, 0x354($s0)
 *     beqz $a0, .Leboot_000633E0
 *     nop
 *     jal func_000D8BFC
 *     or $a0, $s4, $zero
 *   .Leboot_000633E0
 *     sb $zero, 0x48($s0)
 *   .Leboot_000633E4
 *     lb $a0, 0x0($s2)
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     bnez $a0, .Leboot_00063454
 *     nop
 *     jal func_000D8A94
 *     or $a0, $s2, $zero
 *     or $s4, $v0, $zero
 *     jal func_000D8CB4
 *     or $a0, $s4, $zero
 *     xori $a0, $v0, 0x2
 *     xori $a1, $v0, 0x1
 *     sltu $a0, $zero, $a0
 *     sltu $a1, $zero, $a1
 *     and $a0, $a0, $a1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_0006343C
 *     nop
 *     lw $a0, 0x0($s4)
 *     ori $a1, $zero, 0x11
 *     bne $a0, $a1, .Leboot_00063454
 *     nop
 *   .Leboot_0006343C
 *     lbu $a0, 0x354($s0)
 *     beqz $a0, .Leboot_00063450
 *     nop
 *     jal func_000D8BFC
 *     or $a0, $s4, $zero
 *   .Leboot_00063450
 *     sb $zero, 0x14C($s0)
 *   .Leboot_00063454
 *     lb $a0, 0x0($s1)
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     bnez $a0, .Leboot_000634C4
 *     nop
 *     jal func_000D8A94
 *     or $a0, $s1, $zero
 *     or $s4, $v0, $zero
 *     jal func_000D8CB4
 *     or $a0, $s4, $zero
 *     xori $a0, $v0, 0x2
 *     xori $a1, $v0, 0x1
 *     sltu $a0, $zero, $a0
 *     sltu $a1, $zero, $a1
 *     and $a0, $a0, $a1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_000634AC
 *     nop
 *     lw $a0, 0x0($s4)
 *     ori $a1, $zero, 0x11
 *     bne $a0, $a1, .Leboot_000634C4
 *     nop
 *   .Leboot_000634AC
 *     lbu $a0, 0x354($s0)
 *     beqz $a0, .Leboot_000634C0
 *     nop
 *     jal func_000D8BFC
 *     or $a0, $s4, $zero
 *   .Leboot_000634C0
 *     sb $zero, 0x250($s0)
 *   .Leboot_000634C4
 *     lb $a0, 0x0($s3)
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_00063500
 *     nop
 *     lb $a0, 0x0($s2)
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_00063500
 *     nop
 *     lb $a0, 0x0($s1)
 *     sltiu $a0, $a0, 0x1
 *     andi $a0, $a0, 0xFF
 *     bnez $a0, .Leboot_00063508
 *     nop
 *   .Leboot_00063500
 *     b .Leboot_00063510
 *     nop
 *   .Leboot_00063508
 *     jal func_00085A30
 *     or $a0, $s0, $zero
 *   .Leboot_00063510
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $s4, 0x20($sp)
 *     lw $ra, 0x24($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_0006334C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "addiu $s3, $a0, 0x48\n\t"
        "lb $a1, 0x0($s3)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "addiu $s2, $a0, 0x14C\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "addiu $s1, $a0, 0x250\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s4, 0x20($sp)\n\t"
        "sw $ra, 0x24($sp)\n\t"
        "bnez $a1, .Leboot_000633E4\n\t"
        "or $s0, $a0, $zero\n\t"
        "jal func_000D8A94\n\t"
        "or $a0, $s3, $zero\n\t"
        "or $s4, $v0, $zero\n\t"
        "jal func_000D8CB4\n\t"
        "or $a0, $s4, $zero\n\t"
        "xori $a0, $v0, 0x2\n\t"
        "xori $a1, $v0, 0x1\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "and $a0, $a0, $a1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_000633CC\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s4)\n\t"
        "ori $a1, $zero, 0x11\n\t"
        "bne $a0, $a1, .Leboot_000633E4\n\t"
        "nop\n\t"
        ".Leboot_000633CC:\n\t"
        "lbu $a0, 0x354($s0)\n\t"
        "beqz $a0, .Leboot_000633E0\n\t"
        "nop\n\t"
        "jal func_000D8BFC\n\t"
        "or $a0, $s4, $zero\n\t"
        ".Leboot_000633E0:\n\t"
        "sb $zero, 0x48($s0)\n\t"
        ".Leboot_000633E4:\n\t"
        "lb $a0, 0x0($s2)\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "bnez $a0, .Leboot_00063454\n\t"
        "nop\n\t"
        "jal func_000D8A94\n\t"
        "or $a0, $s2, $zero\n\t"
        "or $s4, $v0, $zero\n\t"
        "jal func_000D8CB4\n\t"
        "or $a0, $s4, $zero\n\t"
        "xori $a0, $v0, 0x2\n\t"
        "xori $a1, $v0, 0x1\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "and $a0, $a0, $a1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_0006343C\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s4)\n\t"
        "ori $a1, $zero, 0x11\n\t"
        "bne $a0, $a1, .Leboot_00063454\n\t"
        "nop\n\t"
        ".Leboot_0006343C:\n\t"
        "lbu $a0, 0x354($s0)\n\t"
        "beqz $a0, .Leboot_00063450\n\t"
        "nop\n\t"
        "jal func_000D8BFC\n\t"
        "or $a0, $s4, $zero\n\t"
        ".Leboot_00063450:\n\t"
        "sb $zero, 0x14C($s0)\n\t"
        ".Leboot_00063454:\n\t"
        "lb $a0, 0x0($s1)\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "bnez $a0, .Leboot_000634C4\n\t"
        "nop\n\t"
        "jal func_000D8A94\n\t"
        "or $a0, $s1, $zero\n\t"
        "or $s4, $v0, $zero\n\t"
        "jal func_000D8CB4\n\t"
        "or $a0, $s4, $zero\n\t"
        "xori $a0, $v0, 0x2\n\t"
        "xori $a1, $v0, 0x1\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "and $a0, $a0, $a1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_000634AC\n\t"
        "nop\n\t"
        "lw $a0, 0x0($s4)\n\t"
        "ori $a1, $zero, 0x11\n\t"
        "bne $a0, $a1, .Leboot_000634C4\n\t"
        "nop\n\t"
        ".Leboot_000634AC:\n\t"
        "lbu $a0, 0x354($s0)\n\t"
        "beqz $a0, .Leboot_000634C0\n\t"
        "nop\n\t"
        "jal func_000D8BFC\n\t"
        "or $a0, $s4, $zero\n\t"
        ".Leboot_000634C0:\n\t"
        "sb $zero, 0x250($s0)\n\t"
        ".Leboot_000634C4:\n\t"
        "lb $a0, 0x0($s3)\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_00063500\n\t"
        "nop\n\t"
        "lb $a0, 0x0($s2)\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_00063500\n\t"
        "nop\n\t"
        "lb $a0, 0x0($s1)\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "bnez $a0, .Leboot_00063508\n\t"
        "nop\n\t"
        ".Leboot_00063500:\n\t"
        "b .Leboot_00063510\n\t"
        "nop\n\t"
        ".Leboot_00063508:\n\t"
        "jal func_00085A30\n\t"
        "or $a0, $s0, $zero\n\t"
        ".Leboot_00063510:\n\t"
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

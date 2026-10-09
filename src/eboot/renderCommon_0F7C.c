/**
 * The Sims 2 PSP - renderCommon_0F7C (0x1BD17C, 0x41C bytes)
 *
 *     addiu $sp, $sp, -0x40
 *     sw $s0, 0x20($sp)
 *     sw $s1, 0x24($sp)
 *     sw $s2, 0x28($sp)
 *     sw $s3, 0x2C($sp)
 *     sw $s4, 0x30($sp)
 *     sw $ra, 0x34($sp)
 *     lw $s1, 0x4($a0)
 *     lb $a1, 0x0($a0)
 *     andi $a1, $a1, 0x1
 *     beqz $a1, .Leboot_001BD1B4
 *     or $s0, $a0, $zero
 *     lui $a0, %%hi(sym_000EC52C)
 *     sb $zero, %%lo(sym_000EC52C)($a0)
 *   .Leboot_001BD1B4
 *     lb $a0, 0x0($s0)
 *     andi $a0, $a0, 0x2
 *     beqz $a0, .Leboot_001BD518
 *     nop
 *     jal func_00102E6C
 *     addiu $a0, $s1, 0x18
 *     jal renderCommon_1398
 *     nop
 *     lui $a1, %%hi(func_00010064 + 0xC)
 *     lui $a0, %%hi(sym_001DB710)
 *     lw $s3, %%lo(sym_001DB710)($a0)
 *     cache 0x18, 0x3C($s3)
 *     lui $a0, %%hi(sym_000EC52C)
 *     lbu $a0, %%lo(sym_000EC52C)($a0)
 *     lui $a2, 0x1C00
 *     or $a0, $a0, $a2
 *     lw $a1, %%lo(func_00010064 + 0xC)($a1)
 *     beq $a1, $a0, .Leboot_001BD20C
 *     lui $a1, %%hi(func_00010064 + 0xC)
 *     sw $a0, %%lo(func_00010064 + 0xC)($a1)
 *     sw $a0, 0x0($s3)
 *     addiu $s3, $s3, 0x4
 *   .Leboot_001BD20C
 *     addiu $s2, $s1, 0x14
 *     lw $a0, 0x14($s1)
 *     addu $s2, $s2, $a0
 *     lw $a0, 0xC($s1)
 *     addiu $a1, $s1, 0x34
 *     lw $a2, 0x8($a1)
 *     mult $a0, $a2
 *     mflo $a0
 *     lw $a1, 0x1C($a1)
 *     beqz $a1, .Leboot_001BD25C
 *     nop
 *     addiu $a0, $a0, 0x3
 *     addiu $a1, $zero, -0x4
 *     and $a0, $a0, $a1
 *     lw $a1, 0xC($s1)
 *     addiu $a2, $s1, 0x34
 *     lw $a2, 0x20($a2)
 *     mult $a1, $a2
 *     mflo $a1
 *     addu $a0, $a0, $a1
 *   .Leboot_001BD25C
 *     beqz $a0, .Leboot_001BD3EC
 *     lui $a1, 0x1
 *     slt $a1, $a1, $a0
 *     bnez $a1, .Leboot_001BD3EC
 *     lui $a1, %%hi(sym_000EC534)
 *     lw $a1, %%lo(sym_000EC534)($a1)
 *     lw $a1, 0x4($a1)
 *     slt $a1, $a0, $a1
 *     bnez $a1, .Leboot_001BD3EC
 *     or $a1, $s2, $zero
 *     addiu $a0, $a0, 0x3F
 *     sra $a0, $a0, 6
 *     addiu $a0, $a0, -0x1
 *     lui $a2, %%hi(sym_000EC670)
 *     lw $s2, %%lo(sym_000EC670)($a2)
 *     lui $a2, %%hi(sym_001DB384)
 *     lw $a3, %%lo(sym_001DB384)($a2)
 *     sll $t0, $a3, 16
 *     addu $t0, $zero, $t0
 *     addu $s2, $s2, $t0
 *     sll $a3, $a3, 2
 *     lui $t0, %%hi(sym_000EC678)
 *     addiu $t0, $t0, %%lo(sym_000EC678)
 *     addu $a3, $a3, $t0
 *     lw $a3, 0x0($a3)
 *     sll $t0, $a1, 8
 *     srl $t0, $t0, 8
 *     lui $t1, 0xB200
 *     or $t0, $t0, $t1
 *     sw $t0, 0x0($a3)
 *     sll $t0, $s2, 8
 *     srl $t0, $t0, 8
 *     lui $t1, 0xB400
 *     or $t0, $t0, $t1
 *     sw $t0, 0x4($a3)
 *     lui $t0, 0x3F00
 *     and $a1, $a1, $t0
 *     srl $a1, $a1, 8
 *     lui $t1, %%hi(D_B3000010)
 *     addiu $t1, $t1, %%lo(D_B3000010)
 *     or $a1, $a1, $t1
 *     sw $a1, 0x8($a3)
 *     and $a1, $s2, $t0
 *     srl $a1, $a1, 8
 *     lui $t0, %%hi(D_B5000010)
 *     addiu $t0, $t0, %%lo(D_B5000010)
 *     or $a1, $a1, $t0
 *     sw $a1, 0xC($a3)
 *     lui $a1, 0xEB00
 *     sw $a1, 0x10($a3)
 *     lui $a1, 0xEC00
 *     sw $a1, 0x14($a3)
 *     sll $a0, $a0, 10
 *     lui $a1, %%hi(D_EE00000F)
 *     addiu $a1, $a1, %%lo(D_EE00000F)
 *     or $a0, $a0, $a1
 *     sw $a0, 0x18($a3)
 *     lui $a0, %%hi(D_EA000003)
 *     addiu $a0, $a0, %%lo(D_EA000003)
 *     sw $a0, 0x1C($a3)
 *     lui $a0, 0xCC00
 *     sw $a0, 0x20($a3)
 *     lui $a0, 0xCB00
 *     sw $a0, 0x24($a3)
 *     lw $a0, %%lo(sym_001DB384)($a2)
 *     andi $a0, $a0, 0x3
 *     bnez $a0, .Leboot_001BD38C
 *     nop
 *     lui $a0, 0xE08
 *     sw $a0, 0x0($s3)
 *     lui $a0, 0xC00
 *     sw $a0, 0x4($s3)
 *     lui $a1, 0xF00
 *     sw $a1, 0x8($s3)
 *     sw $a0, 0xC($s3)
 *     addiu $s3, $s3, 0x10
 *   .Leboot_001BD38C
 *     lui $a0, %%hi(sym_001DB384)
 *     lw $a1, %%lo(sym_001DB384)($a0)
 *     addiu $a1, $a1, 0x4
 *     andi $a1, $a1, 0x7
 *     sll $a1, $a1, 2
 *     lui $a2, %%hi(sym_000EC678)
 *     addiu $a2, $a2, %%lo(sym_000EC678)
 *     addu $a1, $a1, $a2
 *     sw $s3, 0x0($a1)
 *     sw $zero, 0x0($s3)
 *     sw $zero, 0x4($s3)
 *     sw $zero, 0x8($s3)
 *     sw $zero, 0xC($s3)
 *     sw $zero, 0x10($s3)
 *     sw $zero, 0x14($s3)
 *     sw $zero, 0x18($s3)
 *     sw $zero, 0x1C($s3)
 *     sw $zero, 0x20($s3)
 *     sw $zero, 0x24($s3)
 *     addiu $s3, $s3, 0x28
 *     lw $a1, %%lo(sym_001DB384)($a0)
 *     addiu $a1, $a1, 0x1
 *     andi $a1, $a1, 0x7
 *     sw $a1, %%lo(sym_001DB384)($a0)
 *   .Leboot_001BD3EC
 *     lui $a0, %%hi(sym_001DB710)
 *     sw $s3, %%lo(sym_001DB710)($a0)
 *     lui $a0, %%hi(sym_000EC78C)
 *     lw $a0, %%lo(sym_000EC78C)($a0)
 *     lh $s3, 0x278($a0)
 *     ori $s4, $zero, 0x0
 *     slt $a0, $s4, $s3
 *     beqz $a0, .Leboot_001BD510
 *     nop
 *   .Leboot_001BD410
 *     lui $a0, %%hi(sym_000EC78C)
 *     lw $a0, %%lo(sym_000EC78C)($a0)
 *     addiu $a2, $s1, 0x34
 *     jal renderCommon_0580
 *     or $a1, $s4, $zero
 *     or $a0, $v0, $zero
 *     lui $a1, %%hi(sym_001DB710)
 *     lw $a1, %%lo(sym_001DB710)($a1)
 *     lb $a2, 0x0($s0)
 *     andi $a2, $a2, 0x1
 *     beqz $a2, .Leboot_001BD460
 *     nop
 *     lui $a2, 0xC880
 *     lui $a3, %%hi(D_00010320)
 *     lw $a3, %%lo(D_00010320)($a3)
 *     beq $a3, $a2, .Leboot_001BD460
 *     lui $a3, %%hi(D_00010320)
 *     sw $a2, %%lo(D_00010320)($a3)
 *     sw $a2, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *   .Leboot_001BD460
 *     lw $a2, 0x4($a0)
 *     addu $a2, $s2, $a2
 *     addiu $a3, $s1, 0x8
 *     lw $t0, 0x8($s1)
 *     addu $a3, $a3, $t0
 *     lw $a0, 0x0($a0)
 *     sw $a0, 0x0($a1)
 *     lui $a0, 0x3F00
 *     and $t0, $a2, $a0
 *     srl $t0, $t0, 8
 *     lui $t1, 0x1000
 *     or $t0, $t0, $t1
 *     sw $t0, 0x4($a1)
 *     sll $a2, $a2, 8
 *     srl $a2, $a2, 8
 *     lui $t0, 0x100
 *     or $a2, $a2, $t0
 *     sw $a2, 0x8($a1)
 *     and $a0, $a3, $a0
 *     srl $a0, $a0, 8
 *     or $a0, $a0, $t1
 *     sw $a0, 0xC($a1)
 *     sll $a0, $a3, 8
 *     srl $a0, $a0, 8
 *     lui $a2, 0x200
 *     or $a0, $a0, $a2
 *     sw $a0, 0x10($a1)
 *     lw $a0, 0x4($s1)
 *     lui $a2, 0x404
 *     or $a0, $a0, $a2
 *     sw $a0, 0x14($a1)
 *     addiu $a0, $a1, 0x18
 *     lui $a1, %%hi(sym_001DB710)
 *     sw $a0, %%lo(sym_001DB710)($a1)
 *     lui $a0, %%hi(sym_000EC6A0)
 *     lw $a1, %%lo(sym_000EC6A0)($a0)
 *     lw $a2, 0xC($s1)
 *     addu $a1, $a1, $a2
 *     addiu $a1, $a1, -0x2
 *     sw $a1, %%lo(sym_000EC6A0)($a0)
 *     addiu $s4, $s4, 0x1
 *     slt $a0, $s4, $s3
 *     bnez $a0, .Leboot_001BD410
 *     nop
 *   .Leboot_001BD510
 *     b .Leboot_001BD578
 *     nop
 *   .Leboot_001BD518
 *     lw $a2, 0x4($s1)
 *     addiu $a1, $s1, 0x8
 *     lw $a0, 0x8($s1)
 *     addu $a1, $a1, $a0
 *     lw $a0, 0xC($s1)
 *     lw $t0, 0x14($s1)
 *     ori $t1, $zero, 0x1
 *     beq $t0, $t1, .Leboot_001BD548
 *     ori $a3, $zero, 0x0
 *     addiu $a3, $s1, 0x14
 *     lw $t0, 0x14($s1)
 *     addu $a3, $a3, $t0
 *   .Leboot_001BD548
 *     or $t0, $a0, $zero
 *     lw $t1, 0x10($s1)
 *     lw $t3, 0x0($s1)
 *     lb $a0, 0x0($s0)
 *     andi $a0, $a0, 0x4
 *     sltu $t2, $zero, $a0
 *     andi $t2, $t2, 0xFF
 *     or $a0, $a2, $zero
 *     or $a2, $t0, $zero
 *     or $t0, $t1, $zero
 *     jal drawing_0C04
 *     or $t1, $t3, $zero
 *   .Leboot_001BD578
 *     lw $s0, 0x20($sp)
 *     lw $s1, 0x24($sp)
 *     lw $s2, 0x28($sp)
 *     lw $s3, 0x2C($sp)
 *     lw $s4, 0x30($sp)
 *     lw $ra, 0x34($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x40
 *
 * renderCommon: one phase of the common render path.
 */

#include "types.h"

__attribute__((noreturn)) void renderCommon_0F7C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "sw $s0, 0x20($sp)\n\t"
        "sw $s1, 0x24($sp)\n\t"
        "sw $s2, 0x28($sp)\n\t"
        "sw $s3, 0x2C($sp)\n\t"
        "sw $s4, 0x30($sp)\n\t"
        "sw $ra, 0x34($sp)\n\t"
        "lw $s1, 0x4($a0)\n\t"
        "lb $a1, 0x0($a0)\n\t"
        "andi $a1, $a1, 0x1\n\t"
        "beqz $a1, .Leboot_001BD1B4\n\t"
        "or $s0, $a0, $zero\n\t"
        "lui $a0, %%hi(sym_000EC52C)\n\t"
        "sb $zero, %%lo(sym_000EC52C)($a0)\n\t"
        ".Leboot_001BD1B4:\n\t"
        "lb $a0, 0x0($s0)\n\t"
        "andi $a0, $a0, 0x2\n\t"
        "beqz $a0, .Leboot_001BD518\n\t"
        "nop\n\t"
        "jal func_00102E6C\n\t"
        "addiu $a0, $s1, 0x18\n\t"
        "jal renderCommon_1398\n\t"
        "nop\n\t"
        "lui $a1, %%hi(func_00010064 + 0xC)\n\t"
        "lui $a0, %%hi(sym_001DB710)\n\t"
        "lw $s3, %%lo(sym_001DB710)($a0)\n\t"
        "cache 0x18, 0x3C($s3)\n\t"
        "lui $a0, %%hi(sym_000EC52C)\n\t"
        "lbu $a0, %%lo(sym_000EC52C)($a0)\n\t"
        "lui $a2, 0x1C00\n\t"
        "or $a0, $a0, $a2\n\t"
        "lw $a1, %%lo(func_00010064 + 0xC)($a1)\n\t"
        "beq $a1, $a0, .Leboot_001BD20C\n\t"
        "lui $a1, %%hi(func_00010064 + 0xC)\n\t"
        "sw $a0, %%lo(func_00010064 + 0xC)($a1)\n\t"
        "sw $a0, 0x0($s3)\n\t"
        "addiu $s3, $s3, 0x4\n\t"
        ".Leboot_001BD20C:\n\t"
        "addiu $s2, $s1, 0x14\n\t"
        "lw $a0, 0x14($s1)\n\t"
        "addu $s2, $s2, $a0\n\t"
        "lw $a0, 0xC($s1)\n\t"
        "addiu $a1, $s1, 0x34\n\t"
        "lw $a2, 0x8($a1)\n\t"
        "mult $a0, $a2\n\t"
        "mflo $a0\n\t"
        "lw $a1, 0x1C($a1)\n\t"
        "beqz $a1, .Leboot_001BD25C\n\t"
        "nop\n\t"
        "addiu $a0, $a0, 0x3\n\t"
        "addiu $a1, $zero, -0x4\n\t"
        "and $a0, $a0, $a1\n\t"
        "lw $a1, 0xC($s1)\n\t"
        "addiu $a2, $s1, 0x34\n\t"
        "lw $a2, 0x20($a2)\n\t"
        "mult $a1, $a2\n\t"
        "mflo $a1\n\t"
        "addu $a0, $a0, $a1\n\t"
        ".Leboot_001BD25C:\n\t"
        "beqz $a0, .Leboot_001BD3EC\n\t"
        "lui $a1, 0x1\n\t"
        "slt $a1, $a1, $a0\n\t"
        "bnez $a1, .Leboot_001BD3EC\n\t"
        "lui $a1, %%hi(sym_000EC534)\n\t"
        "lw $a1, %%lo(sym_000EC534)($a1)\n\t"
        "lw $a1, 0x4($a1)\n\t"
        "slt $a1, $a0, $a1\n\t"
        "bnez $a1, .Leboot_001BD3EC\n\t"
        "or $a1, $s2, $zero\n\t"
        "addiu $a0, $a0, 0x3F\n\t"
        "sra $a0, $a0, 6\n\t"
        "addiu $a0, $a0, -0x1\n\t"
        "lui $a2, %%hi(sym_000EC670)\n\t"
        "lw $s2, %%lo(sym_000EC670)($a2)\n\t"
        "lui $a2, %%hi(sym_001DB384)\n\t"
        "lw $a3, %%lo(sym_001DB384)($a2)\n\t"
        "sll $t0, $a3, 16\n\t"
        "addu $t0, $zero, $t0\n\t"
        "addu $s2, $s2, $t0\n\t"
        "sll $a3, $a3, 2\n\t"
        "lui $t0, %%hi(sym_000EC678)\n\t"
        "addiu $t0, $t0, %%lo(sym_000EC678)\n\t"
        "addu $a3, $a3, $t0\n\t"
        "lw $a3, 0x0($a3)\n\t"
        "sll $t0, $a1, 8\n\t"
        "srl $t0, $t0, 8\n\t"
        "lui $t1, 0xB200\n\t"
        "or $t0, $t0, $t1\n\t"
        "sw $t0, 0x0($a3)\n\t"
        "sll $t0, $s2, 8\n\t"
        "srl $t0, $t0, 8\n\t"
        "lui $t1, 0xB400\n\t"
        "or $t0, $t0, $t1\n\t"
        "sw $t0, 0x4($a3)\n\t"
        "lui $t0, 0x3F00\n\t"
        "and $a1, $a1, $t0\n\t"
        "srl $a1, $a1, 8\n\t"
        "lui $t1, %%hi(D_B3000010)\n\t"
        "addiu $t1, $t1, %%lo(D_B3000010)\n\t"
        "or $a1, $a1, $t1\n\t"
        "sw $a1, 0x8($a3)\n\t"
        "and $a1, $s2, $t0\n\t"
        "srl $a1, $a1, 8\n\t"
        "lui $t0, %%hi(D_B5000010)\n\t"
        "addiu $t0, $t0, %%lo(D_B5000010)\n\t"
        "or $a1, $a1, $t0\n\t"
        "sw $a1, 0xC($a3)\n\t"
        "lui $a1, 0xEB00\n\t"
        "sw $a1, 0x10($a3)\n\t"
        "lui $a1, 0xEC00\n\t"
        "sw $a1, 0x14($a3)\n\t"
        "sll $a0, $a0, 10\n\t"
        "lui $a1, %%hi(D_EE00000F)\n\t"
        "addiu $a1, $a1, %%lo(D_EE00000F)\n\t"
        "or $a0, $a0, $a1\n\t"
        "sw $a0, 0x18($a3)\n\t"
        "lui $a0, %%hi(D_EA000003)\n\t"
        "addiu $a0, $a0, %%lo(D_EA000003)\n\t"
        "sw $a0, 0x1C($a3)\n\t"
        "lui $a0, 0xCC00\n\t"
        "sw $a0, 0x20($a3)\n\t"
        "lui $a0, 0xCB00\n\t"
        "sw $a0, 0x24($a3)\n\t"
        "lw $a0, %%lo(sym_001DB384)($a2)\n\t"
        "andi $a0, $a0, 0x3\n\t"
        "bnez $a0, .Leboot_001BD38C\n\t"
        "nop\n\t"
        "lui $a0, 0xE08\n\t"
        "sw $a0, 0x0($s3)\n\t"
        "lui $a0, 0xC00\n\t"
        "sw $a0, 0x4($s3)\n\t"
        "lui $a1, 0xF00\n\t"
        "sw $a1, 0x8($s3)\n\t"
        "sw $a0, 0xC($s3)\n\t"
        "addiu $s3, $s3, 0x10\n\t"
        ".Leboot_001BD38C:\n\t"
        "lui $a0, %%hi(sym_001DB384)\n\t"
        "lw $a1, %%lo(sym_001DB384)($a0)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "andi $a1, $a1, 0x7\n\t"
        "sll $a1, $a1, 2\n\t"
        "lui $a2, %%hi(sym_000EC678)\n\t"
        "addiu $a2, $a2, %%lo(sym_000EC678)\n\t"
        "addu $a1, $a1, $a2\n\t"
        "sw $s3, 0x0($a1)\n\t"
        "sw $zero, 0x0($s3)\n\t"
        "sw $zero, 0x4($s3)\n\t"
        "sw $zero, 0x8($s3)\n\t"
        "sw $zero, 0xC($s3)\n\t"
        "sw $zero, 0x10($s3)\n\t"
        "sw $zero, 0x14($s3)\n\t"
        "sw $zero, 0x18($s3)\n\t"
        "sw $zero, 0x1C($s3)\n\t"
        "sw $zero, 0x20($s3)\n\t"
        "sw $zero, 0x24($s3)\n\t"
        "addiu $s3, $s3, 0x28\n\t"
        "lw $a1, %%lo(sym_001DB384)($a0)\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "andi $a1, $a1, 0x7\n\t"
        "sw $a1, %%lo(sym_001DB384)($a0)\n\t"
        ".Leboot_001BD3EC:\n\t"
        "lui $a0, %%hi(sym_001DB710)\n\t"
        "sw $s3, %%lo(sym_001DB710)($a0)\n\t"
        "lui $a0, %%hi(sym_000EC78C)\n\t"
        "lw $a0, %%lo(sym_000EC78C)($a0)\n\t"
        "lh $s3, 0x278($a0)\n\t"
        "ori $s4, $zero, 0x0\n\t"
        "slt $a0, $s4, $s3\n\t"
        "beqz $a0, .Leboot_001BD510\n\t"
        "nop\n\t"
        ".Leboot_001BD410:\n\t"
        "lui $a0, %%hi(sym_000EC78C)\n\t"
        "lw $a0, %%lo(sym_000EC78C)($a0)\n\t"
        "addiu $a2, $s1, 0x34\n\t"
        "jal renderCommon_0580\n\t"
        "or $a1, $s4, $zero\n\t"
        "or $a0, $v0, $zero\n\t"
        "lui $a1, %%hi(sym_001DB710)\n\t"
        "lw $a1, %%lo(sym_001DB710)($a1)\n\t"
        "lb $a2, 0x0($s0)\n\t"
        "andi $a2, $a2, 0x1\n\t"
        "beqz $a2, .Leboot_001BD460\n\t"
        "nop\n\t"
        "lui $a2, 0xC880\n\t"
        "lui $a3, %%hi(D_00010320)\n\t"
        "lw $a3, %%lo(D_00010320)($a3)\n\t"
        "beq $a3, $a2, .Leboot_001BD460\n\t"
        "lui $a3, %%hi(D_00010320)\n\t"
        "sw $a2, %%lo(D_00010320)($a3)\n\t"
        "sw $a2, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".Leboot_001BD460:\n\t"
        "lw $a2, 0x4($a0)\n\t"
        "addu $a2, $s2, $a2\n\t"
        "addiu $a3, $s1, 0x8\n\t"
        "lw $t0, 0x8($s1)\n\t"
        "addu $a3, $a3, $t0\n\t"
        "lw $a0, 0x0($a0)\n\t"
        "sw $a0, 0x0($a1)\n\t"
        "lui $a0, 0x3F00\n\t"
        "and $t0, $a2, $a0\n\t"
        "srl $t0, $t0, 8\n\t"
        "lui $t1, 0x1000\n\t"
        "or $t0, $t0, $t1\n\t"
        "sw $t0, 0x4($a1)\n\t"
        "sll $a2, $a2, 8\n\t"
        "srl $a2, $a2, 8\n\t"
        "lui $t0, 0x100\n\t"
        "or $a2, $a2, $t0\n\t"
        "sw $a2, 0x8($a1)\n\t"
        "and $a0, $a3, $a0\n\t"
        "srl $a0, $a0, 8\n\t"
        "or $a0, $a0, $t1\n\t"
        "sw $a0, 0xC($a1)\n\t"
        "sll $a0, $a3, 8\n\t"
        "srl $a0, $a0, 8\n\t"
        "lui $a2, 0x200\n\t"
        "or $a0, $a0, $a2\n\t"
        "sw $a0, 0x10($a1)\n\t"
        "lw $a0, 0x4($s1)\n\t"
        "lui $a2, 0x404\n\t"
        "or $a0, $a0, $a2\n\t"
        "sw $a0, 0x14($a1)\n\t"
        "addiu $a0, $a1, 0x18\n\t"
        "lui $a1, %%hi(sym_001DB710)\n\t"
        "sw $a0, %%lo(sym_001DB710)($a1)\n\t"
        "lui $a0, %%hi(sym_000EC6A0)\n\t"
        "lw $a1, %%lo(sym_000EC6A0)($a0)\n\t"
        "lw $a2, 0xC($s1)\n\t"
        "addu $a1, $a1, $a2\n\t"
        "addiu $a1, $a1, -0x2\n\t"
        "sw $a1, %%lo(sym_000EC6A0)($a0)\n\t"
        "addiu $s4, $s4, 0x1\n\t"
        "slt $a0, $s4, $s3\n\t"
        "bnez $a0, .Leboot_001BD410\n\t"
        "nop\n\t"
        ".Leboot_001BD510:\n\t"
        "b .Leboot_001BD578\n\t"
        "nop\n\t"
        ".Leboot_001BD518:\n\t"
        "lw $a2, 0x4($s1)\n\t"
        "addiu $a1, $s1, 0x8\n\t"
        "lw $a0, 0x8($s1)\n\t"
        "addu $a1, $a1, $a0\n\t"
        "lw $a0, 0xC($s1)\n\t"
        "lw $t0, 0x14($s1)\n\t"
        "ori $t1, $zero, 0x1\n\t"
        "beq $t0, $t1, .Leboot_001BD548\n\t"
        "ori $a3, $zero, 0x0\n\t"
        "addiu $a3, $s1, 0x14\n\t"
        "lw $t0, 0x14($s1)\n\t"
        "addu $a3, $a3, $t0\n\t"
        ".Leboot_001BD548:\n\t"
        "or $t0, $a0, $zero\n\t"
        "lw $t1, 0x10($s1)\n\t"
        "lw $t3, 0x0($s1)\n\t"
        "lb $a0, 0x0($s0)\n\t"
        "andi $a0, $a0, 0x4\n\t"
        "sltu $t2, $zero, $a0\n\t"
        "andi $t2, $t2, 0xFF\n\t"
        "or $a0, $a2, $zero\n\t"
        "or $a2, $t0, $zero\n\t"
        "or $t0, $t1, $zero\n\t"
        "jal drawing_0C04\n\t"
        "or $t1, $t3, $zero\n\t"
        ".Leboot_001BD578:\n\t"
        "lw $s0, 0x20($sp)\n\t"
        "lw $s1, 0x24($sp)\n\t"
        "lw $s2, 0x28($sp)\n\t"
        "lw $s3, 0x2C($sp)\n\t"
        "lw $s4, 0x30($sp)\n\t"
        "lw $ra, 0x34($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

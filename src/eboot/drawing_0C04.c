/**
 * The Sims 2 PSP - drawing_0C04 (0x1BBE84, 0x36C bytes)
 *
 *     addiu $sp, $sp, -0x50
 *     sw $s5, 0x30($sp)
 *     or $s5, $a0, $zero
 *     sw $s0, 0x1C($sp)
 *     sw $s1, 0x20($sp)
 *     sw $s2, 0x24($sp)
 *     sw $s3, 0x28($sp)
 *     sw $s4, 0x2C($sp)
 *     lui $a0, %%hi(sym_001DB10C)
 *     or $s4, $t0, $zero
 *     or $s2, $a3, $zero
 *     sw $t1, 0x14($sp)
 *     or $s1, $a2, $zero
 *     or $s0, $a1, $zero
 *     andi $s3, $t2, 0xFF
 *     sw $s6, 0x34($sp)
 *     sw $s7, 0x38($sp)
 *     sw $fp, 0x3C($sp)
 *     sw $ra, 0x40($sp)
 *     jal func_00102E6C
 *     addiu $a0, $a0, %%lo(sym_001DB10C)
 *     jal renderCommon_1398
 *     nop
 *     lui $a1, %%hi(sym_000EC6A0)
 *     lw $a2, %%lo(sym_000EC6A0)($a1)
 *     addu $a2, $a2, $s1
 *     or $a0, $s4, $zero
 *     sw $a2, %%lo(sym_000EC6A0)($a1)
 *     bgtz $a0, .Leboot_001BBF0C
 *     sw $s5, 0x10($sp)
 *     bltz $a0, .Leboot_001BC1C0
 *     nop
 *     b .Leboot_001BBF28
 *     nop
 *   .Leboot_001BBF0C
 *     slti $a1, $a0, 0x2
 *     bnez $a1, .Leboot_001BC1B8
 *     slti $a0, $a0, 0x3
 *     beqz $a0, .Leboot_001BC1C0
 *     nop
 *     b .Leboot_001BC1C0
 *     nop
 *   .Leboot_001BBF28
 *     lui $a3, %%hi(sym_000EC78C)
 *     lw $a0, %%lo(sym_000EC78C)($a3)
 *     ori $a2, $zero, 0x0
 *     lh $t0, 0x278($a0)
 *     addiu $a1, $zero, -0x40
 *     slt $t0, $a2, $t0
 *     beqz $t0, .Leboot_001BBF60
 *     lui $a0, %%hi(sym_000EC66C)
 *     lw $t0, %%lo(sym_000EC78C)($a3)
 *   .Leboot_001BBF4C
 *     addiu $a2, $a2, 0x1
 *     lh $t0, 0x278($t0)
 *     slt $t0, $a2, $t0
 *     bnel $t0, $zero, .Leboot_001BBF4C
 *     lw $t0, %%lo(sym_000EC78C)($a3)
 *   .Leboot_001BBF60
 *     lw $a2, %%lo(sym_000EC66C)($a0)
 *     sll $a3, $s1, 5
 *     and $a2, $a2, $a1
 *     subu $s5, $a2, $a3
 *     and $s5, $s5, $a1
 *     ori $a3, $zero, 0x0
 *     sw $s5, %%lo(sym_000EC66C)($a0)
 *     slt $t0, $a3, $s1
 *     beqz $t0, .Leboot_001BC018
 *     or $a2, $s5, $zero
 *   .Leboot_001BBF88
 *     lv.s S100, 0xC($s2)
 *     lv.s S110, 0x10($s2)
 *     lv.s S120, 0x14($s2)
 *     lv.s S101, 0x38($s2)
 *     lv.s S111, 0x3C($s2)
 *     lv.s S121, 0x40($s2)
 *     lv.s S020, 0x18($s2)
 *     lv.s S022, 0x44($s2)
 *     vf2in.t R100, R100, 30
 *     vf2in.t R101, R101, 30
 *     lv.s S000, 0x1C($s2)
 *     lv.s S010, 0x20($s2)
 *     lv.s S002, 0x48($s2)
 *     lv.s S012, 0x4C($s2)
 *     vi2s.q R100, R100
 *     vi2s.q R101, R101
 *     lv.s S011, 0x0($s2)
 *     lv.s S021, 0x4($s2)
 *     lv.s S031, 0x8($s2)
 *     vmov.s S030, S100
 *     vmov.s S001, S110
 *     vmov.s S032, S101
 *     vmov.s S003, S111
 *     lv.s S013, 0x2C($s2)
 *     lv.s S023, 0x30($s2)
 *     lv.s S033, 0x34($s2)
 *     sv.q R000, 0x0($a2), wb
 *     sv.q R001, 0x10($a2), wb
 *     sv.q R002, 0x20($a2), wb
 *     sv.q R003, 0x30($a2), wb
 *     addiu $s2, $s2, 0x58
 *     addiu $a2, $a2, 0x40
 *     addiu $a3, $a3, 0x2
 *     slt $t0, $a3, $s1
 *     bnez $t0, .Leboot_001BBF88
 *     nop
 *   .Leboot_001BC018
 *     beqz $s3, .Leboot_001BC0C4
 *     nop
 *     lw $a3, 0x10($sp)
 *     lw $a2, %%lo(sym_000EC66C)($a0)
 *     addu $t0, $a3, $a3
 *     and $a2, $a2, $a1
 *     subu $a2, $a2, $t0
 *     and $a1, $a2, $a1
 *     sw $a1, %%lo(sym_000EC66C)($a0)
 *     ori $a2, $zero, 0x0
 *     slt $t0, $a2, $a3
 *     beqz $t0, .Leboot_001BC0BC
 *     or $a0, $a1, $zero
 *   .Leboot_001BC04C
 *     vnop
 *     lv.s S000, 0x0($s0)
 *     lv.s S000, 0x0($s0)
 *     lv.s S010, 0x4($s0)
 *     lv.s S020, 0x8($s0)
 *     lv.s S030, 0xC($s0)
 *     lv.s S001, 0x10($s0)
 *     lv.s S011, 0x14($s0)
 *     lv.s S021, 0x18($s0)
 *     lv.s S031, 0x1C($s0)
 *     lv.s S002, 0x20($s0)
 *     lv.s S012, 0x24($s0)
 *     lv.s S022, 0x28($s0)
 *     lv.s S032, 0x2C($s0)
 *     lv.s S003, 0x30($s0)
 *     lv.s S013, 0x34($s0)
 *     lv.s S023, 0x38($s0)
 *     lv.s S033, 0x3C($s0)
 *     sv.q R000, 0x0($a1), wb
 *     sv.q R001, 0x10($a1), wb
 *     sv.q R002, 0x20($a1), wb
 *     sv.q R003, 0x30($a1), wb
 *     addiu $s0, $s0, 0x40
 *     addiu $a1, $a1, 0x40
 *     addiu $a2, $a2, 0x20
 *     slt $t0, $a2, $a3
 *     bnez $t0, .Leboot_001BC04C
 *     nop
 *   .Leboot_001BC0BC
 *     b .Leboot_001BC0C8
 *     nop
 *   .Leboot_001BC0C4
 *     or $a0, $s0, $zero
 *   .Leboot_001BC0C8
 *     lui $a1, %%hi(sym_000EC78C)
 *     lw $a1, %%lo(sym_000EC78C)($a1)
 *     ori $s0, $zero, 0x0
 *     lh $a1, 0x278($a1)
 *     slt $a1, $s0, $a1
 *     beqz $a1, .Leboot_001BC1B0
 *     lui $a1, 0x3F00
 *     and $a2, $s5, $a1
 *     srl $s6, $a2, 8
 *     sll $a2, $s5, 8
 *     and $a1, $a0, $a1
 *     srl $s5, $a2, 8
 *     sll $a0, $a0, 8
 *     lui $a2, 0x100
 *     srl $s3, $a0, 8
 *     or $s5, $s5, $a2
 *     lui $a0, 0x200
 *     lw $a2, 0x14($sp)
 *     or $s3, $s3, $a0
 *     lui $s4, 0x1000
 *     lui $a0, %%hi(sym_001DB17C)
 *     or $s6, $s6, $s4
 *     srl $a1, $a1, 8
 *     sll $s2, $a2, 2
 *     addiu $a0, $a0, %%lo(sym_001DB17C)
 *     lui $fp, %%hi(sym_001DB31C)
 *     lui $s7, %%hi(D_120011DF)
 *     or $s4, $a1, $s4
 *     addu $s2, $s2, $a0
 *     addiu $fp, $fp, %%lo(sym_001DB31C)
 *     addiu $s7, $s7, %%lo(D_120011DF)
 *     lui $s1, %%hi(sym_001DB710)
 *   .Leboot_001BC148
 *     sw $s2, 0x18($sp)
 *     lui $s2, %%hi(sym_000EC78C)
 *     lw $a0, %%lo(sym_000EC78C)($s2)
 *     or $a1, $s0, $zero
 *     jal renderCommon_0580
 *     or $a2, $fp, $zero
 *     lw $a0, %%lo(sym_001DB710)($s1)
 *     sw $s7, 0x0($a0)
 *     sw $s6, 0x4($a0)
 *     sw $s5, 0x8($a0)
 *     sw $s4, 0xC($a0)
 *     sw $s3, 0x10($a0)
 *     lw $a1, 0x10($sp)
 *     lw $s2, 0x18($sp)
 *     addiu $a3, $a0, 0x18
 *     lw $a2, 0x0($s2)
 *     addiu $s0, $s0, 0x1
 *     or $a1, $a2, $a1
 *     sw $a1, 0x14($a0)
 *     sw $a3, %%lo(sym_001DB710)($s1)
 *     lui $a0, %%hi(sym_000EC78C)
 *     lw $a0, %%lo(sym_000EC78C)($a0)
 *     lh $a0, 0x278($a0)
 *     slt $a0, $s0, $a0
 *     bnez $a0, .Leboot_001BC148
 *     nop
 *   .Leboot_001BC1B0
 *     b .Leboot_001BC1C0
 *     nop
 *   .Leboot_001BC1B8
 *     b .Leboot_001BC1C0
 *     nop
 *   .Leboot_001BC1C0
 *     lw $s0, 0x1C($sp)
 *     lw $s1, 0x20($sp)
 *     lw $s2, 0x24($sp)
 *     lw $s3, 0x28($sp)
 *     lw $s4, 0x2C($sp)
 *     lw $s5, 0x30($sp)
 *     lw $s6, 0x34($sp)
 *     lw $s7, 0x38($sp)
 *     lw $fp, 0x3C($sp)
 *     lw $ra, 0x40($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x50
 *     nop
 *     nop
 *     nop
 *     nop
 *
 * drawing: one phase of the drawing pass.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_0C04(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x50\n\t"
        "sw $s5, 0x30($sp)\n\t"
        "or $s5, $a0, $zero\n\t"
        "sw $s0, 0x1C($sp)\n\t"
        "sw $s1, 0x20($sp)\n\t"
        "sw $s2, 0x24($sp)\n\t"
        "sw $s3, 0x28($sp)\n\t"
        "sw $s4, 0x2C($sp)\n\t"
        "lui $a0, %%hi(sym_001DB10C)\n\t"
        "or $s4, $t0, $zero\n\t"
        "or $s2, $a3, $zero\n\t"
        "sw $t1, 0x14($sp)\n\t"
        "or $s1, $a2, $zero\n\t"
        "or $s0, $a1, $zero\n\t"
        "andi $s3, $t2, 0xFF\n\t"
        "sw $s6, 0x34($sp)\n\t"
        "sw $s7, 0x38($sp)\n\t"
        "sw $fp, 0x3C($sp)\n\t"
        "sw $ra, 0x40($sp)\n\t"
        "jal func_00102E6C\n\t"
        "addiu $a0, $a0, %%lo(sym_001DB10C)\n\t"
        "jal renderCommon_1398\n\t"
        "nop\n\t"
        "lui $a1, %%hi(sym_000EC6A0)\n\t"
        "lw $a2, %%lo(sym_000EC6A0)($a1)\n\t"
        "addu $a2, $a2, $s1\n\t"
        "or $a0, $s4, $zero\n\t"
        "sw $a2, %%lo(sym_000EC6A0)($a1)\n\t"
        "bgtz $a0, .Leboot_001BBF0C\n\t"
        "sw $s5, 0x10($sp)\n\t"
        "bltz $a0, .Leboot_001BC1C0\n\t"
        "nop\n\t"
        "b .Leboot_001BBF28\n\t"
        "nop\n\t"
        ".Leboot_001BBF0C:\n\t"
        "slti $a1, $a0, 0x2\n\t"
        "bnez $a1, .Leboot_001BC1B8\n\t"
        "slti $a0, $a0, 0x3\n\t"
        "beqz $a0, .Leboot_001BC1C0\n\t"
        "nop\n\t"
        "b .Leboot_001BC1C0\n\t"
        "nop\n\t"
        ".Leboot_001BBF28:\n\t"
        "lui $a3, %%hi(sym_000EC78C)\n\t"
        "lw $a0, %%lo(sym_000EC78C)($a3)\n\t"
        "ori $a2, $zero, 0x0\n\t"
        "lh $t0, 0x278($a0)\n\t"
        "addiu $a1, $zero, -0x40\n\t"
        "slt $t0, $a2, $t0\n\t"
        "beqz $t0, .Leboot_001BBF60\n\t"
        "lui $a0, %%hi(sym_000EC66C)\n\t"
        "lw $t0, %%lo(sym_000EC78C)($a3)\n\t"
        ".Leboot_001BBF4C:\n\t"
        "addiu $a2, $a2, 0x1\n\t"
        "lh $t0, 0x278($t0)\n\t"
        "slt $t0, $a2, $t0\n\t"
        "bnel $t0, $zero, .Leboot_001BBF4C\n\t"
        "lw $t0, %%lo(sym_000EC78C)($a3)\n\t"
        ".Leboot_001BBF60:\n\t"
        "lw $a2, %%lo(sym_000EC66C)($a0)\n\t"
        "sll $a3, $s1, 5\n\t"
        "and $a2, $a2, $a1\n\t"
        "subu $s5, $a2, $a3\n\t"
        "and $s5, $s5, $a1\n\t"
        "ori $a3, $zero, 0x0\n\t"
        "sw $s5, %%lo(sym_000EC66C)($a0)\n\t"
        "slt $t0, $a3, $s1\n\t"
        "beqz $t0, .Leboot_001BC018\n\t"
        "or $a2, $s5, $zero\n\t"
        ".Leboot_001BBF88:\n\t"
        "lv.s S100, 0xC($s2)\n\t"
        "lv.s S110, 0x10($s2)\n\t"
        "lv.s S120, 0x14($s2)\n\t"
        "lv.s S101, 0x38($s2)\n\t"
        "lv.s S111, 0x3C($s2)\n\t"
        "lv.s S121, 0x40($s2)\n\t"
        "lv.s S020, 0x18($s2)\n\t"
        "lv.s S022, 0x44($s2)\n\t"
        "vf2in.t R100, R100, 30\n\t"
        "vf2in.t R101, R101, 30\n\t"
        "lv.s S000, 0x1C($s2)\n\t"
        "lv.s S010, 0x20($s2)\n\t"
        "lv.s S002, 0x48($s2)\n\t"
        "lv.s S012, 0x4C($s2)\n\t"
        "vi2s.q R100, R100\n\t"
        "vi2s.q R101, R101\n\t"
        "lv.s S011, 0x0($s2)\n\t"
        "lv.s S021, 0x4($s2)\n\t"
        "lv.s S031, 0x8($s2)\n\t"
        "vmov.s S030, S100\n\t"
        "vmov.s S001, S110\n\t"
        "vmov.s S032, S101\n\t"
        "vmov.s S003, S111\n\t"
        "lv.s S013, 0x2C($s2)\n\t"
        "lv.s S023, 0x30($s2)\n\t"
        "lv.s S033, 0x34($s2)\n\t"
        "sv.q R000, 0x0($a2), wb\n\t"
        "sv.q R001, 0x10($a2), wb\n\t"
        "sv.q R002, 0x20($a2), wb\n\t"
        "sv.q R003, 0x30($a2), wb\n\t"
        "addiu $s2, $s2, 0x58\n\t"
        "addiu $a2, $a2, 0x40\n\t"
        "addiu $a3, $a3, 0x2\n\t"
        "slt $t0, $a3, $s1\n\t"
        "bnez $t0, .Leboot_001BBF88\n\t"
        "nop\n\t"
        ".Leboot_001BC018:\n\t"
        "beqz $s3, .Leboot_001BC0C4\n\t"
        "nop\n\t"
        "lw $a3, 0x10($sp)\n\t"
        "lw $a2, %%lo(sym_000EC66C)($a0)\n\t"
        "addu $t0, $a3, $a3\n\t"
        "and $a2, $a2, $a1\n\t"
        "subu $a2, $a2, $t0\n\t"
        "and $a1, $a2, $a1\n\t"
        "sw $a1, %%lo(sym_000EC66C)($a0)\n\t"
        "ori $a2, $zero, 0x0\n\t"
        "slt $t0, $a2, $a3\n\t"
        "beqz $t0, .Leboot_001BC0BC\n\t"
        "or $a0, $a1, $zero\n\t"
        ".Leboot_001BC04C:\n\t"
        "vnop\n\t"
        "lv.s S000, 0x0($s0)\n\t"
        "lv.s S000, 0x0($s0)\n\t"
        "lv.s S010, 0x4($s0)\n\t"
        "lv.s S020, 0x8($s0)\n\t"
        "lv.s S030, 0xC($s0)\n\t"
        "lv.s S001, 0x10($s0)\n\t"
        "lv.s S011, 0x14($s0)\n\t"
        "lv.s S021, 0x18($s0)\n\t"
        "lv.s S031, 0x1C($s0)\n\t"
        "lv.s S002, 0x20($s0)\n\t"
        "lv.s S012, 0x24($s0)\n\t"
        "lv.s S022, 0x28($s0)\n\t"
        "lv.s S032, 0x2C($s0)\n\t"
        "lv.s S003, 0x30($s0)\n\t"
        "lv.s S013, 0x34($s0)\n\t"
        "lv.s S023, 0x38($s0)\n\t"
        "lv.s S033, 0x3C($s0)\n\t"
        "sv.q R000, 0x0($a1), wb\n\t"
        "sv.q R001, 0x10($a1), wb\n\t"
        "sv.q R002, 0x20($a1), wb\n\t"
        "sv.q R003, 0x30($a1), wb\n\t"
        "addiu $s0, $s0, 0x40\n\t"
        "addiu $a1, $a1, 0x40\n\t"
        "addiu $a2, $a2, 0x20\n\t"
        "slt $t0, $a2, $a3\n\t"
        "bnez $t0, .Leboot_001BC04C\n\t"
        "nop\n\t"
        ".Leboot_001BC0BC:\n\t"
        "b .Leboot_001BC0C8\n\t"
        "nop\n\t"
        ".Leboot_001BC0C4:\n\t"
        "or $a0, $s0, $zero\n\t"
        ".Leboot_001BC0C8:\n\t"
        "lui $a1, %%hi(sym_000EC78C)\n\t"
        "lw $a1, %%lo(sym_000EC78C)($a1)\n\t"
        "ori $s0, $zero, 0x0\n\t"
        "lh $a1, 0x278($a1)\n\t"
        "slt $a1, $s0, $a1\n\t"
        "beqz $a1, .Leboot_001BC1B0\n\t"
        "lui $a1, 0x3F00\n\t"
        "and $a2, $s5, $a1\n\t"
        "srl $s6, $a2, 8\n\t"
        "sll $a2, $s5, 8\n\t"
        "and $a1, $a0, $a1\n\t"
        "srl $s5, $a2, 8\n\t"
        "sll $a0, $a0, 8\n\t"
        "lui $a2, 0x100\n\t"
        "srl $s3, $a0, 8\n\t"
        "or $s5, $s5, $a2\n\t"
        "lui $a0, 0x200\n\t"
        "lw $a2, 0x14($sp)\n\t"
        "or $s3, $s3, $a0\n\t"
        "lui $s4, 0x1000\n\t"
        "lui $a0, %%hi(sym_001DB17C)\n\t"
        "or $s6, $s6, $s4\n\t"
        "srl $a1, $a1, 8\n\t"
        "sll $s2, $a2, 2\n\t"
        "addiu $a0, $a0, %%lo(sym_001DB17C)\n\t"
        "lui $fp, %%hi(sym_001DB31C)\n\t"
        "lui $s7, %%hi(D_120011DF)\n\t"
        "or $s4, $a1, $s4\n\t"
        "addu $s2, $s2, $a0\n\t"
        "addiu $fp, $fp, %%lo(sym_001DB31C)\n\t"
        "addiu $s7, $s7, %%lo(D_120011DF)\n\t"
        "lui $s1, %%hi(sym_001DB710)\n\t"
        ".Leboot_001BC148:\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "lui $s2, %%hi(sym_000EC78C)\n\t"
        "lw $a0, %%lo(sym_000EC78C)($s2)\n\t"
        "or $a1, $s0, $zero\n\t"
        "jal renderCommon_0580\n\t"
        "or $a2, $fp, $zero\n\t"
        "lw $a0, %%lo(sym_001DB710)($s1)\n\t"
        "sw $s7, 0x0($a0)\n\t"
        "sw $s6, 0x4($a0)\n\t"
        "sw $s5, 0x8($a0)\n\t"
        "sw $s4, 0xC($a0)\n\t"
        "sw $s3, 0x10($a0)\n\t"
        "lw $a1, 0x10($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "addiu $a3, $a0, 0x18\n\t"
        "lw $a2, 0x0($s2)\n\t"
        "addiu $s0, $s0, 0x1\n\t"
        "or $a1, $a2, $a1\n\t"
        "sw $a1, 0x14($a0)\n\t"
        "sw $a3, %%lo(sym_001DB710)($s1)\n\t"
        "lui $a0, %%hi(sym_000EC78C)\n\t"
        "lw $a0, %%lo(sym_000EC78C)($a0)\n\t"
        "lh $a0, 0x278($a0)\n\t"
        "slt $a0, $s0, $a0\n\t"
        "bnez $a0, .Leboot_001BC148\n\t"
        "nop\n\t"
        ".Leboot_001BC1B0:\n\t"
        "b .Leboot_001BC1C0\n\t"
        "nop\n\t"
        ".Leboot_001BC1B8:\n\t"
        "b .Leboot_001BC1C0\n\t"
        "nop\n\t"
        ".Leboot_001BC1C0:\n\t"
        "lw $s0, 0x1C($sp)\n\t"
        "lw $s1, 0x20($sp)\n\t"
        "lw $s2, 0x24($sp)\n\t"
        "lw $s3, 0x28($sp)\n\t"
        "lw $s4, 0x2C($sp)\n\t"
        "lw $s5, 0x30($sp)\n\t"
        "lw $s6, 0x34($sp)\n\t"
        "lw $s7, 0x38($sp)\n\t"
        "lw $fp, 0x3C($sp)\n\t"
        "lw $ra, 0x40($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x50\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

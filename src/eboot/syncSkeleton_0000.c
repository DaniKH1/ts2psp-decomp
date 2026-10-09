/**
 * The Sims 2 PSP - syncSkeleton_0000 (0x1B8A1C, 0x10C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x14($sp)
 *     or $s0, $a0, $zero
 *     swc1 $f20, 0x10($sp)
 *     sw $ra, 0x18($sp)
 *     jal func_0008E25C
 *     mtc1 $zero, $f20
 *     lw $a0, 0x10($v0)
 *     beqz $a0, .Leboot_001B8A64
 *     nop
 *     lw $a1, 0x58($a0)
 *     addiu $a1, $a1, 0x58
 *     lh $a2, 0x0($a1)
 *     lw $a1, 0x4($a1)
 *     jalr $a1
 *     addu $a0, $a0, $a2
 *     beq $v0, $s0, .Leboot_001B8AC0
 *     nop
 *   .Leboot_001B8A64
 *     lbu $a0, 0x58FC($s0)
 *     beql $a0, $zero, .Leboot_001B8A7C
 *     lw $a0, 0x128($s0)
 *     lui $a0, 0x3F00
 *     b .Leboot_001B8AC0
 *     mtc1 $a0, $f20
 *   .Leboot_001B8A7C
 *     lwc1 $f20, 0x130($s0)
 *     lwc1 $f12, 0x64($a0)
 *     lui $a0, 0x3E80
 *     mul.s $f20, $f12, $f20
 *     mtc1 $a0, $f12
 *     c.le.s $f20, $f12
 *     nop
 *     bc1tl .Leboot_001B8AA0
 *     mov.s $f20, $f12
 *   .Leboot_001B8AA0
 *     lwc1 $f12, 0x12C($s0)
 *     div.s $f12, $f12, $f20
 *     lui $a0, 0x41A0
 *     lw $a1, 0x60($s0)
 *     mtc1 $a0, $f13
 *     div.s $f12, $f12, $f13
 *     lwc1 $f20, 0x1C($a1)
 *     mul.s $f20, $f12, $f20
 *   .Leboot_001B8AC0
 *     jal func_0001E0B0
 *     or $a0, $s0, $zero
 *     beqz $v0, .Leboot_001B8AF4
 *     nop
 *     addiu $a0, $s0, 0x118
 *     jal syncSkeleton_1B38
 *     mov.s $f12, $f20
 *     lw $a0, 0x128($s0)
 *     lbu $a0, 0x5C($a0)
 *     bnez $a0, .Leboot_001B8AFC
 *     nop
 *     b .Leboot_001B8B14
 *     nop
 *   .Leboot_001B8AF4
 *     b .Leboot_001B8B14
 *     nop
 *   .Leboot_001B8AFC
 *     lw $a0, 0x8C($s0)
 *     addiu $a0, $a0, 0xC0
 *     lh $a1, 0x0($a0)
 *     lw $a2, 0x4($a0)
 *     jalr $a2
 *     addu $a0, $s0, $a1
 *   .Leboot_001B8B14
 *     lwc1 $f20, 0x10($sp)
 *     lw $s0, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * syncSkeleton: the entry point of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_0000(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x14($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "swc1 $f20, 0x10($sp)\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "jal func_0008E25C\n\t"
        "mtc1 $zero, $f20\n\t"
        "lw $a0, 0x10($v0)\n\t"
        "beqz $a0, .Leboot_001B8A64\n\t"
        "nop\n\t"
        "lw $a1, 0x58($a0)\n\t"
        "addiu $a1, $a1, 0x58\n\t"
        "lh $a2, 0x0($a1)\n\t"
        "lw $a1, 0x4($a1)\n\t"
        "jalr $a1\n\t"
        "addu $a0, $a0, $a2\n\t"
        "beq $v0, $s0, .Leboot_001B8AC0\n\t"
        "nop\n\t"
        ".Leboot_001B8A64:\n\t"
        "lbu $a0, 0x58FC($s0)\n\t"
        "beql $a0, $zero, .Leboot_001B8A7C\n\t"
        "lw $a0, 0x128($s0)\n\t"
        "lui $a0, 0x3F00\n\t"
        "b .Leboot_001B8AC0\n\t"
        "mtc1 $a0, $f20\n\t"
        ".Leboot_001B8A7C:\n\t"
        "lwc1 $f20, 0x130($s0)\n\t"
        "lwc1 $f12, 0x64($a0)\n\t"
        "lui $a0, 0x3E80\n\t"
        "mul.s $f20, $f12, $f20\n\t"
        "mtc1 $a0, $f12\n\t"
        "c.le.s $f20, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_001B8AA0\n\t"
        "mov.s $f20, $f12\n\t"
        ".Leboot_001B8AA0:\n\t"
        "lwc1 $f12, 0x12C($s0)\n\t"
        "div.s $f12, $f12, $f20\n\t"
        "lui $a0, 0x41A0\n\t"
        "lw $a1, 0x60($s0)\n\t"
        "mtc1 $a0, $f13\n\t"
        "div.s $f12, $f12, $f13\n\t"
        "lwc1 $f20, 0x1C($a1)\n\t"
        "mul.s $f20, $f12, $f20\n\t"
        ".Leboot_001B8AC0:\n\t"
        "jal func_0001E0B0\n\t"
        "or $a0, $s0, $zero\n\t"
        "beqz $v0, .Leboot_001B8AF4\n\t"
        "nop\n\t"
        "addiu $a0, $s0, 0x118\n\t"
        "jal syncSkeleton_1B38\n\t"
        "mov.s $f12, $f20\n\t"
        "lw $a0, 0x128($s0)\n\t"
        "lbu $a0, 0x5C($a0)\n\t"
        "bnez $a0, .Leboot_001B8AFC\n\t"
        "nop\n\t"
        "b .Leboot_001B8B14\n\t"
        "nop\n\t"
        ".Leboot_001B8AF4:\n\t"
        "b .Leboot_001B8B14\n\t"
        "nop\n\t"
        ".Leboot_001B8AFC:\n\t"
        "lw $a0, 0x8C($s0)\n\t"
        "addiu $a0, $a0, 0xC0\n\t"
        "lh $a1, 0x0($a0)\n\t"
        "lw $a2, 0x4($a0)\n\t"
        "jalr $a2\n\t"
        "addu $a0, $s0, $a1\n\t"
        ".Leboot_001B8B14:\n\t"
        "lwc1 $f20, 0x10($sp)\n\t"
        "lw $s0, 0x14($sp)\n\t"
        "lw $ra, 0x18($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

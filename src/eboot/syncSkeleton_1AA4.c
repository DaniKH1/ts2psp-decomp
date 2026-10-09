/**
 * The Sims 2 PSP - syncSkeleton_1AA4 (0x1BA4C0, 0x94 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw $a1, 0x128($a0)
 *     lwc1 $f13, 0x130($a0)
 *     lwc1 $f12, 0x64($a1)
 *     mul.s $f13, $f12, $f13
 *     lui $a1, 0x3E80
 *     sw $s0, 0x10($sp)
 *     mtc1 $a1, $f12
 *     or $s0, $a0, $zero
 *     c.le.s $f13, $f12
 *     sw $ra, 0x14($sp)
 *     bc1tl .Leboot_001BA4F4
 *     mov.s $f13, $f12
 *   .Leboot_001BA4F4
 *     lwc1 $f12, 0x12C($s0)
 *     div.s $f12, $f12, $f13
 *     lw $a0, 0x60($s0)
 *     lui $a1, 0x41F0
 *     lwc1 $f14, 0x20($a0)
 *     mtc1 $a1, $f15
 *     mul.s $f14, $f14, $f15
 *     div.s $f12, $f12, $f14
 *     jal syncSkeleton_1B38
 *     addiu $a0, $s0, 0x118
 *     lw $a0, 0x128($s0)
 *     lbu $a0, 0x5C($a0)
 *     beqz $a0, .Leboot_001BA544
 *     nop
 *     lw $a0, 0x8C($s0)
 *     addiu $a0, $a0, 0xC0
 *     lh $a1, 0x0($a0)
 *     lw $a2, 0x4($a0)
 *     jalr $a2
 *     addu $a0, $s0, $a1
 *   .Leboot_001BA544
 *     lw $s0, 0x10($sp)
 *     lw $ra, 0x14($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_1AA4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw $a1, 0x128($a0)\n\t"
        "lwc1 $f13, 0x130($a0)\n\t"
        "lwc1 $f12, 0x64($a1)\n\t"
        "mul.s $f13, $f12, $f13\n\t"
        "lui $a1, 0x3E80\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "mtc1 $a1, $f12\n\t"
        "or $s0, $a0, $zero\n\t"
        "c.le.s $f13, $f12\n\t"
        "sw $ra, 0x14($sp)\n\t"
        "bc1tl .Leboot_001BA4F4\n\t"
        "mov.s $f13, $f12\n\t"
        ".Leboot_001BA4F4:\n\t"
        "lwc1 $f12, 0x12C($s0)\n\t"
        "div.s $f12, $f12, $f13\n\t"
        "lw $a0, 0x60($s0)\n\t"
        "lui $a1, 0x41F0\n\t"
        "lwc1 $f14, 0x20($a0)\n\t"
        "mtc1 $a1, $f15\n\t"
        "mul.s $f14, $f14, $f15\n\t"
        "div.s $f12, $f12, $f14\n\t"
        "jal syncSkeleton_1B38\n\t"
        "addiu $a0, $s0, 0x118\n\t"
        "lw $a0, 0x128($s0)\n\t"
        "lbu $a0, 0x5C($a0)\n\t"
        "beqz $a0, .Leboot_001BA544\n\t"
        "nop\n\t"
        "lw $a0, 0x8C($s0)\n\t"
        "addiu $a0, $a0, 0xC0\n\t"
        "lh $a1, 0x0($a0)\n\t"
        "lw $a2, 0x4($a0)\n\t"
        "jalr $a2\n\t"
        "addu $a0, $s0, $a1\n\t"
        ".Leboot_001BA544:\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $ra, 0x14($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - syncSkeleton_1C20 (0x1BA63C, 0x144 bytes)
 *
 *     addiu $sp, $sp, -0x70
 *     sw $s0, 0x48($sp)
 *     sw $s1, 0x4C($sp)
 *     sw $s2, 0x50($sp)
 *     sw $s3, 0x54($sp)
 *     sw $s4, 0x58($sp)
 *     sw $s5, 0x5C($sp)
 *     sw $s6, 0x60($sp)
 *     sw $ra, 0x64($sp)
 *     or $s0, $a3, $zero
 *     or $s1, $a2, $zero
 *     or $s2, $a1, $zero
 *     or $s3, $a0, $zero
 *     cache 0x18, 0x0($s0)
 *     addiu $s4, $s2, 0x20
 *     addiu $s5, $sp, 0x10
 *     addiu $s6, $sp, 0x20
 *     addiu $a1, $s3, 0x20
 *     or $a0, $s6, $zero
 *     jal syncSkeleton_22B8
 *     or $a2, $s2, $zero
 *     addiu $a2, $s3, 0x10
 *     or $a0, $s5, $zero
 *     jal syncSkeleton_22B8
 *     or $a1, $s6, $zero
 *     or $a0, $s5, $zero
 *     or $a1, $s4, $zero
 *     jal syncSkeleton_2200
 *     or $a2, $s0, $zero
 *     lbu $a0, 0xB($s3)
 *     beqz $a0, .Leboot_001BA74C
 *     nop
 *     lwc1 $f13, 0x0($s1)
 *     mtc1 $zero, $f12
 *     c.eq.s $f13, $f12
 *     nop
 *     bc1t .Leboot_001BA74C
 *     nop
 *     lwc1 $f13, 0x4($s1)
 *     c.eq.s $f13, $f12
 *     nop
 *     bc1t .Leboot_001BA74C
 *     nop
 *     lwc1 $f13, 0x8($s1)
 *     c.eq.s $f13, $f12
 *     nop
 *     bc1t .Leboot_001BA74C
 *     lui $a0, 0x3F80
 *     mtc1 $a0, $f12
 *     swc1 $f12, 0x3C($sp)
 *     swc1 $f12, 0x40($sp)
 *     addiu $a0, $sp, 0x3C
 *     swc1 $f12, 0x44($sp)
 *     lwc1 $f12, 0x0($a0)
 *     lwc1 $f13, 0x0($s1)
 *     div.s $f12, $f12, $f13
 *     lwc1 $f14, 0x4($a0)
 *     lwc1 $f15, 0x4($s1)
 *     lwc1 $f16, 0x8($a0)
 *     lwc1 $f17, 0x8($s1)
 *     addiu $a1, $sp, 0x30
 *     or $a0, $s0, $zero
 *     div.s $f14, $f14, $f15
 *     swc1 $f12, 0x30($sp)
 *     div.s $f12, $f16, $f17
 *     swc1 $f14, 0x34($sp)
 *     jal syncSkeleton_2808
 *     swc1 $f12, 0x38($sp)
 *   .Leboot_001BA74C
 *     addiu $a1, $s2, 0x10
 *     jal syncSkeleton_27D0
 *     or $a0, $s0, $zero
 *     lw $s0, 0x48($sp)
 *     lw $s1, 0x4C($sp)
 *     lw $s2, 0x50($sp)
 *     lw $s3, 0x54($sp)
 *     lw $s4, 0x58($sp)
 *     lw $s5, 0x5C($sp)
 *     lw $s6, 0x60($sp)
 *     lw $ra, 0x64($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x70
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_1C20(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x70\n\t"
        "sw $s0, 0x48($sp)\n\t"
        "sw $s1, 0x4C($sp)\n\t"
        "sw $s2, 0x50($sp)\n\t"
        "sw $s3, 0x54($sp)\n\t"
        "sw $s4, 0x58($sp)\n\t"
        "sw $s5, 0x5C($sp)\n\t"
        "sw $s6, 0x60($sp)\n\t"
        "sw $ra, 0x64($sp)\n\t"
        "or $s0, $a3, $zero\n\t"
        "or $s1, $a2, $zero\n\t"
        "or $s2, $a1, $zero\n\t"
        "or $s3, $a0, $zero\n\t"
        "cache 0x18, 0x0($s0)\n\t"
        "addiu $s4, $s2, 0x20\n\t"
        "addiu $s5, $sp, 0x10\n\t"
        "addiu $s6, $sp, 0x20\n\t"
        "addiu $a1, $s3, 0x20\n\t"
        "or $a0, $s6, $zero\n\t"
        "jal syncSkeleton_22B8\n\t"
        "or $a2, $s2, $zero\n\t"
        "addiu $a2, $s3, 0x10\n\t"
        "or $a0, $s5, $zero\n\t"
        "jal syncSkeleton_22B8\n\t"
        "or $a1, $s6, $zero\n\t"
        "or $a0, $s5, $zero\n\t"
        "or $a1, $s4, $zero\n\t"
        "jal syncSkeleton_2200\n\t"
        "or $a2, $s0, $zero\n\t"
        "lbu $a0, 0xB($s3)\n\t"
        "beqz $a0, .Leboot_001BA74C\n\t"
        "nop\n\t"
        "lwc1 $f13, 0x0($s1)\n\t"
        "mtc1 $zero, $f12\n\t"
        "c.eq.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001BA74C\n\t"
        "nop\n\t"
        "lwc1 $f13, 0x4($s1)\n\t"
        "c.eq.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001BA74C\n\t"
        "nop\n\t"
        "lwc1 $f13, 0x8($s1)\n\t"
        "c.eq.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001BA74C\n\t"
        "lui $a0, 0x3F80\n\t"
        "mtc1 $a0, $f12\n\t"
        "swc1 $f12, 0x3C($sp)\n\t"
        "swc1 $f12, 0x40($sp)\n\t"
        "addiu $a0, $sp, 0x3C\n\t"
        "swc1 $f12, 0x44($sp)\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f13, 0x0($s1)\n\t"
        "div.s $f12, $f12, $f13\n\t"
        "lwc1 $f14, 0x4($a0)\n\t"
        "lwc1 $f15, 0x4($s1)\n\t"
        "lwc1 $f16, 0x8($a0)\n\t"
        "lwc1 $f17, 0x8($s1)\n\t"
        "addiu $a1, $sp, 0x30\n\t"
        "or $a0, $s0, $zero\n\t"
        "div.s $f14, $f14, $f15\n\t"
        "swc1 $f12, 0x30($sp)\n\t"
        "div.s $f12, $f16, $f17\n\t"
        "swc1 $f14, 0x34($sp)\n\t"
        "jal syncSkeleton_2808\n\t"
        "swc1 $f12, 0x38($sp)\n\t"
        ".Leboot_001BA74C:\n\t"
        "addiu $a1, $s2, 0x10\n\t"
        "jal syncSkeleton_27D0\n\t"
        "or $a0, $s0, $zero\n\t"
        "lw $s0, 0x48($sp)\n\t"
        "lw $s1, 0x4C($sp)\n\t"
        "lw $s2, 0x50($sp)\n\t"
        "lw $s3, 0x54($sp)\n\t"
        "lw $s4, 0x58($sp)\n\t"
        "lw $s5, 0x5C($sp)\n\t"
        "lw $s6, 0x60($sp)\n\t"
        "lw $ra, 0x64($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x70\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

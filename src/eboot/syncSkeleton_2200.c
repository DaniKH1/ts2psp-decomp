/**
 * The Sims 2 PSP - syncSkeleton_2200 (0x1BAC1C, 0x60 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $s0, 0x10($sp)
 *     sw $s1, 0x14($sp)
 *     or $s1, $a1, $zero
 *     or $s0, $a2, $zero
 *     sw $ra, 0x18($sp)
 *     jal syncSkeleton_21A0
 *     or $a1, $s0, $zero
 *     sw $zero, 0xC($s0)
 *     sw $zero, 0x1C($s0)
 *     sw $zero, 0x2C($s0)
 *     lwc1 $f12, 0x0($s1)
 *     swc1 $f12, 0x30($s0)
 *     lwc1 $f12, 0x4($s1)
 *     swc1 $f12, 0x34($s0)
 *     lui $a0, 0x3F80
 *     lwc1 $f12, 0x8($s1)
 *     sw $a0, 0x3C($s0)
 *     swc1 $f12, 0x38($s0)
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $ra, 0x18($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * syncSkeleton: wrapper that calls syncSkeleton_21A0 with this, the second and third arguments.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_2200(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "or $s1, $a1, $zero\n\t"
        "or $s0, $a2, $zero\n\t"
        "sw $ra, 0x18($sp)\n\t"
        "jal syncSkeleton_21A0\n\t"
        "or $a1, $s0, $zero\n\t"
        "sw $zero, 0xC($s0)\n\t"
        "sw $zero, 0x1C($s0)\n\t"
        "sw $zero, 0x2C($s0)\n\t"
        "lwc1 $f12, 0x0($s1)\n\t"
        "swc1 $f12, 0x30($s0)\n\t"
        "lwc1 $f12, 0x4($s1)\n\t"
        "swc1 $f12, 0x34($s0)\n\t"
        "lui $a0, 0x3F80\n\t"
        "lwc1 $f12, 0x8($s1)\n\t"
        "sw $a0, 0x3C($s0)\n\t"
        "swc1 $f12, 0x38($s0)\n\t"
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

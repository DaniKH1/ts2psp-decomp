/**
 * The Sims 2 PSP - syncSkeleton_0688 (0x1B90A4, 0x64 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s0, 0x10($sp)
 *     or $s0, $a0, $zero
 *     sw $s1, 0x14($sp)
 *     sw $s2, 0x18($sp)
 *     sw $s3, 0x1C($sp)
 *     sw $ra, 0x20($sp)
 *     ori $s3, $zero, 0x0
 *     addiu $s1, $s0, 0x88
 *     lui $s2, %%hi(sym_000E2480)
 *   .Leboot_001B90CC
 *     sw $s3, %%lo(sym_000E2480)($s2)
 *     or $a0, $s0, $zero
 *     jal syncSkeleton_0428
 *     or $a1, $s1, $zero
 *     addiu $s3, $s3, 0x1
 *     slti $a0, $s3, 0x4
 *     bnez $a0, .Leboot_001B90CC
 *     addiu $s1, $s1, 0x5C
 *     lw $s0, 0x10($sp)
 *     lw $s1, 0x14($sp)
 *     lw $s2, 0x18($sp)
 *     lw $s3, 0x1C($sp)
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_0688(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "sw $s1, 0x14($sp)\n\t"
        "sw $s2, 0x18($sp)\n\t"
        "sw $s3, 0x1C($sp)\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "ori $s3, $zero, 0x0\n\t"
        "addiu $s1, $s0, 0x88\n\t"
        "lui $s2, %%hi(sym_000E2480)\n\t"
        ".Leboot_001B90CC:\n\t"
        "sw $s3, %%lo(sym_000E2480)($s2)\n\t"
        "or $a0, $s0, $zero\n\t"
        "jal syncSkeleton_0428\n\t"
        "or $a1, $s1, $zero\n\t"
        "addiu $s3, $s3, 0x1\n\t"
        "slti $a0, $s3, 0x4\n\t"
        "bnez $a0, .Leboot_001B90CC\n\t"
        "addiu $s1, $s1, 0x5C\n\t"
        "lw $s0, 0x10($sp)\n\t"
        "lw $s1, 0x14($sp)\n\t"
        "lw $s2, 0x18($sp)\n\t"
        "lw $s3, 0x1C($sp)\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

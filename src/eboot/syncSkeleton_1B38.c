/**
 * The Sims 2 PSP - syncSkeleton_1B38 (0x1BA554, 0xE8 bytes)
 *
 *     addiu $sp, $sp, -0xB0
 *     swc1 $f20, 0x90($sp)
 *     sw $s0, 0x94($sp)
 *     mov.s $f20, $f12
 *     or $s0, $a0, $zero
 *     sw $s1, 0x98($sp)
 *     sw $s2, 0x9C($sp)
 *     sw $ra, 0xA0($sp)
 *     lw $a0, 0x0($s0)
 *     sw $s0, 0x10($sp)
 *     xor $a1, $a0, $s0
 *     sltu $a1, $zero, $a1
 *     andi $a1, $a1, 0xFF
 *     beqz $a1, .Leboot_001BA620
 *     sw $a0, 0x14($sp)
 *     lui $s2, %%hi(sym_00074208)
 *     addiu $s1, $sp, 0x20
 *     addiu $s2, $s2, %%lo(sym_00074208)
 *   .Leboot_001BA59C
 *     lw $a1, 0x230($a0)
 *     beqz $a1, .Leboot_001BA5DC
 *     nop
 *     sb $zero, 0x8C($sp)
 *     sb $zero, 0x8D($sp)
 *     lw $a1, 0x230($a0)
 *     lw $a2, 0x27C($a0)
 *     addiu $a0, $a1, 0x10
 *     or $a1, $a2, $zero
 *     jal updateNodeGraph_0000
 *     or $a2, $s1, $zero
 *     lbu $a1, 0x8D($sp)
 *     beqz $a1, .Leboot_001BA5D4
 *     lw $a0, 0x14($sp)
 *   .Leboot_001BA5D4
 *     b .Leboot_001BA600
 *     lw $a0, 0x0($a0)
 *   .Leboot_001BA5DC
 *     addiu $a0, $a0, 0x10
 *     mov.s $f12, $f20
 *     jal syncSkeleton_0FDC
 *     or $a1, $s2, $zero
 *     lw $a0, 0x14($sp)
 *     jal syncSkeleton_06EC
 *     addiu $a0, $a0, 0x10
 *     lw $a0, 0x14($sp)
 *     lw $a0, 0x0($a0)
 *   .Leboot_001BA600
 *     sw $s0, 0x10($sp)
 *     sw $a0, 0x14($sp)
 *     lw $a0, 0x14($sp)
 *     xor $a1, $a0, $s0
 *     sltu $a1, $zero, $a1
 *     andi $a1, $a1, 0xFF
 *     bnez $a1, .Leboot_001BA59C
 *     nop
 *   .Leboot_001BA620
 *     lwc1 $f20, 0x90($sp)
 *     lw $s0, 0x94($sp)
 *     lw $s1, 0x98($sp)
 *     lw $s2, 0x9C($sp)
 *     lw $ra, 0xA0($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0xB0
 *
 * syncSkeleton: one phase of the skeleton sync.
 */

#include "types.h"

__attribute__((noreturn)) void syncSkeleton_1B38(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0xB0\n\t"
        "swc1 $f20, 0x90($sp)\n\t"
        "sw $s0, 0x94($sp)\n\t"
        "mov.s $f20, $f12\n\t"
        "or $s0, $a0, $zero\n\t"
        "sw $s1, 0x98($sp)\n\t"
        "sw $s2, 0x9C($sp)\n\t"
        "sw $ra, 0xA0($sp)\n\t"
        "lw $a0, 0x0($s0)\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "xor $a1, $a0, $s0\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "beqz $a1, .Leboot_001BA620\n\t"
        "sw $a0, 0x14($sp)\n\t"
        "lui $s2, %%hi(sym_00074208)\n\t"
        "addiu $s1, $sp, 0x20\n\t"
        "addiu $s2, $s2, %%lo(sym_00074208)\n\t"
        ".Leboot_001BA59C:\n\t"
        "lw $a1, 0x230($a0)\n\t"
        "beqz $a1, .Leboot_001BA5DC\n\t"
        "nop\n\t"
        "sb $zero, 0x8C($sp)\n\t"
        "sb $zero, 0x8D($sp)\n\t"
        "lw $a1, 0x230($a0)\n\t"
        "lw $a2, 0x27C($a0)\n\t"
        "addiu $a0, $a1, 0x10\n\t"
        "or $a1, $a2, $zero\n\t"
        "jal updateNodeGraph_0000\n\t"
        "or $a2, $s1, $zero\n\t"
        "lbu $a1, 0x8D($sp)\n\t"
        "beqz $a1, .Leboot_001BA5D4\n\t"
        "lw $a0, 0x14($sp)\n\t"
        ".Leboot_001BA5D4:\n\t"
        "b .Leboot_001BA600\n\t"
        "lw $a0, 0x0($a0)\n\t"
        ".Leboot_001BA5DC:\n\t"
        "addiu $a0, $a0, 0x10\n\t"
        "mov.s $f12, $f20\n\t"
        "jal syncSkeleton_0FDC\n\t"
        "or $a1, $s2, $zero\n\t"
        "lw $a0, 0x14($sp)\n\t"
        "jal syncSkeleton_06EC\n\t"
        "addiu $a0, $a0, 0x10\n\t"
        "lw $a0, 0x14($sp)\n\t"
        "lw $a0, 0x0($a0)\n\t"
        ".Leboot_001BA600:\n\t"
        "sw $s0, 0x10($sp)\n\t"
        "sw $a0, 0x14($sp)\n\t"
        "lw $a0, 0x14($sp)\n\t"
        "xor $a1, $a0, $s0\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "andi $a1, $a1, 0xFF\n\t"
        "bnez $a1, .Leboot_001BA59C\n\t"
        "nop\n\t"
        ".Leboot_001BA620:\n\t"
        "lwc1 $f20, 0x90($sp)\n\t"
        "lw $s0, 0x94($sp)\n\t"
        "lw $s1, 0x98($sp)\n\t"
        "lw $s2, 0x9C($sp)\n\t"
        "lw $ra, 0xA0($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0xB0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

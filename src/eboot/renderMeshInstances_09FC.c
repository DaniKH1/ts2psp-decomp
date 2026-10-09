/**
 * The Sims 2 PSP - renderMeshInstances_09FC (0x1BE3FC, 0x120 bytes)
 *
 *     addiu $sp, $sp, -0x50
 *     swc1 $f20, 0x28($sp)
 *     swc1 $f22, 0x2C($sp)
 *     sw $s0, 0x30($sp)
 *     sw $s1, 0x34($sp)
 *     sw $s3, 0x3C($sp)
 *     sw $s4, 0x40($sp)
 *     mov.s $f22, $f12
 *     andi $s4, $a2, 0xFF
 *     mov.s $f20, $f13
 *     andi $s3, $a3, 0xFF
 *     or $s1, $a0, $zero
 *     or $s0, $a1, $zero
 *     sw $s2, 0x38($sp)
 *     sw $ra, 0x44($sp)
 *     ori $s2, $zero, 0x0
 *   .Leboot_001BE43C
 *     lw $a1, 0x0($s1)
 *     sw $s1, 0x20($sp)
 *     xor $a0, $a1, $s1
 *     sltu $a0, $zero, $a0
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BE4E4
 *     sw $a1, 0x24($sp)
 *   .Leboot_001BE458
 *     lw $a0, 0x220($a1)
 *     andi $a0, $a0, 0x2
 *     sltu $a0, $zero, $a0
 *     andi $a0, $a0, 0xFF
 *     beqz $a0, .Leboot_001BE484
 *     nop
 *     ori $a0, $zero, 0x2
 *     beql $s2, $zero, .Leboot_001BE47C
 *     ori $a0, $zero, 0x1
 *   .Leboot_001BE47C
 *     b .Leboot_001BE49C
 *     nop
 *   .Leboot_001BE484
 *     bnez $s2, .Leboot_001BE494
 *     nop
 *     b .Leboot_001BE49C
 *     ori $a0, $zero, 0x2
 *   .Leboot_001BE494
 *     b .Leboot_001BE4C4
 *     lw $a0, 0x0($a1)
 *   .Leboot_001BE49C
 *     or $t0, $a0, $zero
 *     mov.s $f12, $f22
 *     addiu $a0, $a1, 0x10
 *     mov.s $f13, $f20
 *     or $a1, $s0, $zero
 *     or $a2, $s4, $zero
 *     jal renderMeshInstances_0658
 *     or $a3, $s3, $zero
 *     lw $a1, 0x24($sp)
 *     lw $a0, 0x0($a1)
 *   .Leboot_001BE4C4
 *     sw $s1, 0x20($sp)
 *     sw $a0, 0x24($sp)
 *     lw $a1, 0x24($sp)
 *     xor $a0, $a1, $s1
 *     sltu $a0, $zero, $a0
 *     andi $a0, $a0, 0xFF
 *     bnez $a0, .Leboot_001BE458
 *     nop
 *   .Leboot_001BE4E4
 *     addiu $s2, $s2, 0x1
 *     slti $a0, $s2, 0x2
 *     bnez $a0, .Leboot_001BE43C
 *     nop
 *     lwc1 $f20, 0x28($sp)
 *     lwc1 $f22, 0x2C($sp)
 *     lw $s0, 0x30($sp)
 *     lw $s1, 0x34($sp)
 *     lw $s2, 0x38($sp)
 *     lw $s3, 0x3C($sp)
 *     lw $s4, 0x40($sp)
 *     lw $ra, 0x44($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x50
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_09FC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x50\n\t"
        "swc1 $f20, 0x28($sp)\n\t"
        "swc1 $f22, 0x2C($sp)\n\t"
        "sw $s0, 0x30($sp)\n\t"
        "sw $s1, 0x34($sp)\n\t"
        "sw $s3, 0x3C($sp)\n\t"
        "sw $s4, 0x40($sp)\n\t"
        "mov.s $f22, $f12\n\t"
        "andi $s4, $a2, 0xFF\n\t"
        "mov.s $f20, $f13\n\t"
        "andi $s3, $a3, 0xFF\n\t"
        "or $s1, $a0, $zero\n\t"
        "or $s0, $a1, $zero\n\t"
        "sw $s2, 0x38($sp)\n\t"
        "sw $ra, 0x44($sp)\n\t"
        "ori $s2, $zero, 0x0\n\t"
        ".Leboot_001BE43C:\n\t"
        "lw $a1, 0x0($s1)\n\t"
        "sw $s1, 0x20($sp)\n\t"
        "xor $a0, $a1, $s1\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE4E4\n\t"
        "sw $a1, 0x24($sp)\n\t"
        ".Leboot_001BE458:\n\t"
        "lw $a0, 0x220($a1)\n\t"
        "andi $a0, $a0, 0x2\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "beqz $a0, .Leboot_001BE484\n\t"
        "nop\n\t"
        "ori $a0, $zero, 0x2\n\t"
        "beql $s2, $zero, .Leboot_001BE47C\n\t"
        "ori $a0, $zero, 0x1\n\t"
        ".Leboot_001BE47C:\n\t"
        "b .Leboot_001BE49C\n\t"
        "nop\n\t"
        ".Leboot_001BE484:\n\t"
        "bnez $s2, .Leboot_001BE494\n\t"
        "nop\n\t"
        "b .Leboot_001BE49C\n\t"
        "ori $a0, $zero, 0x2\n\t"
        ".Leboot_001BE494:\n\t"
        "b .Leboot_001BE4C4\n\t"
        "lw $a0, 0x0($a1)\n\t"
        ".Leboot_001BE49C:\n\t"
        "or $t0, $a0, $zero\n\t"
        "mov.s $f12, $f22\n\t"
        "addiu $a0, $a1, 0x10\n\t"
        "mov.s $f13, $f20\n\t"
        "or $a1, $s0, $zero\n\t"
        "or $a2, $s4, $zero\n\t"
        "jal renderMeshInstances_0658\n\t"
        "or $a3, $s3, $zero\n\t"
        "lw $a1, 0x24($sp)\n\t"
        "lw $a0, 0x0($a1)\n\t"
        ".Leboot_001BE4C4:\n\t"
        "sw $s1, 0x20($sp)\n\t"
        "sw $a0, 0x24($sp)\n\t"
        "lw $a1, 0x24($sp)\n\t"
        "xor $a0, $a1, $s1\n\t"
        "sltu $a0, $zero, $a0\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "bnez $a0, .Leboot_001BE458\n\t"
        "nop\n\t"
        ".Leboot_001BE4E4:\n\t"
        "addiu $s2, $s2, 0x1\n\t"
        "slti $a0, $s2, 0x2\n\t"
        "bnez $a0, .Leboot_001BE43C\n\t"
        "nop\n\t"
        "lwc1 $f20, 0x28($sp)\n\t"
        "lwc1 $f22, 0x2C($sp)\n\t"
        "lw $s0, 0x30($sp)\n\t"
        "lw $s1, 0x34($sp)\n\t"
        "lw $s2, 0x38($sp)\n\t"
        "lw $s3, 0x3C($sp)\n\t"
        "lw $s4, 0x40($sp)\n\t"
        "lw $ra, 0x44($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x50\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

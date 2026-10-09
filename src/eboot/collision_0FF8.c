/**
 * The Sims 2 PSP - collision_0FF8 (0x1B1818, 0x218 bytes)
 *
 *     addiu $sp, $sp, -0x40
 *     lw $t1, 0x1C($a3)
 *     lw $t0, 0x18($a3)
 *     lui $t2, %%hi(sym_001DE6B0)
 *     sw $s5, 0x28($sp)
 *     lw $s5, %%lo(sym_001DE6B4)($t2)
 *     sw $s4, 0x24($sp)
 *     lw $s4, %%lo(sym_001DE6B0)($t2)
 *     xor $t2, $t1, $s5
 *     sltiu $t2, $t2, 0x1
 *     sltu $t0, $t0, $s4
 *     slt $t3, $t1, $s5
 *     and $t0, $t2, $t0
 *     or $t0, $t0, $t3
 *     sw $s0, 0x14($sp)
 *     sw $s1, 0x18($sp)
 *     sw $s2, 0x1C($sp)
 *     or $s0, $a0, $zero
 *     andi $t0, $t0, 0xFF
 *     or $s1, $a1, $zero
 *     or $s2, $a2, $zero
 *     sw $s3, 0x20($sp)
 *     sw $s6, 0x2C($sp)
 *     sw $ra, 0x30($sp)
 *     bnez $t0, .Leboot_001B193C
 *     or $a0, $a3, $zero
 *   .Leboot_001B1880
 *     lw $s3, 0x28($a0)
 *     lw $s6, 0x2C($a0)
 *     or $a0, $s1, $zero
 *     jal collision_1430
 *     or $a1, $s6, $zero
 *     beqz $v0, .Leboot_001B18F0
 *     nop
 *     or $a0, $s1, $zero
 *     jal collision_1430
 *     or $a1, $s3, $zero
 *     beqz $v0, .Leboot_001B18C8
 *     or $a0, $s6, $zero
 *     or $a0, $s0, $zero
 *     or $a1, $s1, $zero
 *     or $a2, $s2, $zero
 *     jal collision_0FF8
 *     or $a3, $s3, $zero
 *     or $a0, $s6, $zero
 *   .Leboot_001B18C8
 *     lw $a3, 0x1C($a0)
 *     lw $a2, 0x18($a0)
 *     xor $a1, $a3, $s5
 *     sltiu $a1, $a1, 0x1
 *     sltu $a2, $a2, $s4
 *     slt $t0, $a3, $s5
 *     and $a1, $a1, $a2
 *     or $s6, $a1, $t0
 *     b .Leboot_001B1934
 *     andi $s6, $s6, 0xFF
 *   .Leboot_001B18F0
 *     or $a0, $s1, $zero
 *     jal collision_1430
 *     or $a1, $s3, $zero
 *     beqz $v0, .Leboot_001B192C
 *     or $a0, $s3, $zero
 *     lw $a3, 0x1C($a0)
 *     lw $a2, 0x18($a0)
 *     xor $a1, $a3, $s5
 *     sltiu $a1, $a1, 0x1
 *     sltu $a2, $a2, $s4
 *     slt $t0, $a3, $s5
 *     and $a1, $a1, $a2
 *     or $s6, $a1, $t0
 *     b .Leboot_001B1934
 *     andi $s6, $s6, 0xFF
 *   .Leboot_001B192C
 *     b .Leboot_001B1A08
 *     nop
 *   .Leboot_001B1934
 *     beqz $s6, .Leboot_001B1880
 *     nop
 *   .Leboot_001B193C
 *     addiu $a1, $s0, 0x8
 *     beq $a0, $a1, .Leboot_001B196C
 *     nop
 *     lw $a1, 0xC($s2)
 *     or $s0, $a0, $zero
 *     addiu $s1, $a1, 0x1
 *     sltu $a2, $s1, $a1
 *     sll $s3, $s1, 2
 *     bnez $a2, .Leboot_001B1974
 *     sll $a0, $s1, 2
 *     b .Leboot_001B19B0
 *     nop
 *   .Leboot_001B196C
 *     b .Leboot_001B1A08
 *     nop
 *   .Leboot_001B1974
 *     lw $a2, 0x4($s2)
 *     or $a3, $a1, $zero
 *     addu $a1, $a2, $s3
 *     sll $s3, $a3, 2
 *     addu $s3, $a2, $s3
 *     beq $s3, $a1, .Leboot_001B199C
 *     nop
 *     addiu $s3, $s3, -0x4
 *   .Leboot_001B1994
 *     bne $s3, $a1, .Leboot_001B1994
 *     addiu $s3, $s3, -0x4
 *   .Leboot_001B199C
 *     or $a1, $a0, $zero
 *     jal func_0012C914
 *     or $a0, $s2, $zero
 *     b .Leboot_001B1A08
 *     sw $s1, 0xC($s2)
 *   .Leboot_001B19B0
 *     or $a1, $a0, $zero
 *     jal func_0012C914
 *     or $a0, $s2, $zero
 *     srl $a2, $v0, 2
 *     lw $a0, 0xC($s2)
 *     sltu $a3, $s1, $a2
 *     lw $a1, 0x4($s2)
 *     bnel $a3, $zero, .Leboot_001B19D4
 *     or $a2, $s1, $zero
 *   .Leboot_001B19D4
 *     sll $a0, $a0, 2
 *     addu $a3, $a1, $a0
 *     addu $a0, $a1, $s3
 *     or $a1, $a3, $zero
 *     beq $a1, $a0, .Leboot_001B1A04
 *     nop
 *   .Leboot_001B19EC
 *     or $a3, $a1, $zero
 *     bnel $a3, $zero, .Leboot_001B19F8
 *     sw $s0, 0x0($a3)
 *   .Leboot_001B19F8
 *     addiu $a1, $a1, 0x4
 *     bne $a1, $a0, .Leboot_001B19EC
 *     nop
 *   .Leboot_001B1A04
 *     sw $a2, 0xC($s2)
 *   .Leboot_001B1A08
 *     lw $s0, 0x14($sp)
 *     lw $s1, 0x18($sp)
 *     lw $s2, 0x1C($sp)
 *     lw $s3, 0x20($sp)
 *     lw $s4, 0x24($sp)
 *     lw $s5, 0x28($sp)
 *     lw $s6, 0x2C($sp)
 *     lw $ra, 0x30($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x40
 *
 * collision: one phase of the collision pipeline.
 */

#include "types.h"

__attribute__((noreturn)) void collision_0FF8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "lw $t1, 0x1C($a3)\n\t"
        "lw $t0, 0x18($a3)\n\t"
        "lui $t2, %%hi(sym_001DE6B0)\n\t"
        "sw $s5, 0x28($sp)\n\t"
        "lw $s5, %%lo(sym_001DE6B4)($t2)\n\t"
        "sw $s4, 0x24($sp)\n\t"
        "lw $s4, %%lo(sym_001DE6B0)($t2)\n\t"
        "xor $t2, $t1, $s5\n\t"
        "sltiu $t2, $t2, 0x1\n\t"
        "sltu $t0, $t0, $s4\n\t"
        "slt $t3, $t1, $s5\n\t"
        "and $t0, $t2, $t0\n\t"
        "or $t0, $t0, $t3\n\t"
        "sw $s0, 0x14($sp)\n\t"
        "sw $s1, 0x18($sp)\n\t"
        "sw $s2, 0x1C($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "andi $t0, $t0, 0xFF\n\t"
        "or $s1, $a1, $zero\n\t"
        "or $s2, $a2, $zero\n\t"
        "sw $s3, 0x20($sp)\n\t"
        "sw $s6, 0x2C($sp)\n\t"
        "sw $ra, 0x30($sp)\n\t"
        "bnez $t0, .Leboot_001B193C\n\t"
        "or $a0, $a3, $zero\n\t"
        ".Leboot_001B1880:\n\t"
        "lw $s3, 0x28($a0)\n\t"
        "lw $s6, 0x2C($a0)\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal collision_1430\n\t"
        "or $a1, $s6, $zero\n\t"
        "beqz $v0, .Leboot_001B18F0\n\t"
        "nop\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal collision_1430\n\t"
        "or $a1, $s3, $zero\n\t"
        "beqz $v0, .Leboot_001B18C8\n\t"
        "or $a0, $s6, $zero\n\t"
        "or $a0, $s0, $zero\n\t"
        "or $a1, $s1, $zero\n\t"
        "or $a2, $s2, $zero\n\t"
        "jal collision_0FF8\n\t"
        "or $a3, $s3, $zero\n\t"
        "or $a0, $s6, $zero\n\t"
        ".Leboot_001B18C8:\n\t"
        "lw $a3, 0x1C($a0)\n\t"
        "lw $a2, 0x18($a0)\n\t"
        "xor $a1, $a3, $s5\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "sltu $a2, $a2, $s4\n\t"
        "slt $t0, $a3, $s5\n\t"
        "and $a1, $a1, $a2\n\t"
        "or $s6, $a1, $t0\n\t"
        "b .Leboot_001B1934\n\t"
        "andi $s6, $s6, 0xFF\n\t"
        ".Leboot_001B18F0:\n\t"
        "or $a0, $s1, $zero\n\t"
        "jal collision_1430\n\t"
        "or $a1, $s3, $zero\n\t"
        "beqz $v0, .Leboot_001B192C\n\t"
        "or $a0, $s3, $zero\n\t"
        "lw $a3, 0x1C($a0)\n\t"
        "lw $a2, 0x18($a0)\n\t"
        "xor $a1, $a3, $s5\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "sltu $a2, $a2, $s4\n\t"
        "slt $t0, $a3, $s5\n\t"
        "and $a1, $a1, $a2\n\t"
        "or $s6, $a1, $t0\n\t"
        "b .Leboot_001B1934\n\t"
        "andi $s6, $s6, 0xFF\n\t"
        ".Leboot_001B192C:\n\t"
        "b .Leboot_001B1A08\n\t"
        "nop\n\t"
        ".Leboot_001B1934:\n\t"
        "beqz $s6, .Leboot_001B1880\n\t"
        "nop\n\t"
        ".Leboot_001B193C:\n\t"
        "addiu $a1, $s0, 0x8\n\t"
        "beq $a0, $a1, .Leboot_001B196C\n\t"
        "nop\n\t"
        "lw $a1, 0xC($s2)\n\t"
        "or $s0, $a0, $zero\n\t"
        "addiu $s1, $a1, 0x1\n\t"
        "sltu $a2, $s1, $a1\n\t"
        "sll $s3, $s1, 2\n\t"
        "bnez $a2, .Leboot_001B1974\n\t"
        "sll $a0, $s1, 2\n\t"
        "b .Leboot_001B19B0\n\t"
        "nop\n\t"
        ".Leboot_001B196C:\n\t"
        "b .Leboot_001B1A08\n\t"
        "nop\n\t"
        ".Leboot_001B1974:\n\t"
        "lw $a2, 0x4($s2)\n\t"
        "or $a3, $a1, $zero\n\t"
        "addu $a1, $a2, $s3\n\t"
        "sll $s3, $a3, 2\n\t"
        "addu $s3, $a2, $s3\n\t"
        "beq $s3, $a1, .Leboot_001B199C\n\t"
        "nop\n\t"
        "addiu $s3, $s3, -0x4\n\t"
        ".Leboot_001B1994:\n\t"
        "bne $s3, $a1, .Leboot_001B1994\n\t"
        "addiu $s3, $s3, -0x4\n\t"
        ".Leboot_001B199C:\n\t"
        "or $a1, $a0, $zero\n\t"
        "jal func_0012C914\n\t"
        "or $a0, $s2, $zero\n\t"
        "b .Leboot_001B1A08\n\t"
        "sw $s1, 0xC($s2)\n\t"
        ".Leboot_001B19B0:\n\t"
        "or $a1, $a0, $zero\n\t"
        "jal func_0012C914\n\t"
        "or $a0, $s2, $zero\n\t"
        "srl $a2, $v0, 2\n\t"
        "lw $a0, 0xC($s2)\n\t"
        "sltu $a3, $s1, $a2\n\t"
        "lw $a1, 0x4($s2)\n\t"
        "bnel $a3, $zero, .Leboot_001B19D4\n\t"
        "or $a2, $s1, $zero\n\t"
        ".Leboot_001B19D4:\n\t"
        "sll $a0, $a0, 2\n\t"
        "addu $a3, $a1, $a0\n\t"
        "addu $a0, $a1, $s3\n\t"
        "or $a1, $a3, $zero\n\t"
        "beq $a1, $a0, .Leboot_001B1A04\n\t"
        "nop\n\t"
        ".Leboot_001B19EC:\n\t"
        "or $a3, $a1, $zero\n\t"
        "bnel $a3, $zero, .Leboot_001B19F8\n\t"
        "sw $s0, 0x0($a3)\n\t"
        ".Leboot_001B19F8:\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "bne $a1, $a0, .Leboot_001B19EC\n\t"
        "nop\n\t"
        ".Leboot_001B1A04:\n\t"
        "sw $a2, 0xC($s2)\n\t"
        ".Leboot_001B1A08:\n\t"
        "lw $s0, 0x14($sp)\n\t"
        "lw $s1, 0x18($sp)\n\t"
        "lw $s2, 0x1C($sp)\n\t"
        "lw $s3, 0x20($sp)\n\t"
        "lw $s4, 0x24($sp)\n\t"
        "lw $s5, 0x28($sp)\n\t"
        "lw $s6, 0x2C($sp)\n\t"
        "lw $ra, 0x30($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

/**
 * The Sims 2 PSP - updateNodeGraph_08C4 (0x1B6144, 0xBC bytes)
 *
 *     lw $a2, 0x8($a0)
 *     sll $t0, $a1, 2
 *     addu $a2, $a2, $t0
 *     lwc1 $f13, 0x0($a2)
 *     c.eq.s $f13, $f12
 *     nop
 *     bc1t .Leboot_001B61F8
 *     nop
 *     lw $a2, 0x8($a0)
 *     sltiu $t1, $a1, 0x1E
 *     addu $a2, $a2, $t0
 *     swc1 $f12, 0x0($a2)
 *     lw $a3, 0xC($a0)
 *     ori $t0, $zero, 0x1
 *     beqz $t1, .Leboot_001B61C0
 *     lw $a2, 0x4($a0)
 *     sllv $a1, $t0, $a1
 *     or $a1, $a3, $a1
 *     lw $a3, 0x0($a0)
 *     sw $a1, 0xC($a0)
 *     lw $a0, 0x38($a3)
 *     sra $a1, $a2, 5
 *     sll $a1, $a1, 2
 *     addu $a0, $a0, $a1
 *     lw $a1, 0x0($a0)
 *     andi $a2, $a2, 0x1F
 *     sllv $a2, $t0, $a2
 *     or $a1, $a1, $a2
 *     sw $a1, 0x0($a0)
 *     b .Leboot_001B61F8
 *     sb $zero, 0x34($a3)
 *   .Leboot_001B61C0
 *     lui $a1, 0x8000
 *     or $a1, $a3, $a1
 *     lw $a3, 0x0($a0)
 *     sw $a1, 0xC($a0)
 *     lw $a0, 0x38($a3)
 *     sra $a1, $a2, 5
 *     sll $a1, $a1, 2
 *     addu $a0, $a0, $a1
 *     lw $a1, 0x0($a0)
 *     andi $a2, $a2, 0x1F
 *     sllv $a2, $t0, $a2
 *     or $a1, $a1, $a2
 *     sw $a1, 0x0($a0)
 *     sb $zero, 0x34($a3)
 *   .Leboot_001B61F8
 *     jr $ra
 *     nop
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_08C4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $a2, 0x8($a0)\n\t"
        "sll $t0, $a1, 2\n\t"
        "addu $a2, $a2, $t0\n\t"
        "lwc1 $f13, 0x0($a2)\n\t"
        "c.eq.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t .Leboot_001B61F8\n\t"
        "nop\n\t"
        "lw $a2, 0x8($a0)\n\t"
        "sltiu $t1, $a1, 0x1E\n\t"
        "addu $a2, $a2, $t0\n\t"
        "swc1 $f12, 0x0($a2)\n\t"
        "lw $a3, 0xC($a0)\n\t"
        "ori $t0, $zero, 0x1\n\t"
        "beqz $t1, .Leboot_001B61C0\n\t"
        "lw $a2, 0x4($a0)\n\t"
        "sllv $a1, $t0, $a1\n\t"
        "or $a1, $a3, $a1\n\t"
        "lw $a3, 0x0($a0)\n\t"
        "sw $a1, 0xC($a0)\n\t"
        "lw $a0, 0x38($a3)\n\t"
        "sra $a1, $a2, 5\n\t"
        "sll $a1, $a1, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        "lw $a1, 0x0($a0)\n\t"
        "andi $a2, $a2, 0x1F\n\t"
        "sllv $a2, $t0, $a2\n\t"
        "or $a1, $a1, $a2\n\t"
        "sw $a1, 0x0($a0)\n\t"
        "b .Leboot_001B61F8\n\t"
        "sb $zero, 0x34($a3)\n\t"
        ".Leboot_001B61C0:\n\t"
        "lui $a1, 0x8000\n\t"
        "or $a1, $a3, $a1\n\t"
        "lw $a3, 0x0($a0)\n\t"
        "sw $a1, 0xC($a0)\n\t"
        "lw $a0, 0x38($a3)\n\t"
        "sra $a1, $a2, 5\n\t"
        "sll $a1, $a1, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        "lw $a1, 0x0($a0)\n\t"
        "andi $a2, $a2, 0x1F\n\t"
        "sllv $a2, $t0, $a2\n\t"
        "or $a1, $a1, $a2\n\t"
        "sw $a1, 0x0($a0)\n\t"
        "sb $zero, 0x34($a3)\n\t"
        ".Leboot_001B61F8:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

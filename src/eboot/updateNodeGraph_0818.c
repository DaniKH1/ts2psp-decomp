/**
 * The Sims 2 PSP - updateNodeGraph_0818 (0x1B6098, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw $ra, 0x10($sp)
 *     jal func_000AA9A8
 *     nop
 *     or $a0, $v0, $zero
 *     ori $v0, $zero, 0x0
 *     bnel $a0, $zero, .Leboot_001B60B8
 *     addiu $v0, $a0, 0x10
 *   .Leboot_001B60B8
 *     lw $ra, 0x10($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x20
 *
 * updateNodeGraph: call func_000AA9A8, and if it answered, take the graph pointer it produced.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0818(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw $ra, 0x10($sp)\n\t"
        "jal func_000AA9A8\n\t"
        "nop\n\t"
        "or $a0, $v0, $zero\n\t"
        "ori $v0, $zero, 0x0\n\t"
        "bnel $a0, $zero, .Leboot_001B60B8\n\t"
        "addiu $v0, $a0, 0x10\n\t"
        ".Leboot_001B60B8:\n\t"
        "lw $ra, 0x10($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

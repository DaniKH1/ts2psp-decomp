/**
 * The Sims 2 PSP - updateNodeGraph_0D74 (0x1B65F4, 0x30 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     lw $a3, 0x8($a0)
 *     or $t0, $a1, $zero
 *     addiu $a1, $a3, 0x4
 *     addiu $a2, $a3, 0x10
 *     lw $a0, 0x14($a0)
 *     sw $ra, 0x20($sp)
 *     jal func_00105474
 *     addiu $a3, $a3, 0x1C
 *     lw $ra, 0x20($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * updateNodeGraph: one phase of the node-graph update.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0D74(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "lw $a3, 0x8($a0)\n\t"
        "or $t0, $a1, $zero\n\t"
        "addiu $a1, $a3, 0x4\n\t"
        "addiu $a2, $a3, 0x10\n\t"
        "lw $a0, 0x14($a0)\n\t"
        "sw $ra, 0x20($sp)\n\t"
        "jal func_00105474\n\t"
        "addiu $a3, $a3, 0x1C\n\t"
        "lw $ra, 0x20($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

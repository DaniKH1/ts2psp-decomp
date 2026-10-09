/**
 * The Sims 2 PSP - func_000804B8 (0x0804B8, 0x10 bytes)
 *
 *     lw $a0, 0x4($a0)
 *     andi $v0, $a0, 0x8
 *     jr $ra
 *     sltu $v0, $zero, $v0
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_000804B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $a0, 0x4($a0)\n\t"
        "andi $v0, $a0, 0x8\n\t"
        "jr $ra\n\t"
        "sltu $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

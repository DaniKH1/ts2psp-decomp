/**
 * The Sims 2 PSP - func_00080584 (0x080584, 0x8 bytes)
 *
 *     jr $ra
 *     lw $v0, 0x18($a0)
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_00080584(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "lw $v0, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

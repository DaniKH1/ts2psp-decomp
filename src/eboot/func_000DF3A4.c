/**
 * The Sims 2 PSP - func_000DF3A4 (0x0DF3A4, 0x8 bytes)
 *
 *     jr $ra
 *     or $v0, $zero, $zero
 *
 * sortAndCullScene: one phase of the sort-and-cull pass.
 */

#include "types.h"

__attribute__((noreturn)) void func_000DF3A4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "or $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}

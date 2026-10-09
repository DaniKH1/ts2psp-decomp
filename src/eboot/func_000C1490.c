/**
 * The Sims 2 PSP - func_000C1490 (0x000C1490, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * The twelfth of the thirteen identical +0x24 getters; see
 * func_000BD5F4.  It is the first after the gap, 0x24 bytes past
 * func_000BEE9C - the first gap in the run, which is where a larger
 * function was emitted between two getters of the same class.
 */
#include "types.h"

__attribute__((noreturn)) void func_000C1490(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
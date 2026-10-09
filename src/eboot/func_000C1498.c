/**
 * The Sims 2 PSP - func_000C1498 (0x000C1498, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * The last of the thirteen identical +0x24 getters; see func_000BD5F4.
 * It is the only one immediately adjacent to another member of the
 * group (func_000C1490 is 8 bytes before it).
 */
#include "types.h"

__attribute__((noreturn)) void func_000C1498(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
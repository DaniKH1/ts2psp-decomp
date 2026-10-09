/**
 * The Sims 2 PSP - func_000BDDE4 (0x000BDDE4, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * The sixth of the thirteen identical +0x24 getters; see func_000BD5F4.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDDE4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
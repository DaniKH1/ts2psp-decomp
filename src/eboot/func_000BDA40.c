/**
 * The Sims 2 PSP - func_000BDA40 (0x000BDA40, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * The fourth of the thirteen identical +0x24 getters; see func_000BD5F4.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDA40(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
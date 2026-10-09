/**
 * The Sims 2 PSP - func_000BEE9C (0x000BEE9C, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * The eleventh of the thirteen identical +0x24 getters; see
 * func_000BD5F4.  This one is the last of the consecutive run that
 * starts at 0x000BD5F4 - the next member is 0x000C1490, after the gap.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BEE9C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
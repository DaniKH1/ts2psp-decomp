/**
 * The Sims 2 PSP - func_0000F574 (0x0000F574, 0x8 bytes)
 *
 *     jr    $ra
 *     ori   $v0, $zero, 0x10
 *
 * Returns 0x10 (16) in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000F574(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
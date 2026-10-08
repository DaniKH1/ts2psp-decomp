/**
 * The Sims 2 PSP - func_00005AE8 (0x00005AE8, 0x8 bytes)
 *
 *     jr    $ra
 *     ori   $v0, $zero, 0x3
 *
 * Returns 3 in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_00005AE8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x3\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
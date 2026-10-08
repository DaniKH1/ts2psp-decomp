/**
 * The Sims 2 PSP - func_0006B6B8 (0x0006B6B8, 0x0C bytes)
 *
 * Loads the address of a global at 0x0E2240 and returns it.
 *
 *     lui  $v0, 0x0E
 *     jr   $ra
 *     addiu $v0, $v0, 0x2240
 *
 * **Returns a global address.**  The delay slot does the low part of
 * the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0006B6B8(void) {
    __asm__ __volatile__(
        "lui  $v0, 0x0E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x2240\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
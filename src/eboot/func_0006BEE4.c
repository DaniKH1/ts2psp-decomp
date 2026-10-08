/**
 * The Sims 2 PSP - func_0006BEE4 (0x0006BEE4, 0x0C bytes)
 *
 * Loads the address of a global at 0x0E2260 and returns it.
 *
 *     lui  $v0, 0x0E
 *     jr   $ra
 *     addiu $v0, $v0, 0x2260
 *
 * **Returns a global address.**  Same pattern as func_0006B6B8,
 * different global.
 */
#include "types.h"

/* 0x0E2260 - global address */

__attribute__((noreturn)) void *func_0006BEE4(void) {
    __asm__ __volatile__(
        "lui  $v0, 0x0E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x2260\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
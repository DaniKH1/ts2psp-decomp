/**
 * The Sims 2 PSP - func_00082180 (0x00082180, 0x0C bytes)
 *
 * Loads the address of global 0x0E2340 and returns it.
 *
 *     lui  $v0, 0x0E
 *     jr   $ra
 *     addiu $v0, $v0, 0x2340
 *
 * **Returns a global address.**  Same pattern as func_0006B6B8,
 * different global (0x0E2340).
 */
#include "types.h"

/* 0x0E2340 - global address */

__attribute__((noreturn)) void *func_00082180(void) {
    __asm__ __volatile__(
        "lui  $v0, 0x0E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x2340\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
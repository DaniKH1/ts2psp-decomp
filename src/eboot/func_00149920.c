/**
 * The Sims 2 PSP - func_00149920 (0x00149920, 0x0C bytes)
 *
 * Loads the address of global 0x1E24C4 and returns it.
 *
 *     lui  $v0, 0x1E
 *     jr   $ra
 *     addiu $v0, $v0, 0x24C4
 *
 * **Returns a global address.**  0x1E24C4 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00149920(void) {
    __asm__ __volatile__(
        "lui  $v0, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x24C4\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
/**
 * The Sims 2 PSP - func_00091534 (0x00091534, 0x0C bytes)
 *
 * Loads the address of global 0x0E23D4 and returns it.
 *
 *     lui  $v0, %hi(sym_000E23D4)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_000E23D4)
 *
 * **Returns a global address.**  0x0E23D4 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00091534(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_000E23D4)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_000E23D4)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
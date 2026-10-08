/**
 * The Sims 2 PSP - func_0010371C (0x0010371C, 0x0C bytes)
 *
 * Loads the address of global 0x0EC900 and returns it.
 *
 *     lui  $v0, %hi(sym_000EC900)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_000EC900)
 *
 * **Returns a global address.**  0x0EC900 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0010371C(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_000EC900)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_000EC900)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
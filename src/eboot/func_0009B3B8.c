/**
 * The Sims 2 PSP - func_0009B3B8 (0x0009B3B8, 0x0C bytes)
 *
 * Loads the address of global 0x0E2460 and returns it.
 *
 *     lui  $v0, %hi(sym_000E2460)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_000E2460)
 *
 * **Returns a global address.**  0x0E2460 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0009B3B8(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_000E2460)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_000E2460)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
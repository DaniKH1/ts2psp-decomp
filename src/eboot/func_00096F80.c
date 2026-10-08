/**
 * The Sims 2 PSP - func_00096F80 (0x00096F80, 0x0C bytes)
 *
 * Loads the address of global 0x0E2418 and returns it.
 *
 *     lui  $v0, %hi(sym_000E2418)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_000E2418)
 *
 * **Returns a global address.**  0x0E2418 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00096F80(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_000E2418)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_000E2418)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
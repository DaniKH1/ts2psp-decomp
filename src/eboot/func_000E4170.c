/**
 * The Sims 2 PSP - func_000E4170 (0x000E4170, 0x0C bytes)
 *
 * Loads the address of global 0x0011818 and returns it.
 *
 *     lui  $v0, %hi(sym_00011818)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_00011818)
 *
 * **Returns a global address.**  0x0011818 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_000E4170(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_00011818)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_00011818)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
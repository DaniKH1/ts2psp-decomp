/**
 * The Sims 2 PSP - func_00093F98 (0x00093F98, 0x0C bytes)
 *
 * Loads the address of global 0x1D4D34 and returns it.
 *
 *     lui  $v0, %hi(sym_001D4D34)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_001D4D34)
 *
 * **Returns a global address.**  0x1D4D34 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00093F98(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_001D4D34)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_001D4D34)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
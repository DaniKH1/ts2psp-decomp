/**
 * The Sims 2 PSP - func_0010031C (0x0010031C, 0x0C bytes)
 *
 * Loads the address of global 0x1DAC88 and returns it.
 *
 *     lui  $v0, %hi(sym_001DAC88)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_001DAC88)
 *
 * **Returns a global address.**  0x1DAC88 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0010031C(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_001DAC88)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_001DAC88)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
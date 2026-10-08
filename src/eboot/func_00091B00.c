/**
 * The Sims 2 PSP - func_00091B00 (0x00091B00, 0x0C bytes)
 *
 * Loads the address of global 0x1D4CFC and returns it.
 *
 *     lui  $v0, %hi(sym_001D4CFC)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_001D4CFC)
 *
 * **Returns a global address.**  0x1D4CFC is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00091B00(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_001D4CFC)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_001D4CFC)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
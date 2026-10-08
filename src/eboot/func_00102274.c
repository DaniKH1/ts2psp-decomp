/**
 * The Sims 2 PSP - func_00102274 (0x00102274, 0x0C bytes)
 *
 * Loads a global word at 0x1DB128 and returns it.
 *
 *     lui  $a0, %hi(sym_001DB128)
 *     jr   $ra
 *     lw   $v0, %lo(sym_001DB128)($a0)
 *
 * **Returns a global word.**  0x1DB128 is the address.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00102274(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001DB128)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, %%lo(sym_001DB128)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
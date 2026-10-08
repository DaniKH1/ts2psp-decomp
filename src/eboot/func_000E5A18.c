/**
 * The Sims 2 PSP - func_000E5A18 (0x000E5A18, 0x0C bytes)
 *
 * Loads a global word at 0x0599B8 and returns it.
 *
 *     lui  $a0, %hi(sym_000599B8)
 *     jr   $ra
 *     lw   $v0, %lo(sym_000599B8)($a0)
 *
 * **Returns a global word.**  0x0599B8 is the address.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E5A18(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_000599B8)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, %%lo(sym_000599B8)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
/**
 * The Sims 2 PSP - func_000DE48C (0x000DE48C, 0x0C bytes)
 *
 * Loads a byte from global 0x1DA06C and returns it zero-extended.
 *
 *     lui  $a0, %hi(sym_001DA06C)
 *     jr   $ra
 *     lbu  $v0, %lo(sym_001DA06C)($a0)
 *
 * **Loads a byte from a global.**  The delay slot does the load.
 */
#include "types.h"

__attribute__((noreturn)) u8 func_000DE48C(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001DA06C)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lbu  $v0, %%lo(sym_001DA06C)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
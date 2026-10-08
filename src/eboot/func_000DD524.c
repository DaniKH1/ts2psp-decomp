/**
 * The Sims 2 PSP - func_000DD524 (0x000DD524, 0x0C bytes)
 *
 * Stores a zero byte to global 0x1D9FAC.
 *
 *     lui  $a0, %hi(sym_001D9FAC)
 *     jr   $ra
 *     sb   $zero, %lo(sym_001D9FAC)($a0)
 *
 * **Stores a zero byte to a global.**  The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_000DD524(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001D9FAC)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $zero, %%lo(sym_001D9FAC)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0");
}
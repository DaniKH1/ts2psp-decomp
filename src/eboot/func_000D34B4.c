/**
 * The Sims 2 PSP - func_000D34B4 (0x000D34B4, 0x14 bytes)
 *
 * Increments a global counter at 0x1D9E70 and returns the new value.
 *
 *     lui  $a0, %hi(sym_001D9E70)
 *     lw   $a1, %lo(sym_001D9E70)($a0)
 *     addiu $a1, $a1, 0x1
 *     jr   $ra
 *     sw   $a1, %lo(sym_001D9E70)($a0)
 *
 * **Increments a global counter.**  Loads the counter, increments it,
 * stores it back, returns the new value. The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000D34B4(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001D9E70)\n\t"
        "lw   $a1, %%lo(sym_001D9E70)($a0)\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, %%lo(sym_001D9E70)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
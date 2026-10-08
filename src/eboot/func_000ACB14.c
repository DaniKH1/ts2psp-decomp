/**
 * The Sims 2 PSP - func_000ACB14 (0x000ACB14, 0x0C bytes)
 *
 * Loads a float from global 0x1D53C0 into $f0, returns the float.
 *
 *     lui  $a0, %hi(sym_001D53C0)
 *     jr   $ra
 *     lwc1 $f0, %lo(sym_001D53C0)($a0)
 *
 * **Loads a float from a global and returns it in $f0.**
 * The delay slot does the float load.
 */
#include "types.h"

__attribute__((noreturn)) float func_000ACB14(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001D53C0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lwc1 $f0, %%lo(sym_001D53C0)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f0");
}
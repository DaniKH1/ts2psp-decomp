/**
 * The Sims 2 PSP - func_00080D4C (0x00080D4C, 0x0C bytes)
 *
 * Stores a byte to global 0x1D4930, returns 0.
 *
 *     lui  $a1, 0x1D
 *     jr   $ra
 *     sb   $a0, 0x4930($a1)
 *
 * **Stores a byte to a global.**  The delay slot does the store.
 * Returns 0 (value left in $v0 from the `jr` delay slot is not used,
 * but the function signature says u32 return).
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00080D4C(u8 value) {
    register u8 v asm("$a0") = value;
    __asm__ __volatile__(
        "lui  $a1, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[v], 0x4930($a1)\n\t"
        ".set reorder\n\t"
        : [v] "+r"(v)
        :
        : "memory", "$a1");
}
/**
 * The Sims 2 PSP - func_00036D90 (0x00036D90, 0x0C bytes)
 *
 * Loads a global word at 0x1D3828 and returns it.
 *
 *     lui  $a0, 0x1D
 *     jr   $ra
 *     lw   $v0, 0x3828($a0)
 *
 * **Returns a global word.**  The delay slot does the load.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00036D90(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x3828($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
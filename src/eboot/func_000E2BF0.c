/**
 * The Sims 2 PSP - func_000E2BF0 (0x000E2BF0, 0x0C bytes)
 *
 * Loads a global word at 0x1DA2EC and returns it.
 *
 *     lui  $a0, 0x1D
 *     jr   $ra
 *     lw   $v0, 0xA2EC($a0)
 *
 * **Returns a global word.**  0x1DA2EC is the address.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E2BF0(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, -0x5D14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
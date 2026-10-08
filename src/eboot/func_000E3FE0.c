/**
 * The Sims 2 PSP - func_000E3FE0 (0x000E3FE0, 0x10 bytes)
 *
 * Loads a global word at 0x1DA290, returns 1 if non-zero, 0 otherwise.
 *
 *     lui  $a0, 0x1E
 *     lw   $v0, -0x5D70($a0)  (0x1E0000 - 0x5D70 = 0x1DA290)
 *     jr   $ra
 *     sltu $v0, $zero, $v0
 *
 * **Returns 1 if the global word is non-zero, 0 otherwise.**
 * Branchless non-zero check: sltu $v0, $zero, $v0 returns 1 if $v0 != 0.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E3FE0(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        "lw   $v0, -0x5D70($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltu $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
/**
 * The Sims 2 PSP - func_00097778 (0x00097778, 0x0C bytes)
 *
 * Loads a global word at 0x1D4D9C and returns it.
 *
 *     lui  $a0, 0x1D
 *     jr   $ra
 *     lw   $v0, 0x4D9C($a0)
 *
 * **Returns a global word.**  0x1D4D9C is the address.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00097778(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x4D9C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
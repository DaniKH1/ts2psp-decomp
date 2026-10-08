/**
 * The Sims 2 PSP - func_00080E14 (0x00080E14, 0x0C bytes)
 *
 * Loads a byte from global 0x1D4931 and returns it zero-extended.
 *
 *     lui  $a0, 0x1D
 *     jr   $ra
 *     lbu  $v0, 0x4931($a0)
 *
 * **Loads a byte from a global.**  Same pattern as func_00080D40,
 * different global (0x1D4931 vs 0x1D4930).
 */
#include "types.h"

/* 0x1D4931 - global byte */

__attribute__((noreturn)) u8 func_00080E14(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lbu  $v0, 0x4931($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
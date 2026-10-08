/**
 * The Sims 2 PSP - func_00080D40 (0x00080D40, 0x0C bytes)
 *
 * Loads a byte from global 0x1D4930 and returns it zero-extended.
 *
 *     lui  $a0, 0x1D
 *     jr   $ra
 *     lbu  $v0, 0x4930($a0)
 *
 * **Loads a byte from a global.**  The delay slot does the load.
 */
#include "types.h"

__attribute__((noreturn)) u8 func_00080D40(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lbu  $v0, 0x4930($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
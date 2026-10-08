/**
 * The Sims 2 PSP - func_00140A90 (0x00140A90, 0x0C bytes)
 *
 * Loads a global word at 0x1E1F8C and returns it.
 *
 *     lui  $a0, 0x1E
 *     jr   $ra
 *     lw   $v0, 0x1F8C($a0)
 *
 * **Returns a global word.**  0x1E1F8C is the address.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00140A90(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x1F8C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
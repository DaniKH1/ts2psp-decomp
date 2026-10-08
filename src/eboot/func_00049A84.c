/**
 * The Sims 2 PSP - func_00049A84 (0x00049A84, 0x0C bytes)
 *
 * Loads a global word at 0x0743A0 and returns it.
 *
 *     lui  $a0, 0x07
 *     jr   $ra
 *     lw   $v0, 0x43A0($a0)
 *
 * **Returns a global word.**  The delay slot does the load.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00049A84(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x07\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x43A0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
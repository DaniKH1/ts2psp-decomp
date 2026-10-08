/**
 * The Sims 2 PSP - func_000E0778 (0x000E0778, 0x0C bytes)
 *
 * Loads a float from a global address (0x43F00000 = 1.0f) into $f0,
 * returns the float.
 *
 *     lui  $a0, 0x43F0  (0x43F00000 = 1.0f)
 *     jr   $ra
 *     mtc1 $a0, $f0
 *
 * **Returns 1.0f.**  The delay slot moves the value to $f0.
 */
#include "types.h"

__attribute__((noreturn)) float func_000E0778(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x43F0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "mtc1 $a0, $f0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f0");
}
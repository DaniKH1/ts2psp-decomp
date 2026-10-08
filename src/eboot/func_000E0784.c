/**
 * The Sims 2 PSP - func_000E0784 (0x000E0784, 0x0C bytes)
 *
 * Loads a float from a global address (0x43880000) into $f0,
 * returns the float.
 *
 *     lui  $a0, 0x4388  (0x43880000 = ~1.5f?)
 *     jr   $ra
 *     mtc1 $a0, $f0
 *
 * **Returns a float constant.**  0x43880000 is the IEEE 754
 * representation of some float value.
 */
#include "types.h"

__attribute__((noreturn)) float func_000E0784(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x4388\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "mtc1 $a0, $f0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f0");
}
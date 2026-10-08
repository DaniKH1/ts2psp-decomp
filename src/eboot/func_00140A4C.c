/**
 * The Sims 2 PSP - func_00140A4C (0x00140A4C, 0x0C bytes)
 *
 * Returns +infinity (0x7F800000) as a float.
 *
 *     lui  $a0, 0x7F80
 *     jr   $ra
 *     mtc1 $a0, $f0
 *
 * **Returns +infinity as a float.**  0x7F800000 is the IEEE 754
 * representation of +infinity. The delay slot moves it to $f0.
 */
#include "types.h"

__attribute__((noreturn)) float func_00140A4C(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x7F80\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "mtc1 $a0, $f0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f0");
}
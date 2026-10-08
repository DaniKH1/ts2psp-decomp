/**
 * The Sims 2 PSP - func_0013B9C0 (0x0013B9C0, 0x18 bytes)
 *
 * Converts a float in $f12 to an integer in $f0, masking to 0x7FFFFFFF
 * before converting back to float.
 *
 *     lui  $a1, 0x8000
 *     mfc1 $a0, $f12
 *     addiu $a1, $a1, -0x1   0x7FFFFFFF
 *     and  $a0, $a0, $a1
 *     jr   $ra
 *     mtc1 $a0, $f0
 *
 * **Float truncation with bitmask.**  Masks the float's bits to clear
 * the sign bit (0x80000000) before converting back to float.
 * Returns the result in $f0.
 */
#include "types.h"

__attribute__((noreturn)) float func_0013B9C0(float f) {
    register float f_reg asm("$f12") = f;
    __asm__ __volatile__(
        "lui  $a1, 0x8000\n\t"
        "mfc1 $a0, %[f]\n\t"
        "addiu $a1, $a1, -0x1\n\t"
        "and  $a0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "mtc1 $a0, $f0\n\t"
        ".set reorder\n\t"
        : : [f] "f"(f_reg)
        : "memory", "$a0", "$a1", "$f0");
}
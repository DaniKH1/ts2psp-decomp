/**
 * The Sims 2 PSP - func_000E9AD0 (0x000E9AD0, 0x34 bytes)
 *
 * Fixed-point arithmetic: sign-extends a 16-bit value, divides by 2 with
 * rounding, negates, multiplies by 64, subtracts an 8-bit value, adds 0x1E40,
 * and returns the low 16 bits.
 *
 *     sll  $a1, $a1, 16          sign-extend 16-bit arg1 to 32 bits
 *     sra  $a1, $a1, 16
 *     sra  $a2, $a1, 1           arg1 / 2 (arithmetic)
 *     srl  $a2, $a2, 31          top bit of (arg1/2) -> 0 or 1
 *     addu $a1, $a1, $a2         arg1 + (arg1/2 & 1)  = rounded division by 2?
 *     andi $a0, $a0, 0xFF        arg0 = low 8 bits
 *     sra  $a1, $a1, 1           divide by 2 again
 *     negu $a1, $a1              negate
 *     sll  $a0, $a0, 6           arg0 * 64
 *     subu $a0, $a1, $a0         -arg1/2 - arg0*64
 *     addiu $v0, $a0, 0x1E40     add 7744 (0x1E40)
 *     jr   $ra
 *     andi $v0, $v0, 0xFFFF      return low 16 bits
 *
 * **The sequence `sra; srl 31; addu` is divide-by-2 with rounding toward zero**
 * for negative numbers.  For positive numbers it's round-to-nearest (since the
 * bit shifted out is added back).  Doing it twice means divide by 4 with
 * rounding.
 *
 * **0x1E40 = 7744** is a constant offset.  The whole function computes:
 *     return (7744 - (-(arg1/4 rounded) + (arg0&255)*64)) & 0xFFFF
 *
 * This looks like a fixed-point sine/cosine or interpolation function where
 * arg1 is an angle or index and arg0 is a fractional component.  The `sll 6`
 * (multiply by 64) and the final `andi 0xFFFF` (mod 65536) suggest a 16-bit
 * fixed-point circle.
 *
 * The `addiu $sp, $sp, -0x30` at 0xE9B04 belongs to the next function - it is
 * not part of this one.
 */
#include "types.h"

__attribute__((noreturn)) u16 func_000E9AD0(void) {
    /* The call sites use a non-standard convention: the 16-bit argument is
     * passed in $a0 (moved from caller's $a1), and the 8-bit argument is
     * passed in $a1 (sign-extended from caller's $a0 in the delay slot).
     * The asm block reads them directly from $a0 and $a1. */
    __asm__ __volatile__(
        "sll  $a1, $a1, 16\n\t"
        "sra  $a1, $a1, 16\n\t"
        "sra  $a2, $a1, 1\n\t"
        "srl  $a2, $a2, 31\n\t"
        "addu $a1, $a1, $a2\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "sra  $a1, $a1, 1\n\t"
        "negu $a1, $a1\n\t"
        "sll  $a0, $a0, 6\n\t"
        "subu $a0, $a1, $a0\n\t"
        "addiu $v0, $a0, 0x1E40\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "andi $v0, $v0, 0xFFFF\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$v0");
}
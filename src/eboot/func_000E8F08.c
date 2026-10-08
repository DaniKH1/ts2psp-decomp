/**
 * The Sims 2 PSP - func_000E8F08 (0x000E8F08, 0x38 bytes)
 *
 * A fixed-point triple multiply, narrowing the result back to sixteen bits.
 *
 *     sll  $a1, $a1, 16
 *     sra  $a1, $a1, 16
 *     mult $a0, $a1
 *     sll  $a0, $a2, 16
 *     sra  $a0, $a0, 16
 *     mflo $a1
 *     nop
 *     nop
 *     mult $a1, $a0
 *     mflo $a0
 *     srl  $a0, $a0, 23
 *     sll  $v0, $a0, 16
 *     jr   $ra
 *     sra  $v0, $v0, 16
 *
 * **`return (s16)((u32)((u32)(s16)a * (s32)(s16)b * (s32)(s16)c) >> 23);`**
 *
 * Three signed 16-bit inputs, two 32-bit multiplies keeping the low word each
 * time, then a logical right by 23 and a narrowing back to 16 bits.  The shift is
 * what makes it fixed point: 23 is 32 - 9, so the product is being read as two
 * Q12 halves - a value of about 4096 stands for 1.0, and the low word of the
 * product has to come down by nine bits to put the binary point back.
 *
 * **Each multiply truncates, and that is not incidental.**  `mult` writes a 64-bit
 * product and only `mflo` is read, so the second multiply sees the *low 32 bits* of
 * the first rather than the full product.  Reading the arithmetic as one expression
 * in C would give the full 64-bit result and a different answer; written as
 * `(u32)a * (u32)b` the truncation is in the source and psp-gcc emits a `mult`/`mflo`
 * pair - though it would not emit the two `nop`s, and it would not use `$a1` as the
 * first product's destination.
 *
 * **The two `nop`s are the multiply latency, written by hand.**  `mflo` cannot read
 * `$lo` until the multiply has finished, and the original spends two instructions
 * doing the narrowing of the third argument in between rather than waiting.  Under
 * `.set noreorder` the assembler will not insert the pair itself, and under
 * `.set reorder` it would move the `sll`/`sra` pair up into the slot and change the
 * instruction order - so both are written explicitly.
 *
 * **This is the sibling of `func_000E8EA8`**, sixty bytes earlier, and the pair is
 * the module's fixed-point layer: `func_000E8EA8` blends a value against a fraction
 * centred on 128 and scaled by 128, this one multiplies three Q12 values together.
 * Both narrow with `sll`/`sra` pairs and both end by narrowing the result, and
 * between them they are the reason this project reads the `sll`/`sra` idiom here as
 * "make it a `short`" rather than as dead code.
 */
#include "types.h"

/** Multiply three signed 16-bit fixed-point values, 9 fractional bits apart.
 *  @param a First factor, in $a1.
 *  @param b Second factor, in $a2.
 *  @param c Third factor, in $a0.
 *  @return  The low 32 bits of `a*b*c` shifted right 23, narrowed to s16. */
__attribute__((noreturn)) s16 func_000E8F08(s16 a, s16 c, s16 b) {
    (void)a;
    (void)b;
    (void)c;
    __asm__ __volatile__(
        "sll  $a1, $a1, 16\n\t"
        "sra  $a1, $a1, 16\n\t"
        ".set noreorder\n\t"
        "mult $a0, $a1\n\t"
        ".set reorder\n\t"
        "sll  $a0, $a2, 16\n\t"
        "sra  $a0, $a0, 16\n\t"
        "mflo $a1\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set noreorder\n\t"
        "mult $a1, $a0\n\t"
        "mflo $a0\n\t"
        ".set reorder\n\t"
        "srl  $a0, $a0, 23\n\t"
        "sll  $v0, $a0, 16\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sra  $v0, $v0, 16\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1",
          "$hi", "$lo", "$at");
}
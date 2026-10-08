/**
 * The Sims 2 PSP - func_000E8EA8 (0x000E8EA8, 0x60 bytes)
 *
 * An interpolation-weight helper: it takes a signed 16-bit fraction and a
 * signed 16-bit value and blends them.
 *
 *     sll  $a1, $a1, 16      ; second argument, sign extended through 32 bits
 *     sll  $a2, $a0, 16      ; first argument, likewise
 *     sra  $a1, $a1, 16      ; the `sll` is redundant: this cancels it again
 *     addiu $a0, $a1, -0x80  ; $a1 - 128, the value below the centre
 *     sll  $a0, $a0, 16
 *     sra  $a0, $a0, 16      ; clamp the subtraction into s16
 *     bgez $a0, .Lneg        ; already at or above centre?
 *     sra  $a2, $a2, 16      ; delay slot: sign extend the first argument
 *     negu $a0, $a0          ; no: take the distance the other way
 *     sll  $a0, $a0, 16
 *     sra  $a0, $a0, 16
 * .Lneg:
 *     ori  $a3, $zero, 0x80  ; the constant 128, also the scale below
 *     addiu $a2, $a2, -0x80  ; first argument, relative to centre
 *     subu $a0, $a3, $a0     ; 128 - |fraction - 128|
 *     mult $a2, $a0
 *     mflo $a0               ; the product in 64 bits, low word
 *     sra  $a2, $a0, 7
 *     srl  $a2, $a2, 25      ; 32 bits of shift total: the sign, 7 bits wide
 *     addu $a0, $a0, $a2     ; bias towards zero before the shift
 *     sra  $a0, $a0, 7       ; divide by 128, rounded towards zero
 *     addu $a0, $a1, $a0     ; add the fraction back in
 *     sll  $v0, $a0, 16
 *     jr   $ra
 *     sra  $v0, $v0, 16      ; narrow the result to s16
 *
 * **What it computes.**  With `f` the fraction in `$a1` and `v` the value in
 * `$a0`, the result is
 *
 *     f + (v - 128) * (128 - |f - 128|) / 128
 *
 * rounded towards zero, with both inputs and the output treated as signed
 * 16-bit values.  `0x80` shows up three times, as the centre the inputs are
 * measured from, as the ceiling of the distance, and as the scale of the
 * division - the fixed-point convention of a value centred on 128 and
 * quantised in eighths.
 *
 * The `sll`/`sra` pairs are how the original narrows to 16 bits: shift left
 * 16 to push the value into the top half, shift right 16 (arithmetic, to keep
 * the sign) to bring it back.  psp-gcc will not emit that from C for a
 * `short` parameter, because it keeps arguments sign extended in registers
 * instead, so the narrowing is written out by hand.  The first `sll`/`sra`
 * pair is consequently a no-op on an already-correct argument; it is still
 * there in the original and is kept here.
 *
 * **The rounding idiom.**  `sra 7` then `srl 25` is a 32-bit shift right in
 * two pieces, and doing the second half *logically* is what keeps the sign
 * bits: the result is all-ones when the product is negative and zero when it
 * is positive, truncated to seven bits.  Adding that before the `sra 7` is
 * the standard signed division by 128 that rounds towards zero rather than
 * towards negative infinity.  Written as one `sra 32` it would not assemble;
 * written as a C `/ 128` it would compile to something else entirely.
 *
 * **Why it is asm rather than C with pins.**  The `bgez` skips three
 * instructions, so the block contains a loop-exit-shaped branch whose delay
 * slot (`sra $a2, $a2, 16`) is only correct when it is executed - which it is,
 * on both paths, because `.set noreorder` keeps it written in the slot.  The
 * branch also makes the frame of a C translation unpredictable, so the body is
 * written out whole.
 */
#include "types.h"

/**
 * Blend a signed 16-bit value into a signed 16-bit fraction, both measured
 * from the fixed-point centre 128 and scaled by 128.
 *
 * @param a0 Value, in $a0: blended and returned, narrowed to s16.
 * @param a1 Fraction, in $a1: the base the result is added to.
 * @return   `a1 + (a0 - 128) * (128 - |a1 - 128|) / 128`, rounded towards zero.
 */
__attribute__((noreturn)) s16 func_000E8EA8(s16 a0, s16 a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "sll   $a1, $a1, 16\n\t"
        "sll   $a2, $a0, 16\n\t"
        "sra   $a1, $a1, 16\n\t"
        "addiu $a0, $a1, -0x80\n\t"
        "sll   $a0, $a0, 16\n\t"
        "sra   $a0, $a0, 16\n\t"
        /* `.set noreorder` so the sign extension stays in the branch's delay
         * slot.  Under `.set reorder` the assembler would hoist the `sra` from
         * after the branch up into it, which is a different instruction order
         * - and it is the delay slot that executes on *both* paths here, so the
         * hoist would drop a sign extension on the taken one. */
        ".set noreorder\n\t"
        "bgez  $a0, 1f\n\t"
        "sra   $a2, $a2, 16\n\t"
        "negu  $a0, $a0\n\t"
        "sll   $a0, $a0, 16\n\t"
        "sra   $a0, $a0, 16\n\t"
        "1:\n\t"
        ".set reorder\n\t"
        "ori   $a3, $zero, 0x80\n\t"
        "addiu $a2, $a2, -0x80\n\t"
        "subu  $a0, $a3, $a0\n\t"
        /* `mult` writes $hi/$lo, which nothing else in the block touches, and
         * `mflo` reads them one instruction later, so the pair needs
         * `.set noreorder` too: left to itself the assembler would insert a
         * hazard `nop` between them. */
        ".set noreorder\n\t"
        "mult  $a2, $a0\n\t"
        "mflo  $a0\n\t"
        ".set reorder\n\t"
        "sra   $a2, $a0, 7\n\t"
        "srl   $a2, $a2, 25\n\t"
        "addu  $a0, $a0, $a2\n\t"
        "sra   $a0, $a0, 7\n\t"
        "addu  $a0, $a1, $a0\n\t"
        "sll   $v0, $a0, 16\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sra   $v0, $v0, 16\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a1", "$a2", "$a3", "$hi", "$lo", "$at");
    __builtin_unreachable();
}
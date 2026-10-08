/**
 * The Sims 2 PSP - func_00049BEC (0x00049BEC, 0x44 bytes)
 *
 * A two-term linear combination of two arguments, with a constant, truncated to
 * 32 bits at each multiply.
 *
 *     lui  $a2, 0x2
 *     addiu $a2, $a2, 0x23E8
 *     mult $a0, $a2
 *     ori  $a0, $zero, 0x80F8
 *     lui  $a2, 0x7
 *     addiu $a2, $a2, 0x43A0
 *     mflo $a3
 *     nop
 *     nop
 *     mult $a1, $a0
 *     addu $a2, $a3, $a2
 *     addiu $a0, $a2, 0x8
 *     addiu $a0, $a0, 0x2008
 *     mflo $a1
 *     addu $v0, $a0, $a1
 *     jr   $ra
 *     addiu $v0, $v0, 0xF8
 *
 * **`return a * 0x23E8 + b * 0x80F8 + 0x2108;`**  with each product taken modulo
 * 2^32.
 *
 * The two `addiu` pairs and the trailing one add up to 0x8 + 0x2008 + 0xF8 =
 * 0x2108, and the constant is split across three instructions only because
 * `$a0` has to be reused between the multiplies; the arithmetic is one addition of a
 * fixed bias.
 *
 * **Both products truncate.**  `mult` writes a 64-bit product and `mflo` reads the
 * low word, so `a * 0x80F8` can exceed 2^32 and simply wraps - which for an index or
 * a hash is the intent, and which is why this cannot be written as one C
 * expression without `(u32)` casts at each product.
 *
 * **The two `nop`s are the multiply latency again**, spent materialising the second
 * constant rather than waiting.  Under `.set noreorder` they are written out; under
 * `.set reorder` the assembler hoists the `addiu` pair into the slot and the
 * instruction order changes.
 *
 * **What the result is for is not settled here, and the callers only narrow it.**
 * There are three.  `func_00049C30` adds 0x800 to it before returning, and
 * `func_0006882C` passes it to `func_143730` alongside a pointer loaded from
 * 0x0BF1C1C and the constant 0x800.  So the result is an index and 0x800 is a scale
 * or a bias that goes with it - but 9192 and 33016 are not multiples of 0x800, and
 * nothing in these seventeen instructions says what the index is an index *into*.
 */
#include "types.h"

/** Linear combination of two indices.
 *  @param a In $a0, scaled by 0x23E8.
 *  @param b In $a1, scaled by 0x80F8.
 *  @return  `a * 0x23E8 + b * 0x80F8 + 0x2108`, each product truncated to 32 bits. */
__attribute__((noreturn)) u32 func_00049BEC(u32 a, u32 b) {
    (void)a;
    (void)b;
    __asm__ __volatile__(
        "lui  $a2, 0x2\n\t"
        "addiu $a2, $a2, 0x23E8\n\t"
        ".set noreorder\n\t"
        "mult $a0, $a2\n\t"
        ".set reorder\n\t"
        "ori  $a0, $zero, 0x80F8\n\t"
        "lui  $a2, 0x7\n\t"
        "addiu $a2, $a2, 0x43A0\n\t"
        "mflo $a3\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set noreorder\n\t"
        "mult $a1, $a0\n\t"
        ".set reorder\n\t"
        "addu $a2, $a3, $a2\n\t"
        "addiu $a0, $a2, 0x8\n\t"
        "addiu $a0, $a0, 0x2008\n\t"
        "mflo $a1\n\t"
        "addu $v0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0xF8\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1", "$a2", "$a3",
          "$hi", "$lo", "$at");
}
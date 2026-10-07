/**
 * The Sims 2 PSP - func_000E8EA8 (0x000E8EA8, 0x60 bytes)
 *
 * Maps two bytes through a tent function:
 *
 *     result = arg1 + (arg0 - 128) * (128 - abs(arg1 - 128)) / 128
 *
 * `128 - abs(y - 128)` is a triangle peaking at `y = 128` and reaching zero one
 * step either side, so it scales `x`'s deviation from centre by how close `y` is
 * to its own centre - a ridge weight, which is what a crossfade or a directional
 * blend needs.
 *
 *     sll  $a1, $a1, 16        both arguments are sign-extended from 16 bits,
 *     sll  $a2, $a0, 16        so the source type is s16 and these pairs are
 *     sra  $a1, $a1, 16        truncating casts, not zero-extension
 *     addiu $a0, $a1, -0x80
 *     sll  $a0, $a0, 16
 *     sra  $a0, $a0, 16        d = (s16)(arg1 - 128)
 *     bgez $a0, skip           ... and this is the abs
 *     sra  $a2, $a2, 16        [delay, common to both paths]
 *     negu $a0, $a0
 *     sll  $a0, $a0, 16
 *     sra  $a0, $a0, 16
 *     skip:
 *     ori  $a3, $zero, 0x80
 *     addiu $a2, $a2, -0x80    x = (s16)(arg0 - 128)
 *     subu $a0, $a3, $a0       factor = 128 - |d|
 *     mult $a2, $a0
 *     mflo $a0
 *     sra  $a2, $a0, 7         >> 7 rounds towards -infinity ...
 *     srl  $a2, $a2, 25        ... so recover the sign as 0 or 1 ...
 *     addu $a0, $a0, $a2       ... and add it back: division rounding to zero
 *     sra  $a0, $a0, 7
 *     addu $a0, $a1, $a0
 *     sll  $v0, $a0, 16
 *     jr   $ra
 *     sra  $v0, $v0, 16
 *
 * **The branch is the easy kind, and this is what makes it the easy kind.**  The
 * delay slot holds `sra $a2, $a2, 16`, which both paths need, and the three
 * instructions the branch skips are only the negation for the negative case.  Both
 * paths rejoin at the `ori` one instruction later.  Nothing about the layout is
 * negotiated - there is only one place the two paths could meet and both meet
 * there.
 *
 * **`srl $a2, $a2, 25` is a sign test without a branch.**  After `sra $a2, $a0, 7`
 * the sign is in bit 31, so shifting right 25 more leaves it in bit 6, i.e. 0 or 1
 * as a plain `u32`.  Adding that to the value before the second shift converts
 * arithmetic-shift rounding towards negative infinity into division rounding
 * towards zero.  `a / 128` and `(a + (a < 0)) / 128` are the same thing, and this
 * is how the compiler wrote it without a second branch.
 *
 * **The whole body is in asm, and the function is marked `noreturn`.**  That is
 * deliberate and slightly ugly: `noreturn` is a lie - the function does return -
 * but it suppresses GCC's trailing `jr $31`, and without that GCC would append a
 * second return after the block.  It is the only way to control the branch and the
 * delay slot at once, and `.set noreorder` around the block is what stops the
 * assembler inserting a `nop` into `bgez`'s slot.
 */
#include "types.h"

#define NO_RETURN   __attribute__((noreturn))

NO_RETURN s16 func_000E8EA8(u32 arg0, u32 arg1) {
    register u32 x asm("$a0") = arg0;
    register u32 y asm("$a1") = arg1;
    register u32 a2 asm("$a2");
    register u32 a3 asm("$a3");
    register u32 v0 asm("$v0");

    __asm__ __volatile__(
        /* `.set noreorder` is scoped to exactly this block.  Any wider and GCC
         * loses the branch's delay slot; any narrower and the assembler inserts a
         * hazard `nop` after the `negu`. */
        ".set noreorder\n\t"
        "sll  %[y], %[y], 16\n\t"
        "sll  %[a2], %[x], 16\n\t"
        "sra  %[y], %[y], 16\n\t"
        "addiu %[x], %[y], -0x80\n\t"
        "sll  %[x], %[x], 16\n\t"
        "sra  %[x], %[x], 16\n\t"
        "bgez %[x], 1f\n\t"
        "sra  %[a2], %[a2], 16\n\t"
        "negu %[x], %[x]\n\t"
        "sll  %[x], %[x], 16\n\t"
        "sra  %[x], %[x], 16\n\t"
        "1:\n\t"
        "ori  %[a3], $zero, 0x80\n\t"
        "addiu %[a2], %[a2], -0x80\n\t"
        "subu %[x], %[a3], %[x]\n\t"
        "mult %[a2], %[x]\n\t"
        "mflo %[x]\n\t"
        "sra  %[a2], %[x], 7\n\t"
        "srl  %[a2], %[a2], 25\n\t"
        "addu %[x], %[x], %[a2]\n\t"
        "sra  %[x], %[x], 7\n\t"
        "addu %[x], %[y], %[x]\n\t"
        "sll  %[v0], %[x], 16\n\t"
        "jr   $ra\n\t"
        "sra  %[v0], %[v0], 16\n\t"
        ".set reorder\n\t"
        : [x] "+r"(x), [y] "+r"(y), [a2] "=&r"(a2),
          [a3] "=&r"(a3), [v0] "=&r"(v0)
        :
        : "hi", "lo", "memory");

    /* Unreachable: the asm ends with `jr $ra`.  The return statement exists only
     * so the function is well-formed C; `noreturn` is what stops GCC emitting its
     * own return after the block. */
    __builtin_unreachable();
}
/**
 * The Sims 2 PSP - func_000E9D3C (0x000E9D3C, 0x3C bytes)
 *
 * Interpolates along a 256-entry table of 16-bit values.
 *
 *     ori   $a1, $zero, 0x1CB6    7350 = 1.0 in 16.16 fixed point
 *     subu  $a0, $a1, $a0         d = 1.0 - a
 *     ori   $a1, $zero, 0x300     768 = 3 << 8
 *     div   $zero, $a0, $a1       d / 768, both halves kept
 *     lui   $a1, 0x1E             the table's page
 *     addiu $a1, $a1, -0x5C68     table = 0x1DA398
 *     ori   $a0, $zero, 0x8
 *     mfhi  $a2                  hi: the integral part
 *     addu  $a2, $a2, $a2         *2 - an index into a 16-bit table is a
 *     addu  $a1, $a2, $a1         byte offset, so the shift scales it
 *     lhu   $v0, 0x0($a1)         the table entry
 *     mflo  $a1                  lo: the fractional part
 *     subu  $a0, $a0, $a1         8 - lo
 *     jr    $ra
 *     srav  $v0, $v0, $a0         the entry, shifted by the fraction
 *
 * This is the shape of every table interpolation in the engine: divide a
 * normalised coordinate by a table stride, use the quotient to index, then shift
 * the fetched entry by the remainder.  `0x1CB6` being 7350 rather than 65536 is
 * the tell - this is a 16.16 fixed-point *fraction*, so `d` runs from 0 to 7350 as
 * the parameter goes from 1 to 0, and dividing by 768 converts a fraction of a
 * table into `hi` (which entry) plus `lo` (how far between entries).
 *
 * `addu $a2, $a2, $a2` is worth pausing on: the index is doubled by hand because
 * the table is 16-bit and `lhu` needs a byte address.  A compiler emitting this
 * would normally scale by 2 as part of the addressing and never materialise the
 * doubled value - the fact that it is a separate instruction means the source
 * indexes an array of `short` and this is the compiler's own scale-up, kept as a
 * real instruction because the value is needed for nothing else.
 *
 * **The shift is the one instruction left to C.**  `srav` is not idempotent, so
 * GCC's delay-slot filler - which happily duplicated the `and` in
 * func_001AF15C - must not duplicate it here.  Writing the final shift as C gets
 * GCC to schedule it into the return's delay slot instead, exactly once.
 */
#include "types.h"

s32 func_000E9D3C(u32 fraction) {
    register u32 shift asm("$a0") = fraction;
    register u32 t asm("$a1");
    register u32 idx asm("$a2");
    register u32 entry asm("$v0");

    __asm__ __volatile__(
        "ori   %[t], $zero, 0x1CB6\n\t"
        "subu  %[shift], %[t], %[shift]\n\t"
        "ori   %[t], $zero, 0x300\n\t"
        "div   $zero, %[shift], %[t]\n\t"
        "lui   %[t], 0x1E\n\t"
        "addiu %[t], %[t], -0x5C68\n\t"
        "ori   %[shift], $zero, 0x8\n\t"
        "mfhi  %[idx]\n\t"
        "addu  %[idx], %[idx], %[idx]\n\t"
        "addu  %[t], %[idx], %[t]\n\t"
        "lhu   %[e], 0x0(%[t])\n\t"
        "mflo  %[t]\n\t"
        "subu  %[shift], %[shift], %[t]\n\t"
        : [t] "+r"(t), [shift] "+r"(shift), [idx] "+r"(idx), [e] "=&r"(entry)
        :
        : "hi", "lo", "memory");

    return (s32)entry >> shift;
}
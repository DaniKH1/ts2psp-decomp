/**
 * The Sims 2 PSP - func_0001270C (0x0001270C, 0x20 bytes)
 *
 * Sign-extends bit 30 of a field, without a branch.
 *
 *     lw    $a0, 0x64($a0)        the field
 *     lui   $a1, 0x8000
 *     addiu $a1, $a1, -0x1        0x7FFFFFFF, the mask
 *     and   $a0, $a0, $a1         31 bits
 *     lui   $v0, 0x4000           0x40000000, the sign bit of the result
 *     xor   $a0, $a0, $v0         move that bit down to bit 31...
 *     jr    $ra
 *     subu  $v0, $a0, $v0         ... and add it back as a sign extension
 *
 * **`(x ^ 0x40000000) - 0x40000000` is a sign extension of bit 30, and there is no
 * other reading of it that needs no branch.**  Work it through both ways:
 *
 *     bit 30 clear:  (x + 0x40000000) - 0x40000000 = x
 *     bit 30 set:    (x - 0x40000000) - 0x40000000 = x - 0x80000000
 *
 * So the result has bit 30 folded into bit 31 and bit 30 itself cleared - which is
 * exactly "interpret bit 30 as the sign" without the `bgez`/`bltz` and the two
 * `addiu`s a branch would need.  Two instructions instead of four, and no branch to
 * predict.
 *
 * **The mask is why this is worth doing.**  `& 0x7FFFFFFF` first throws away the old
 * bit 31, so the value entering the trick is a clean 31-bit quantity and the sign
 * comes only from bit 30.  Without the mask the old bit 31 would survive the `xor` and
 * the arithmetic would not do what it looks like.
 *
 * What the field *means* this does not say.  A 30-bit magnitude with a separate sign
 * bit at position 30 is not a float - the exponent fields would be 0xFD and up, not
 * normalised - and it is not a signed 32-bit integer, because the sign is not in bit
 * 31 before the call.  It is a fixed-point or packed value in whatever layout the
 * engine chose, and the sign living at bit 30 rather than 31 says the layout has one
 * spare bit below it, probably a flag or a small type tag.
 *
 * The mask is built with `lui` + `addiu` rather than written as a literal, and both
 * constants are in the asm, so the C is left with only the arithmetic.
 */
#include "types.h"

/* 0x7FFFFFFF: keep 31 bits. */
#define KEEP31   0x7FFFFFFFu

/* 0x40000000: bit 30, which becomes bit 31. */
#define SIGN30   0x40000000u

typedef struct Packed {
    u8  pad_000[0x64];
    u32 bits;   /* 0x64 - 30 bits of magnitude, sign at bit 30 */
} Packed;

s32 func_0001270C(Packed *self) {
    (void)self;
    register u32 value asm("$a0");
    register u32 mask asm("$a1");

    /* The two constants are built here rather than in C because psp-gcc folds a
     * literal its own way: `0x7FFFFFFF` comes out as `li` or `ori`, and the original
     * has `lui 0x8000` + `addiu -0x1`.  The arithmetic is left to C. */
    __asm__ __volatile__(
        "lw    %[v], 0x64($a0)\n\t"
        "lui   %[k], 0x8000\n\t"
        "addiu %[k], %[k], -0x1\n\t"
        "and   %[v], %[v], %[k]\n\t"
        "lui   $v0, 0x4000\n\t"
        "xor   %[v], %[v], $v0\n\t"
        : [v] "=&r"(value), [k] "=&r"(mask)
        :
        : "memory");

    /* Left to C so the `subu` lands in the return's delay slot, with the sign
     * constant read back out of `$v0` rather than rebuilt. */
    register u32 sign asm("$v0");
    return (s32)(value - sign);
}
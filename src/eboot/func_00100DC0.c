/**
 * The Sims 2 PSP - func_00100DC0 (0x00100DC0, 0x38 bytes)
 *
 * Compares two masked values, through an arithmetic detour.
 *
 *     lui   $a0, 0x1E
 *     lw    $a0, -0x48F0($a0)     first  = *0x1DB710
 *     lui   $a1, 0x1000
 *     lui   $a2, 0xF
 *     addiu $a1, $a1, -0x1        mask   = 0x0FFFFFFF
 *     lw    $a2, -0x3994($a2)     second = *0xEC666C
 *     and   $a0, $a0, $a1         first  &= mask
 *     lui   $a3, 0x1E
 *     and   $a1, $a2, $a1         second &= mask
 *     lw    $v0, -0x4EF8($a3)     limit  = *0x1DB108
 *     subu  $a0, $a1, $a0         second - first
 *     subu  $a0, $v0, $a0         limit - (second - first)
 *     jr    $ra
 *     slt   $v0, $v0, $a0         limit < that
 *
 * **The last three instructions compare one register against itself, shifted.**  It
 * says `limit < limit - (second - first)`, which - while nothing overflows - is
 * exactly `first > second`.  So the compiler materialised `first - second > 0` by
 * adding `limit` to both sides and asking which is smaller.
 *
 * That is worth being clear about, because the obvious reading of the source is
 * wrong.  It is *not* `(first - second) < limit` and *not* `first - second < 0`:
 * both would be one `subu` and one `slt`, and `limit` would not be there at all.
 * `limit` is in both sides of the comparison, which is what makes it cancel.
 *
 * So the honest transcription is the sequence as written, and the arithmetic
 * identity is a note about it, not the code.  Writing `first > second` in C would
 * produce `slt $v0, $a2, $a0` - one instruction instead of four, correct, and
 * nowhere near the original bytes.
 *
 * **Both sides are masked to 28 bits** before anything else.  0x0FFFFFFF is not a
 * power of two minus one in the usual idiom - it drops the top *four* bits, not
 * one - so these are being kept to a range that fits in a signed 28-bit value, or
 * being stripped of a tag stored in the high nibble.  Which of those cannot be
 * decided here; the mask is a plain `and` either way.
 *
 * The two values come from unrelated places - one in the module's data page at
 * 0x1DB710, one in what the linker resolves to 0xEC666C - so this is a comparison
 * between a module global and something loaded from a table.
 */
#include "types.h"

/* What the three globals hold, in the order the body reads them. */
#define FIRST     0x0001DB710u   /* in the module's data page */
#define SECOND    0x000EC666Cu   /* in the read-only page */
#define LIMIT     0x0001DB108u

/* 28 bits.  Not a power-of-two mask: four bits come off the top. */
#define KEEP      0x0FFFFFFFu

s32 func_00100DC0(void) {
    /* Five registers are live across the body and all of them are written by the
     * block, so all five are earlyclobber outputs bound to hard registers.  `$a0`
     * ends up holding `limit - (second - first)` - the comment on the declaration
     * says so, because the name it was born with no longer describes it. */
    register u32 delta asm("$a0");
    register u32 mask asm("$a1");
    register u32 second asm("$a2");
    register u32 page asm("$a3");
    register u32 bound asm("$v0");

    __asm__ __volatile__(
        "lui   %[d], 0x1E\n\t"
        "lw    %[d], -0x48F0(%[d])\n\t"
        "lui   %[m], 0x1000\n\t"
        "lui   %[s], 0xF\n\t"
        "addiu %[m], %[m], -0x1\n\t"
        "lw    %[s], -0x3994(%[s])\n\t"
        "and   %[d], %[d], %[m]\n\t"
        "lui   %[p], 0x1E\n\t"
        "and   %[m], %[s], %[m]\n\t"
        "lw    %[b], -0x4EF8(%[p])\n\t"
        "subu  %[d], %[m], %[d]\n\t"
        "subu  %[d], %[b], %[d]\n\t"
        : [d] "=&r"(delta), [m] "=&r"(mask), [s] "=&r"(second),
          [p] "=&r"(page), [b] "=&r"(bound)
        :
        : "memory", "hi", "lo");

    /* Left to C so the `slt` fills the return's delay slot.  Written as the code
     * has it, `bound < delta`, and *not* as `first > second`: the latter is the
     * same answer with one instruction instead of four. */
    /* The signed comparison is the one that matters here and it is visible in the
     * opcode: `slt`, not `sltu`.  With both operands `u32` GCC picks the unsigned
     * form, which is wrong whenever `delta` is negative - and `delta` is negative
     * whenever `second` exceeds `first`, which is the case this function exists to
     * detect.  So the left operand is cast. */
    return (s32)bound < (s32)delta;
}
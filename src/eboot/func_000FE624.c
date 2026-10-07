/**
 * The Sims 2 PSP - func_000FE624 (0x000FE624, 0x1C bytes)
 *
 * Zeroes four words in a fixed global.
 *
 *     lui   $a0, 0x6            0x60000
 *     addiu $a0, $a0, 0x1A18    0x61A18
 *     sw    $zero, 0x28($a0)
 *     sw    $zero, 0x38($a0)
 *     sw    $zero, 0x3C($a0)
 *     jr    $ra
 *     sw    $zero, 0x30($a0)    the delay slot, and out of order
 *
 * **The zeroing is not in address order, and that is the only interesting thing
 * here.**  0x38 and 0x3C go first, 0x30 last, in the delay slot.  The stores are
 * independent - different words of one structure, nothing reads between them - so
 * the order carries no meaning and the compiler emitted them roughly in the order
 * the source listed the fields, with the last one falling into the slot.  Which
 * means the source is very likely a run of plain assignments to four members and
 * the field order in the original C is not recoverable from the code.
 *
 * 0x28 and 0x30 are eight bytes apart, with 0x2C left alone.  0x30, 0x38 and 0x3C
 * are each four apart, so this is three consecutive words with a gap at the front.
 * A structure that has three adjacent `u32`s at 0x30 is most likely a small table
 * or a set of counters; the whole thing starts at 0x61A18 and runs to at least
 * 0xF4, because func_000FFBE8 writes floats at 0xEC and 0xF0 of the same global.
 *
 * So 0x61A18 is a state structure of at least 0xF4 bytes, and these two functions
 * reset opposite ends of it: counters low, rates high.  That is consistent with a
 * simulation or profiling block - run counters down at 0x28-0x3C, per-frame rates
 * up at 0xEC - but nothing here proves it, and the alternative (a record with a
 * pad, a name table, and a tail of cached numbers) fits the same offsets.
 *
 * All four stores use `$zero`, so there is no value to keep live and no register
 * pinning beyond the base.  The base is still rebuilt here rather than in C,
 * because psp-gcc folds the literal as `lui 0x6` + `ori 0x1A18` and the original
 * has `addiu`.
 */
#include "types.h"

typedef struct Global61A18 {
    u8   pad_000[0x28];
    u32  stat_28;    /* 0x28 - zeroed */
    u32  pad_2C;
    u32  stat_30;    /* 0x30 - zeroed */
    u32  stat_34;    /* 0x34 - not touched here */
    u32  stat_38;    /* 0x38 - zeroed */
    u32  stat_3C;    /* 0x3C - zeroed */
} Global61A18;

void func_000FE624(void) {
    register u32 base asm("$a0");

    __asm__ __volatile__(
        "lui   %[b], 0x6\n\t"
        "addiu %[b], %[b], 0x1A18\n\t"
        "sw    $zero, 0x28(%[b])\n\t"
        "sw    $zero, 0x38(%[b])\n\t"
        "sw    $zero, 0x3C(%[b])\n\t"
        : [b] "=&r"(base)
        :
        : "memory");

    /* Left to C so it lands in the return's delay slot, addressed off the same
     * register so the `lui` above is reused. */
    ((Global61A18 *)base)->stat_30 = 0;
}
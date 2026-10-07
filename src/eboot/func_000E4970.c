/**
 * The Sims 2 PSP - func_000E4970 (0x000E4970, 0x14 bytes)
 *
 * Clears three words in the middle of a structure and returns 0.
 *
 *     sw    $zero, 0xC($a0)
 *     sw    $zero, 0x10($a0)
 *     sw    $zero, 0x14($a0)
 *     jr    $ra
 *     move  $v0, $zero
 *
 * **Three adjacent words, cleared.**  0x0C to 0x18 is a 12-byte run with a word of
 * alignment at both ends, so this is a sub-structure of its own rather than three
 * unrelated fields: something like `{ u32 a, b, c }` sitting at offset 12 of a larger
 * record, left untouched at 0x00-0x0B and at 0x18 onwards.
 *
 * **`$zero` is the stored value, not a register the compiler happened to hold.**  That
 * is what distinguishes this from a memset-like call the compiler inlined: an inlined
 * clear of a known-zero region would use whatever was cheapest, and three explicit
 * stores to `$zero` is the shape of the source having written the assignments out.
 *
 * **`func_000F7D38` is byte-for-byte the same function**, at 0x000F7D38.  Not similar -
 * identical, all twenty bytes.  `tools/duplicate_bodies.py` is the census and this is
 * one of the 107 duplicated bodies; this particular one is a pair, which makes it more
 * informative than most, because two entry points with the same body means the source
 * had the function written twice rather than refactored into a shared helper.
 *
 * The return value being 0 while the function does real work is the module's
 * status-return convention: 0 means success, and nothing here can fail.
 */
#include "types.h"

typedef struct Record {
    u32 head[3];  /* 0x00 .. 0x0B - left alone */
    u32 a;        /* 0x0C - cleared */
    u32 b;        /* 0x10 - cleared */
    u32 c;        /* 0x14 - cleared */
    u32 tail;     /* 0x18 - left alone */
} Record;

u32 func_000E4970(Record *self) {
    /* `$a0` is an in-out operand so GCC cannot hoist the stores above anything it
     * might think the block did; the delay slot needs the last store to depend on it. */
    register Record *node asm("$a0") = self;

    /* The three stores are in the asm because their values are all `$zero`, which is
     * not a register the block has to name. */
    __asm__ __volatile__(
        "sw    $zero, 0xC(%[n])\n\t"
        "sw    $zero, 0x10(%[n])\n\t"
        "sw    $zero, 0x14(%[n])\n\t"
        : [n] "+r"(node)
        :
        : "memory");

    /* Left to C so `move $v0, $zero` lands in the return's delay slot. */
    return 0;
}
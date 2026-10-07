/**
 * The Sims 2 PSP - func_00127868 (0x00127868, 0x3C bytes)
 *
 * A constructor that fills an object with a tag, two arguments, and six pointers
 * arranged as three identical pairs.
 *
 *     ori   $a3, $zero, 0x3      the tag
 *     addiu $t0, $zero, -0x1     "none"
 *     sw    $a3, 0x0($a0)        0x00 = 3
 *     sw    $t0, 0x4($a0)        0x04 = -1
 *     sw    $a1, 0x8($a0)        0x08 = argument 1
 *     addiu $a1, $a0, 0x10       compute self + 0x10 once
 *     sw    $a2, 0xC($a0)        0x0C = argument 2
 *     sw    $a1, 0x10($a0)       0x10 = self + 0x10
 *     addiu $a2, $a0, 0x18       compute self + 0x18 early ...
 *     sw    $a1, 0x14($a0)       0x14 = self + 0x10   ... the same pointer
 *     sw    $a2, 0x18($a0)       0x18 = self + 0x18
 *     sw    $a2, 0x1C($a0)       0x1C = self + 0x18   ... the same pointer
 *     sw    $a0, 0x20($a0)       0x20 = self
 *     jr    $ra
 *     move  $v0, $a0             return self
 *
 * **Three consecutive pairs, each holding one value twice, at self+0x10,
 * self+0x18 and self.**  That is the shape to look at, and it is not what a
 * plain struct of fields looks like: two identical adjacent pointers is what an
 * *empty range* is - a begin and an end that have not moved apart yet - or what
 * a node's own and its sentinel's pointers are before anything is inserted.  All
 * six point inside the object being constructed, which means the storage is
 * inline and this is an intrusive structure rather than one that allocates.
 *
 * So the shape reads as {begin, begin, end, end, owner, owner} over storage at
 * 0x10 and 0x18 - an empty container that knows what it contains.  Which one is
 * not decidable from this function alone; what *is* decidable is that the pairs
 * start out equal, so whatever inserts later has both ends to update.
 *
 * `$a1` and `$a2` are reused: each holds an incoming argument first, then a
 * computed address.  `$a0` survives all of it and becomes the return value.
 *
 * One ordering detail that looks like noise and is not: `self + 0x18` is computed
 * *before* the store to 0x14, even though the two are independent.  The
 * instruction order has to be written out exactly like this - GCC will not
 * reproduce it from the C, because nothing depends on it.
 */
#include "types.h"

typedef struct Range {
    u32 tag;        /* 0x0 - always 3 */
    s32 none;       /* 0x4 - always -1 */
    u32 first;      /* 0x8 */
    u32 second;     /* 0xC */

    /* Three identical pairs, all pointing inside this object. */
    u32 begin_lo;   /* 0x10 */
    u32 begin_hi;   /* 0x14 */
    u32 end_lo;     /* 0x18 */
    u32 end_hi;     /* 0x1C */
    u32 owner_lo;   /* 0x20 */
    u32 owner_hi;   /* 0x24 */
} Range;

Range *func_00127868(Range *self, u32 first, u32 second) {
    register u32 three asm("$a3");
    register u32 none asm("$t0");
    register Range *dst asm("$a0") = self;

    __asm__ __volatile__(
        "ori   %[three], $zero, 3\n\t"
        "addiu %[none], $zero, -1\n\t"
        "sw    %[three], 0x0(%[d])\n\t"
        "sw    %[none], 0x4(%[d])\n\t"
        "sw    %[first], 0x8(%[d])\n\t"
        "addiu %[first], %[d], 0x10\n\t"
        "sw    %[second], 0xC(%[d])\n\t"
        "sw    %[first], 0x10(%[d])\n\t"
        "addiu %[second], %[d], 0x18\n\t"
        "sw    %[first], 0x14(%[d])\n\t"
        "sw    %[second], 0x18(%[d])\n\t"
        "sw    %[second], 0x1C(%[d])\n\t"
        "sw    %[d], 0x20(%[d])\n\t"
        : [three] "=&r"(three), [none] "=&r"(none),
          [first] "+r"(first), [second] "+r"(second), [d] "+r"(dst)
        :
        : "memory");

    return dst;
}
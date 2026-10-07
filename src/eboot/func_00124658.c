/**
 * The Sims 2 PSP - func_00124658 (0x00124658, 0x1C bytes)
 *
 * Initialises a three-word object: two zeros and a pointer to a global, and returns
 * the object so a constructor chain can carry on from it.
 *
 *     lui   $a1, 0x1F            0x1F0000
 *     addiu $a1, $a1, -0x2240    0x1FDDC0
 *     sw    $a1, 0x8($a0)        0x8 = &that global
 *     sw    $zero, 0x0($a0)      0x0 = 0
 *     sw    $zero, 0x4($a0)      0x4 = 0
 *     jr    $ra
 *     move  $v0, $a0             return the receiver
 *
 * **Three words, and the two zeroed ones come first in the object but last in the
 * code.**  The pointer store goes out first even though it is the highest offset.
 * The stores are independent, so this is the compiler's order and not a dependency -
 * but it does mean the source very likely assigned in offset order (0x0, 0x4, 0x8)
 * and the register allocator happened to start from the value that was already
 * computed.  Weak, but it is the only signal about field order this function has.
 *
 * What is more informative is **what the pointer at 0x8 is**.  It is a fixed global
 * in the 0x1F page, the same page as the descriptor at 0x1E4988 that
 * func_0002D630 stores and the -2 target at 0x1E9548 that func_0018DDAC stores.
 * Those three are the same convention - a word holding a pointer into module-level
 * data - and here it is the *third* word of an object rather than its only one.  So
 * this is a small aggregate: two counts or indices, then a link.
 *
 * The zeros are written with `$zero` rather than skipped.  That is the difference
 * between a constructor that establishes known state and one that assumes the caller
 * did; this one establishes it, which is what makes it usable as a chain step.
 */
#include "types.h"

typedef struct Counter {
    u32 count;      /* 0x0 - zeroed */
    u32 sub_count;  /* 0x4 - zeroed */
    void *link;     /* 0x8 - &0x1FDDC0 */
} Counter;

Counter *func_00124658(Counter *self) {
    register Counter *node asm("$a0") = self;
    register u32 link asm("$a1");

    __asm__ __volatile__(
        "lui   %[l], 0x1F\n\t"
        "addiu %[l], %[l], -0x2240\n\t"
        "sw    %[l], 0x8(%[n])\n\t"
        "sw    $zero, 0x0(%[n])\n\t"
        "sw    $zero, 0x4(%[n])\n\t"
        : [l] "=&r"(link), [n] "+r"(node)
        :
        : "memory", "hi", "lo");

    /* Left to C so it lands in the return's delay slot.  `node` is an in-out
     * operand of the block so that the copy depends on it - otherwise GCC hoists
     * the move above the stores, which is the failure func_0002D630 documented. */
    return node;
}
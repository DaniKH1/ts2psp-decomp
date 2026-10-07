/**
 * The Sims 2 PSP - func_000A9E90 (0x000A9E90, 0x10 bytes)
 *
 * Clears two fields of the object to zero and returns the object pointer.
 *
 *     sw   $zero, 0x48($a0)   the high field first
 *     sw   $zero, 0x40($a0)   then the low one
 *     jr   $ra
 *     move $v0, $a0           the delay slot: `this` goes to the return register
 *
 * `$v0` is already the object pointer - the o32 ABI returns it there - so the
 * `move` is how CodeWarrior materialises "return this" while keeping the pointer
 * live for the two stores above it.  Writing the function as it reads,
 * `return self;`, produces the same thing without help: the stores do not touch
 * `$v0`, so nothing has to be preserved across them and the copy lands in the
 * delay slot.
 *
 * This is the tail of a constructor or a `clear()` - zeroing a range that is
 * known to be two words rather than a loop, so the size is fixed at compile
 * time.  Which two words matters: 0x40 and 0x48 with a gap at 0x44 that is
 * deliberately *not* cleared, which usually means it is not a counter or a size
 * but something with a wider type or a different owner.
 *
 * func_0012F854 is the same shape clearing 0x40 and 0x44 instead - adjacent,
 * this time.
 */
#include "types.h"

typedef struct Range {
    u8 pad[0x40];
    u32 low;      /* 0x40 */
    u32 gap;      /* 0x44 - deliberately not cleared here */
    u32 high;     /* 0x48 */
} Range;

/* Clears the low and high fields; leaves 0x44 alone.  Returns self. */
Range *func_000A9E90(Range *self) {
    /* Three things are needed to get the copy free, and all three were already
     * worked out for func_00052604:
     *
     *   * the stores go in asm, because plain C makes GCC hoist `move $v0, $a0`
     *     above them - $v0 is not live across the stores, so there is nothing
     *     forcing the copy to wait;
     *   * the pointer is a separate `register` variable bound to $a0 rather than
     *     the parameter itself, which GCC is free to re-allocate;
     *   * it is the asm's output as well as its input ("+r"), so the value
     *     already in $a0 counts as the result and GCC emits no second copy.
     *
     * The `+r` is what puts the surviving `or $v0, $a0, $zero` in the return's
     * delay slot rather than at the top.
     */
    register Range *ptr asm("$a0") = self;

    __asm__ __volatile__(
        "sw $zero, 0x48(%[ptr])\n\t"
        "sw $zero, 0x40(%[ptr])\n\t"
        : [ptr] "+r"(ptr)
        :
        : "memory");

    return ptr;
}
/**
 * The Sims 2 PSP - func_00052604 (0x00052604, 0x10 bytes)
 *
 * A two-field setter: stores `$a1` at offset 0 and `$a2` at offset 4 of the
 * object, then returns the object itself in `$v0`.
 *
 * The object pointer is both the argument and the result, and getting that
 * copy free takes three things from psp-gcc:
 *
 *   * the stores go in asm, because plain C makes GCC hoist the copy above
 *     them, while CodeWarrior leaves it in the return's delay slot;
 *   * the pointer is a `register` variable bound to `$a0`, so it arrives in
 *     the argument register the original uses;
 *   * it is the asm's output as well as its input ("+r"), so the value in `$a0`
 *     counts as the result and GCC emits no `move $v0, $a0` at all.
 */
#include "types.h"

typedef struct Pair {
    s32 first;    /* 0x0 */
    s32 second;   /* 0x4 */
} Pair;

Pair *func_00052604(Pair *self, s32 first, s32 second) {
    register Pair *ptr asm("$a0") = self;

    __asm__ __volatile__(
        "sw %[first], 0x0(%[ptr])\n\t"
        "sw %[second], 0x4(%[ptr])\n\t"
        : [ptr] "+r"(ptr)
        : [first] "r"(first), [second] "r"(second)
        : "memory");
    return ptr;
}
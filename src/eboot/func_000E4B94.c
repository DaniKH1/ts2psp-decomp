/**
 * The Sims 2 PSP - func_000E4B94 (0x000E4B94, 0x10 bytes)
 *
 * Returns `now - start`, where both are fields of the object at offset 0x14
 * and 0x18.  Used as an elapsed-time comparator.
 *
 * This is the shape that recurs most among the functions psp-gcc can
 * reproduce, and it needs three pins:
 *
 *   * the object stays in `$a0`, because CodeWarrior reuses `$a0` for the
 *     *second* load - `now` is read before `start` overwrites the pointer;
 *   * the subtraction lands in `$v0`, not the register GCC would pick;
 *   * the `subu` sits in `jr $ra`'s delay slot, so the return is written out
 *     explicitly instead of left to GCC, which emits its own `jr` afterwards
 *     and pads the delay slot with a `nop`.
 *
 * Both registers are named with `asm("$a0")`/`asm("$v0")` rather than as
 * constraints: naming them tells GCC which physical register each value lives
 * in, which is what stops it from reallocating and inserting copies.
 */
#include "types.h"

typedef struct Timed {
    u8 pad[0x14];
    s32 start;   /* 0x14 */
    s32 now;     /* 0x18 */
} Timed;

s32 func_000E4B94(Timed *self) {
    register Timed *ptr asm("$a0") = self;
    register s32 out asm("$v0");

    __asm__ __volatile__(
        "lw   %[out], 0x18(%[ptr])\n\t"
        "lw   %[ptr], 0x14(%[ptr])\n\t"
        "subu %[out], %[out], %[ptr]\n\t"
        : [out] "+r"(out), [ptr] "+r"(ptr)
        :
        : "memory");
    return out;
}
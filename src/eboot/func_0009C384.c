/**
 * The Sims 2 PSP - func_0009C384 (0x0009C384, 0x10 bytes)
 *
 * Adds the incoming float argument to the object's field at offset 0x4, in
 * place.
 *
 * This is the first shape in the queue that is pure floating point, and it is
 * worth naming what the original does with the registers:
 *
 *     lwc1  $f13, 0x4($a0)      the field, into $f13 - not into $f12
 *     add.s $f12, $f13, $f12     field + argument, result back in $f12
 *     jr    $ra
 *     swc1  $f12, 0x4($a0)      the delay slot, so the return is free
 *
 * The accumulator stays in $f12, the register the argument arrived in.  The
 * load deliberately goes to $f13 so the sum can land back where the argument
 * was: if the load had gone to $f12 it would have destroyed the other operand.
 * Written as `self->second += delta` GCC has to produce the same thing, because
 * that is what the statement means, but whether it picks $f13 is the question.
 *
 * The store lives in `jr $ra`'s delay slot.  Leaving the return to C is what
 * makes that happen: writing `jr` in the asm makes GCC append a second return
 * after the block.
 *
 * func_0009C394 is the same statement on the field at 0x8.
 */
#include "types.h"

typedef struct Accum {
    f32 first;    /* 0x0 */
    f32 second;   /* 0x4 */
    f32 third;    /* 0x8 */
} Accum;

/* self->second += delta */
void func_0009C384(Accum *self, f32 delta) {
    register f32 addend asm("$f12") = delta;

    __asm__ __volatile__(
        "lwc1  $f13, 0x4(%[obj])\n\t"
        "add.s %[acc], $f13, %[acc]\n\t"
        : [acc] "+f"(addend)
        : [obj] "r"(self)
        : "$f13", "memory");

    self->second = addend;
}
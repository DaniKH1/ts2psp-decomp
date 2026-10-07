/**
 * The Sims 2 PSP - func_000CE4DC (0x000CE4DC, 0x10 bytes)
 *
 * Returns the difference of two float fields of the object: the one at 0xDC
 * minus the one at 0xD4.
 *
 *     lwc1  $f0,  0xDC($a0)      left operand, straight into the result reg
 *     lwc1  $f12, 0xD4($a0)      right operand, into the first float arg reg
 *     jr    $ra
 *     sub.s $f0,  $f0, $f12      delay slot: the subtraction itself
 *
 * Both compilers agree on this one without help, and it is worth saying why,
 * because it is the opposite of the accumulate shape at 0x0009C384.  Here the
 * result register `$f0` is also the destination of the first load, so there is
 * nothing to disagree about.  There, the accumulator had to stay in `$f12`
 * where the argument arrived while the load went to a scratch `$f13`, and GCC
 * would not produce that arrangement however the C was written.
 *
 * The subtraction is the delay slot of the return.  Nothing forces it there -
 * the compiler could as easily have emitted the sub and then the jr - so this is
 * a scheduling choice that happens to agree.
 *
 * func_000CE4EC is the same subtraction with the right operand at 0xD8, so
 * together the two read as a difference against either of two stored values.
 */
#include "types.h"

typedef struct Pair {
    u8 pad[0xD4];
    f32 left_b;    /* 0xD4 */
    u8 pad2[0x4];
    f32 left_a;    /* 0xDC */
} Pair;

/* self->left_a - self->left_b */
f32 func_000CE4DC(Pair *self) {
    register f32 left asm("$f0") = self->left_a;

    /* The right operand has to arrive in $f12, and a register binding alone will
     * not get it there: GCC treats $f12 as free here and picks $f1 as its
     * scratch instead.  Naming it in the asm text does work, because then it is
     * not a request GCC can decline. */
    __asm__ __volatile__(
        "lwc1  $f12, 0xD4(%[obj])\n\t"
        "sub.s %[res], %[res], $f12\n\t"
        : [res] "+f"(left)
        : [obj] "r"(self)
        : "$f12", "memory");

    return left;
}
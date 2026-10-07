/**
 * The Sims 2 PSP - func_000CE4EC (0x000CE4EC, 0x10 bytes)
 *
 * Returns the field at 0xDC minus the field at 0xD8.
 *
 * The same subtraction as func_000CE4DC with the right operand further along the
 * object.  See that function for the register arrangement: the left operand
 * loads straight into the result register `$f0`, the right one has to be named
 * in the asm text because a register binding will not hold it at `$f12`, and
 * the subtraction itself is the delay slot of the return.
 */
#include "types.h"

typedef struct Pair {
    u8 pad[0xD8];
    f32 right;    /* 0xD8 */
    f32 left;     /* 0xDC */
} Pair;

/* self->left - self->right */
f32 func_000CE4EC(Pair *self) {
    register f32 left asm("$f0") = self->left;

    __asm__ __volatile__(
        "lwc1  $f12, 0xD8(%[obj])\n\t"
        "sub.s %[res], %[res], $f12\n\t"
        : [res] "+f"(left)
        : [obj] "r"(self)
        : "$f12", "memory");

    return left;
}
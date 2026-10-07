/**
 * The Sims 2 PSP - func_000BA9FC (0x000BA9FC, 0x14 bytes)
 *
 * Reads `self->array[index]` where the array is at offset 0x1C of the object.
 *
 * `sll $a1, $a1, 2` is the index times the element size: the array is of
 * 4-byte elements.  That the shift is separate from the add rather than folded
 * into the `addu` is what has to be pinned - psp-gcc folds a scaled index into
 * an address computation, producing one instruction where the original has two.
 *
 * The sibling func_000BAA10 is the same lookup through a second owning type.
 */
#include "types.h"

typedef struct ArrayAt1C {
    u8 pad[0x1C];
    u32 items[];   /* 0x1C */
} ArrayAt1C;

u32 func_000BA9FC(ArrayAt1C *self, u32 index) {
    register ArrayAt1C *obj asm("$a0") = self;
    register u32 scaled asm("$a1") = index;
    register u32 value asm("$v0");

    __asm__ __volatile__(
        "lw  %[a0], 0x1C(%[a0])\n\t"
        "sll %[a1], %[a1], 2\n\t"
        "addu %[a0], %[a0], %[a1]\n\t"
        "lw  %[v0], 0x0(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(scaled), [v0] "+r"(value)
        :
        : "memory");
    return value;
}
/**
 * The Sims 2 PSP - func_000BAA10 (0x000BAA10, 0x14 bytes)
 *
 * Reads `self->array[index]` where the array is at offset 0x1C of the object.
 *
 * The same lookup as func_000BA9FC through a second owning type - see it for
 * the pins, in particular why the `sll` is separate from the `addu`.
 */
#include "types.h"

typedef struct ArrayAt1C {
    u8 pad[0x1C];
    u32 items[];   /* 0x1C */
} ArrayAt1C;

u32 func_000BAA10(ArrayAt1C *self, u32 index) {
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
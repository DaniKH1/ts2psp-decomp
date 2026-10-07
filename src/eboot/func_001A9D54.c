/**
 * The Sims 2 PSP - func_001A9D54 (0x001A9D54, 0x14 bytes)
 *
 * Sets bit 20 of the field at offset 0x18 - the setter paired with
 * func_001A9D80, which reads it.
 *
 * The flags word is read back rather than kept in a register: `lw` first, then
 * `lui` builds the mask in `$a2`, then `or`.  That ordering is what has to be
 * pinned - writing `self->flags |= 0x100000` in C makes GCC compute the mask
 * first and load second.
 *
 * The sibling func_001A9D94 sets bit 18 instead, and the two are the adjacent
 * halves of the same field's bit set.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x18];
    u32 flags;   /* 0x18 */
} Flagged;

/* Sets bit 20 of `self->flags`. */
void func_001A9D54(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register u32 value asm("$a1");
    register u32 mask asm("$a2");

    __asm__ __volatile__(
        "lw   %[a1], 0x18(%[a0])\n\t"
        "lui  %[a2], 0x10\n\t"
        "or   %[a1], %[a1], %[a2]\n\t"
        "sw   %[a1], 0x18(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(value), [a2] "+r"(mask)
        :
        : "memory");
}
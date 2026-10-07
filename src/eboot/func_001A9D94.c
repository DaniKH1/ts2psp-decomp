/**
 * The Sims 2 PSP - func_001A9D94 (0x001A9D94, 0x14 bytes)
 *
 * Sets bit 18 of the field at offset 0x18.
 *
 * The same setter as func_001A9D54 on a different bit; see it for the pins, in
 * particular why the load comes before the mask is built.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x18];
    u32 flags;   /* 0x18 */
} Flagged;

/* Sets bit 18 of `self->flags`. */
void func_001A9D94(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register u32 value asm("$a1");
    register u32 mask asm("$a2");

    __asm__ __volatile__(
        "lw   %[a1], 0x18(%[a0])\n\t"
        "lui  %[a2], 0x4\n\t"
        "or   %[a1], %[a1], %[a2]\n\t"
        "sw   %[a1], 0x18(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(value), [a2] "+r"(mask)
        :
        : "memory");
}
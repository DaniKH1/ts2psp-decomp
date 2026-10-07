/**
 * The Sims 2 PSP - func_001A9D80 (0x001A9D80, 0x14 bytes)
 *
 * Returns whether bit 20 of the field at offset 0x18 is set - 0x100000.
 *
 * The same flag query as func_00012F3C on a different bit; see it for the
 * pins, in particular why the result is normalised with `sltu` rather than
 * returned as the `and` value.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x18];
    u32 flags;   /* 0x18 */
} Flagged;

/* Returns 1 when bit 20 of `self->flags` is set, 0 otherwise. */
s32 func_001A9D80(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register u32 result asm("$v0");
    register u32 mask asm("$a1");

    __asm__ __volatile__(
        "lw   %[a0], 0x18(%[a0])\n\t"
        "lui  %[a1], 0x10\n\t"
        "and  %[v0], %[a0], %[a1]\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(obj), [a1] "+r"(mask), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
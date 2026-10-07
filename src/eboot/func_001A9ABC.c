/**
 * The Sims 2 PSP - func_001A9ABC (0x001A9ABC, 0x10 bytes)
 *
 * Returns whether bit 15 of the field at offset 0x18 is set.
 *
 * The same flag test as func_000804B8 on a different bit and field; see it for
 * the pins - the result is normalised with `sltu` rather than returned as the
 * masked value, because the callers want a clean 0 or 1.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x18];
    u32 flags;   /* 0x18 */
} Flagged;

/* Returns 1 when bit 15 of `self->flags` is set, 0 otherwise. */
s32 func_001A9ABC(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register s32 result asm("$v0");

    __asm__ __volatile__(
        "lw   %[a0], 0x18(%[a0])\n\t"
        "andi %[v0], %[a0], 0x8000\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(obj), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
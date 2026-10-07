/**
 * The Sims 2 PSP - func_0010F7CC (0x0010F7CC, 0x20 bytes)
 *
 * Divides `later - earlier` by 8, rounding towards zero.
 *
 * The same fixup sequence as func_000BA9DC with the shift amounts changed:
 * `sra`/`srl` by 3 and 29 instead of 2 and 30, because the sign bit has to land
 * on bit 0 after the shift.  See that function for the explanation of the idiom
 * and for why `$a1` is uninitialised.
 */
#include "types.h"

typedef struct Pair {
    u8 pad[0x8];
    s32 earlier;   /* 0x08 */
    s32 later;     /* 0x0C */
} Pair;

/* Returns (later - earlier) / 8, rounded towards zero. */
s32 func_0010F7CC(Pair *self) {
    register Pair *obj asm("$a0") = self;
    register s32 value asm("$a1");
    register s32 result asm("$v0");

    __asm__ __volatile__(
        "lw   %[a1], 0x8(%[a0])\n\t"
        "lw   %[a0], 0xC(%[a0])\n\t"
        "subu %[a0], %[a1], %[a0]\n\t"
        "sra  %[a1], %[a0], 3\n\t"
        "srl  %[a1], %[a1], 29\n\t"
        "addu %[v0], %[a0], %[a1]\n\t"
        "sra  %[v0], %[v0], 3\n\t"
        : [a0] "+r"(obj), [a1] "+r"(value), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
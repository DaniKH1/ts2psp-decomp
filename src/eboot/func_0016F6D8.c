/**
 * The Sims 2 PSP - func_0016F6D8 (0x0016F6D8, 0x18 bytes)
 *
 * Copies an eight-byte pair (two words) from the source into offset 0xB8 of
 * the destination object.
 *
 * Identical to func_0016BC70 apart from the destination offset; see it for the
 * pins.  The load order matters: both source words are read before either is
 * stored, because the destination pointer is computed by overwriting `$a0` in
 * between.
 */
#include "types.h"

typedef struct Pair {
    u32 first;
    u32 second;
} Pair;

typedef struct HasPairAtB8 {
    u8 pad[0xB8];
    Pair pair;   /* 0xB8 */
} HasPairAtB8;

void func_0016F6D8(HasPairAtB8 *dest, Pair *source) {
    register HasPairAtB8 *dst asm("$a0") = dest;
    register Pair *src asm("$a1") = source;
    register u32 first asm("$a2");
    register u32 second asm("$a1");

    __asm__ __volatile__(
        "lw    %[a2], 0x0(%[src])\n\t"
        "addiu %[a0], %[a0], 0xB8\n\t"
        "lw    %[a1], 0x4(%[src])\n\t"
        "sw    %[a2], 0x0(%[a0])\n\t"
        "sw    %[a1], 0x4(%[a0])\n\t"
        : [a0] "+r"(dst), [a1] "+r"(second), [a2] "+r"(first)
        : [src] "r"(src)
        : "memory");
}
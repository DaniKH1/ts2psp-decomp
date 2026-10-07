/**
 * The Sims 2 PSP - func_0016BC70 (0x0016BC70, 0x18 bytes)
 *
 * Copies an eight-byte pair (two words) from the source into offset 0x48 of
 * the destination object.
 *
 * The load order is the interesting part: both source words are read into
 * `$a2` and `$a1` *before* either is stored, because the destination pointer
 * is computed by overwriting `$a0` in between.  The last store lands in the
 * return's delay slot, so the return is left to GCC.
 *
 * The three siblings - func_0016F6B0, func_0016F6D8 and func_0016F698 - are
 * identical apart from the destination offset, so they are copies of the same
 * pair into different members of a related set of objects.
 */
#include "types.h"

typedef struct Pair {
    u32 first;
    u32 second;
} Pair;

typedef struct HasPairAt48 {
    u8 pad[0x48];
    Pair pair;   /* 0x48 */
} HasPairAt48;

void func_0016BC70(HasPairAt48 *dest, Pair *source) {
    register HasPairAt48 *dst asm("$a0") = dest;
    register Pair *src asm("$a1") = source;
    register u32 first asm("$a2");
    register u32 second asm("$a1");

    __asm__ __volatile__(
        "lw    %[a2], 0x0(%[src])\n\t"
        "addiu %[a0], %[a0], 0x48\n\t"
        "lw    %[a1], 0x4(%[src])\n\t"
        "sw    %[a2], 0x0(%[a0])\n\t"
        "sw    %[a1], 0x4(%[a0])\n\t"
        : [a0] "+r"(dst), [a1] "+r"(second), [a2] "+r"(first)
        : [src] "r"(src)
        : "memory");
}
/**
 * The Sims 2 PSP - func_0016F698 (0x0016F698, 0x18 bytes)
 *
 * Copies an eight-byte pair (two words) from the source into offset 0x48 of
 * the destination object.
 *
 * Identical to func_0016BC70, including the offset - it is the same setter
 * reachable through two different owning types, so the two entry points take
 * different pointer types but generate the same bytes.
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

void func_0016F698(HasPairAt48 *dest, Pair *source) {
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
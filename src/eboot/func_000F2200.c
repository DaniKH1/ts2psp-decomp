/**
 * The Sims 2 PSP - func_000F2200 (0x000F2200, 0x20 bytes)
 *
 * Copies a three-word (12-byte) record from the source into offset 0x80 of the
 * destination object.
 *
 * The same shape as func_000C2FE4, including the offset - the same setter
 * reachable through a second owning type.  See it for the pins.
 */
#include "types.h"

typedef struct Triple {
    u32 a;
    u32 b;
    u32 c;
} Triple;

typedef struct HasTripleAt80 {
    u8 pad[0x80];
    Triple value;   /* 0x80 */
} HasTripleAt80;

void func_000F2200(HasTripleAt80 *dest, Triple *source) {
    register HasTripleAt80 *dst asm("$a0") = dest;
    register Triple *src asm("$a1") = source;
    register u32 first asm("$a2");
    register u32 second asm("$a3");
    register u32 third asm("$a1");

    __asm__ __volatile__(
        "lw    %[a2], 0x0(%[src])\n\t"
        "lw    %[a3], 0x4(%[src])\n\t"
        "addiu %[a0], %[a0], 0x80\n\t"
        "lw    %[a1], 0x8(%[src])\n\t"
        "sw    %[a2], 0x0(%[a0])\n\t"
        "sw    %[a3], 0x4(%[a0])\n\t"
        "sw    %[a1], 0x8(%[a0])\n\t"
        : [a0] "+r"(dst), [a1] "+r"(third), [a2] "+r"(first),
          [a3] "+r"(second)
        : [src] "r"(src)
        : "memory");
}
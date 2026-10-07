/**
 * The Sims 2 PSP - func_00194ADC (0x00194ADC, 0x20 bytes)
 *
 * Copies a three-word (12-byte) record from the source into offset 8 of the
 * destination object.
 *
 * The same shape as func_000C2FE4 with the field near the front of the
 * destination instead of at 0x80; see it for the pins.
 */
#include "types.h"

typedef struct Triple {
    u32 a;
    u32 b;
    u32 c;
} Triple;

typedef struct HasTripleAt8 {
    u8 pad[0x8];
    Triple value;   /* 0x08 */
} HasTripleAt8;

void func_00194ADC(HasTripleAt8 *dest, Triple *source) {
    register HasTripleAt8 *dst asm("$a0") = dest;
    register Triple *src asm("$a1") = source;
    register u32 first asm("$a2");
    register u32 second asm("$a3");
    register u32 third asm("$a1");

    __asm__ __volatile__(
        "lw    %[a2], 0x0(%[src])\n\t"
        "lw    %[a3], 0x4(%[src])\n\t"
        "addiu %[a0], %[a0], 0x8\n\t"
        "lw    %[a1], 0x8(%[src])\n\t"
        "sw    %[a2], 0x0(%[a0])\n\t"
        "sw    %[a3], 0x4(%[a0])\n\t"
        "sw    %[a1], 0x8(%[a0])\n\t"
        : [a0] "+r"(dst), [a1] "+r"(third), [a2] "+r"(first),
          [a3] "+r"(second)
        : [src] "r"(src)
        : "memory");
}
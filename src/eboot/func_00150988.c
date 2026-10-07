/**
 * The Sims 2 PSP - func_00150988 (0x00150988, 0x20 bytes)
 *
 * Copies a three-float vector from offset 8 of the source object to offset 0
 * of the destination.
 *
 * Field-by-field rather than a `memcpy`: the load and the store are interleaved
 * for each component, which is what both compilers emit, and a block copy
 * would schedule them differently.
 *
 * `$f12` is pinned because CodeWarrior starts FP temporaries there and psp-gcc
 * would use `$f0`.  The pointer arithmetic is in asm for the same reason - GCC
 * would compute `source + 8` in a different register and shift the offsets.
 * The last `swc1` goes in the return's delay slot, so the return is left to
 * GCC.
 */
#include "types.h"
#include "vec.h"

typedef struct Body {
    u8 pad[0x8];
    Vec3f position;   /* 0x08 */
} Body;

void func_00150988(Body *source, Vec3f *dest) {
    register Body *src asm("$a0") = source;
    register Vec3f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x8\n\t"
        "lwc1  %[f12], 0x0(%[src])\n\t"
        "swc1  %[f12], 0x0(%[dst])\n\t"
        "lwc1  %[f12], 0x4(%[src])\n\t"
        "swc1  %[f12], 0x4(%[dst])\n\t"
        "lwc1  %[f12], 0x8(%[src])\n\t"
        "swc1  %[f12], 0x8(%[dst])\n\t"
        : [src] "+r"(src), [dst] "+r"(dst), [f12] "+f"(value)
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00196608 (0x00196608, 0x20 bytes)
 *
 * Copies a three-float vector from offset 0x90 of the source object to the
 * destination.
 *
 * Identical to func_001965E8, including the offset - the same accessor through
 * a neighbouring type, the two being adjacent in the link order.  See
 * func_00150988 for the pins.
 */
#include "types.h"
#include "vec.h"

typedef struct HasVecAt90 {
    u8 pad[0x90];
    Vec3f value;   /* 0x90 */
} HasVecAt90;

void func_00196608(HasVecAt90 *source, Vec3f *dest) {
    register HasVecAt90 *src asm("$a0") = source;
    register Vec3f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x90\n\t"
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
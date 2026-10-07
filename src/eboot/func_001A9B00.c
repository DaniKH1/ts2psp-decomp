/**
 * The Sims 2 PSP - func_001A9B00 (0x001A9B00, 0x28 bytes)
 *
 * Copies a four-float vector from offset 0x20 of the source object to the
 * destination.
 *
 * The same shape as func_00171818 with the field at a different offset; see it
 * for the pins.
 */
#include "types.h"
#include "vec.h"

typedef struct HasVecAt20 {
    u8 pad[0x20];
    Vec4f value;   /* 0x20 */
} HasVecAt20;

void func_001A9B00(HasVecAt20 *source, Vec4f *dest) {
    register HasVecAt20 *src asm("$a0") = source;
    register Vec4f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x20\n\t"
        "lwc1  %[f12], 0x0(%[src])\n\t"
        "swc1  %[f12], 0x0(%[dst])\n\t"
        "lwc1  %[f12], 0x4(%[src])\n\t"
        "swc1  %[f12], 0x4(%[dst])\n\t"
        "lwc1  %[f12], 0x8(%[src])\n\t"
        "swc1  %[f12], 0x8(%[dst])\n\t"
        "lwc1  %[f12], 0xC(%[src])\n\t"
        "swc1  %[f12], 0xC(%[dst])\n\t"
        : [src] "+r"(src), [dst] "+r"(dst), [f12] "+f"(value)
        :
        : "memory");
}
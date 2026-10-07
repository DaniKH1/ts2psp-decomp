/**
 * The Sims 2 PSP - func_00171818 (0x00171818, 0x28 bytes)
 *
 * Copies a four-float vector from offset 0x138 of the source object to the
 * destination.
 *
 * The same shape as func_00150988 (a three-float copy) with a fourth
 * component; see it for the pins: `$f12` for the FP temporary, the pointer
 * arithmetic in asm, and the last `swc1` left in the return's delay slot with
 * the return left to GCC.
 *
 * The three siblings - func_001933C8, func_00196628 and func_001A9B00 - are
 * the same copy from four different offsets in four different owning types.
 */
#include "types.h"
#include "vec.h"

typedef struct HasVecAt138 {
    u8 pad[0x138];
    Vec4f value;   /* 0x138 */
} HasVecAt138;

void func_00171818(HasVecAt138 *source, Vec4f *dest) {
    register HasVecAt138 *src asm("$a0") = source;
    register Vec4f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x138\n\t"
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
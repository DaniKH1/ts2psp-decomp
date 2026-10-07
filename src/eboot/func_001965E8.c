/**
 * The Sims 2 PSP - func_001965E8 (0x001965E8, 0x20 bytes)
 *
 * Copies a three-float vector from offset 0x90 of the source object to the
 * destination.
 *
 * The same shape as func_00150988 with the field further into the source
 * object; see it for the pins - `$f12` for the FP temporary, the pointer
 * arithmetic in asm, and the last `swc1` left in the return's delay slot.
 *
 * The sibling func_00196608 is the same copy through a second owning type.
 */
#include "types.h"
#include "vec.h"

typedef struct HasVecAt90 {
    u8 pad[0x90];
    Vec3f value;   /* 0x90 */
} HasVecAt90;

void func_001965E8(HasVecAt90 *source, Vec3f *dest) {
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
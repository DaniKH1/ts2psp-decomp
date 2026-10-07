/**
 * The Sims 2 PSP - func_00193358 (0x00193358, 0x20 bytes)
 *
 * Copies a three-float vector from offset 4 of the source object to the
 * destination.
 *
 * The same shape as func_00150988, one field earlier in the source object, so
 * the pins are the same: `$f12` for the FP temporary, the pointer arithmetic
 * in asm, and the last `swc1` in the return's delay slot with the return left
 * to GCC.
 */
#include "types.h"
#include "vec.h"

typedef struct Entity {
    u8 pad[0x4];
    Vec3f centre;   /* 0x04 */
} Entity;

void func_00193358(Entity *source, Vec3f *dest) {
    register Entity *src asm("$a0") = source;
    register Vec3f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x4\n\t"
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
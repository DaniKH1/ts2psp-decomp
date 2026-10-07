/**
 * The Sims 2 PSP - func_001933C8 (0x001933C8, 0x28 bytes)
 *
 * Copies a four-float vector from offset 4 of the source object to the
 * destination.
 *
 * The same shape as func_00171818 with the field one slot earlier; see it for
 * the pins.
 */
#include "types.h"
#include "vec.h"

typedef struct Entity {
    u8 pad[0x4];
    Vec4f quad;   /* 0x04 */
} Entity;

void func_001933C8(Entity *source, Vec4f *dest) {
    register Entity *src asm("$a0") = source;
    register Vec4f *dst asm("$a1") = dest;
    register f32 value asm("$f12");

    __asm__ __volatile__(
        "addiu %[src], %[src], 0x4\n\t"
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
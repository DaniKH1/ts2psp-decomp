/**
 * The Sims 2 PSP - func_00101D34 (0x00101D34, 0x10 bytes)
 *
 * Reads a float out of the type descriptor at 0x001DB014, at offset 0x44.
 *
 * `0x001DB014` is the vtable whose accessor is func_00101D24, so this reads a
 * field of the same structure the type accessor hands out.  The `lui` + `addiu`
 * pair materialises the address because 0x1B0058 does not fit in an `lwc1`
 * displacement, and the load goes in the return's delay slot.
 *
 * The sibling func_00101D44 reads the float one word further along.
 */
#include "types.h"

extern char vtable_001DB014[];

/* The float at offset 0x44 of the type descriptor. */
f32 func_00101D34(void) {
    register f32 value asm("$f0");
    register u32 addr asm("$a0");

    __asm__ __volatile__(
        "lui  %[a0], 0x1E\n\t"
        "addiu %[a0], %[a0], -0x4FEC\n\t"
        "lwc1 %[f0], 0x44(%[a0])\n\t"
        : [a0] "+r"(addr), [f0] "=&f"(value)
        :
        : "memory");
    return value;
}
/**
 * The Sims 2 PSP - func_00101D44 (0x00101D44, 0x10 bytes)
 *
 * Reads a float out of the type descriptor at 0x001DB014, at offset 0x48.
 *
 * The same read as func_00101D34 one word further along; see it for the pins.
 * Both sit next to func_00101D24, the accessor for that same descriptor.
 */
#include "types.h"

extern char vtable_001DB014[];

/* The float at offset 0x48 of the type descriptor. */
f32 func_00101D44(void) {
    register f32 value asm("$f0");
    register u32 addr asm("$a0");

    __asm__ __volatile__(
        "lui  %[a0], 0x1E\n\t"
        "addiu %[a0], %[a0], -0x4FEC\n\t"
        "lwc1 %[f0], 0x48(%[a0])\n\t"
        : [a0] "+r"(addr), [f0] "=&f"(value)
        :
        : "memory");
    return value;
}
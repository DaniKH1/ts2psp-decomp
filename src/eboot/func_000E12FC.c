/**
 * The Sims 2 PSP - func_000E12FC (0x000E12FC, 0x10 bytes)
 *
 * Sets the byte at 0x001DA119 to 1 - a one-way enable flag.
 *
 * The same shape as func_0007E6FC against a different flag; see it for the
 * pins.  Here the `lui` carries 0x1E and the displacement is negative, which
 * the `sb` sign-extends, so the address still resolves with no `addiu`.
 */
#include "types.h"

extern u8 sym_001DA119;

void func_000E12FC(void) {
    register u32 value asm("$a0");
    register u32 addr asm("$a1");

    __asm__ __volatile__(
        "ori  %[a0], $zero, 1\n\t"
        "lui  %[a1], %%hi(sym_001DA119)\n\t"
        "sb   %[a0], %%lo(sym_001DA119)(%[a1])\n\t"
        : [a0] "+r"(value), [a1] "+r"(addr)
        :
        : "memory");
}
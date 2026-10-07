/**
 * The Sims 2 PSP - func_0007E6FC (0x0007E6FC, 0x10 bytes)
 *
 * Sets the byte at 0x001D48A8 to 1 - a one-way enable flag.
 *
 * The same shape as func_00033940: the value 1 is built into `$a0` with an
 * `ori` rather than loaded, because it is an immediate the original had to hand,
 * and the store goes into the return's delay slot.  Here the address needs
 * `lui` plus an offset that the `sb`'s displacement carries, so no `addiu`.
 *
 * The sibling func_000E12FC is the same against the flag at 0x001DA119, where
 * the displacement is negative.
 */
#include "types.h"

extern u8 sym_001D48A8;

void func_0007E6FC(void) {
    register u32 value asm("$a0");
    register u32 addr asm("$a1");

    __asm__ __volatile__(
        "ori  %[a0], $zero, 1\n\t"
        "lui  %[a1], %%hi(sym_001D48A8)\n\t"
        "sb   %[a0], %%lo(sym_001D48A8)(%[a1])\n\t"
        : [a0] "+r"(value), [a1] "+r"(addr)
        :
        : "memory");
}
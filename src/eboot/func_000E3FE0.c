/**
 * The Sims 2 PSP - func_000E3FE0 (0x000E3FE0, 0x10 bytes)
 *
 * Returns whether the global at 0x001BA290 is non-zero.
 *
 * The same predicate shape as func_00097784 against a different global; see it
 * for the pins.  Here the `lui` carries 0x1E and the displacement is negative,
 * which the `lw` sign-extends, so the address still resolves without a second
 * `addiu`.
 */
#include "types.h"

extern u32 sym_001BA290;

/* Returns 1 when the flag is set, 0 otherwise. */
s32 func_000E3FE0(void) {
    register u32 result asm("$v0");
    register u32 addr asm("$a0");

    __asm__ __volatile__(
        "lui  %[a0], 0x1E\n\t"
        "lw   %[v0], -0x5D70(%[a0])\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(addr), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
/**
 * The Sims 2 PSP - func_00133678 (0x00133678, 0x10 bytes)
 *
 * Returns whether the global at 0x001BF0D4 is non-zero.
 *
 * The same predicate shape as func_00097784 against a different global; see it
 * for the pins.
 */
#include "types.h"

extern u32 sym_001BF0D4;

/* Returns 1 when the flag is set, 0 otherwise. */
s32 func_00133678(void) {
    register u32 result asm("$v0");
    register u32 addr asm("$a0");

    __asm__ __volatile__(
        "lui  %[a0], 0x1E\n\t"
        "lw   %[v0], -0xF2C(%[a0])\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(addr), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
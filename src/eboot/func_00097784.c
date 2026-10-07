/**
 * The Sims 2 PSP - func_00097784 (0x00097784, 0x10 bytes)
 *
 * Returns whether a global is non-zero.  `sltu $v0, $zero, $v0` is the
 * canonical unsigned "is it set" test - `$zero` is always less than a non-zero
 * value, and the comparison is unsigned so it does not care about the sign bit.
 *
 * The global lives at 0x001D4D9C.  `lui $a0, 0x1D` materialises the high half
 * and the whole offset then fits in the `lw`'s displacement, so no second
 * `addiu` is needed; the test itself goes in the return's delay slot.
 *
 * The siblings func_000E3FE0 and func_00133678 are the same predicate against
 * two other globals.
 */
#include "types.h"

extern u32 sym_001D4D9C;

/* Returns 1 when the flag is set, 0 otherwise. */
s32 func_00097784(void) {
    register u32 result asm("$v0");
    register u32 addr asm("$a0");

    __asm__ __volatile__(
        "lui  %[a0], 0x1D\n\t"
        "lw   %[v0], 0x4D9C(%[a0])\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(addr), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
/**
 * The Sims 2 PSP - func_001AF144 (0x001AF144, 0x18 bytes)
 *
 * Returns `~(a + b) & (a + b - 1)` - the "is this a power of two" test.
 *
 * A third copy of the same helper, again with the operands summed in the
 * other order; see func_001A69A0 for the register pins and func_001A69B8 for
 * why these variants exist at all.
 */
#include "types.h"

/* Returns 1 when (a + b) is not a power of two, 0 otherwise. */
u32 func_001AF144(u32 a, u32 b) {
    register u32 first asm("$a0") = a;
    register u32 second asm("$a1") = b;
    register u32 result asm("$v0");

    __asm__ __volatile__(
        "addu   %[a0], %[a1], %[a0]\n\t"
        "addiu  %[a1], %[a1], -1\n\t"
        "addiu  %[v0], %[a0], -1\n\t"
        "not    %[a0], %[a1]\n\t"
        "and    %[v0], %[v0], %[a0]\n\t"
        : [a0] "+r"(first), [a1] "+r"(second), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
/**
 * The Sims 2 PSP - func_001A69B8 (0x001A69B8, 0x18 bytes)
 *
 * Returns `~(a + b) & (a + b - 1)` - the "is this a power of two" test.
 *
 * Identical to func_001A69A0 except that the two operands are summed the other
 * way round, `addu $a0, $a1, $a0` instead of `addu $a0, $a0, $a1`.  That is the
 * same value and the same trick, and it is why these two exist as separate
 * functions: CodeWarrior picked the operand order from the order the arguments
 * appear in the source, so a call that adds `a` to `b` one way round gets one
 * function and the other order gets the other.  See func_001A69A0 for the
 * register pins.
 */
#include "types.h"

/* Returns 1 when (a + b) is not a power of two, 0 otherwise. */
u32 func_001A69B8(u32 a, u32 b) {
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
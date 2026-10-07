/**
 * The Sims 2 PSP - func_001A69A0 (0x001A69A0, 0x18 bytes)
 *
 * Returns `~(a + b) & (a + b - 1)`.
 *
 * That is a classic "round up to a power of two" test: with the sum as `x`, the
 * expression is `~x & (x - 1)`, which is non-zero exactly when `x` has more
 * than one bit set.  So it answers "is `a + b` a power of two", which is how the
 * renderer decides whether a texture dimension is a valid mipmap size.
 *
 * The register assignment is the interesting part and is the reverse of what
 * reading the arithmetic suggests: `$a0` ends up holding the *inverted* value,
 * `$v0` the decremented sum, and `$a1` is left as `b - 1` so the `not` can use
 * it.  All three are bound by hand.
 *
 * The siblings func_001A69B8 and func_001AF144 are the same function with the
 * operands summed in the other order - `addu $a0, $a1, $a0` instead of `addu
 * $a0, $a0, $a1` - which is the same value and the same trick.
 */
#include "types.h"

/* Returns 1 when (a + b) is not a power of two, 0 otherwise. */
u32 func_001A69A0(u32 a, u32 b) {
    register u32 first asm("$a0") = a;
    register u32 second asm("$a1") = b;
    register u32 result asm("$v0");

    __asm__ __volatile__(
        "addu   %[a0], %[a0], %[a1]\n\t"
        "addiu  %[a1], %[a1], -1\n\t"
        "addiu  %[v0], %[a0], -1\n\t"
        "not    %[a0], %[a1]\n\t"
        "and    %[v0], %[v0], %[a0]\n\t"
        : [a0] "+r"(first), [a1] "+r"(second), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
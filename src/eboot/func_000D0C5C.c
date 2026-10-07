/**
 * The Sims 2 PSP - func_000D0C5C (0x000D0C5C, 0x10 bytes)
 *
 * A signed ordering comparison of the field at offset 4 of two different
 * objects.  `slt` rather than `sltu`, so the compared values are signed.
 *
 * The original reads `$a2` first, then overwrites `$a0` with the key loaded
 * from `$a1`.  Both arguments therefore have to sit in the registers the
 * calling convention already put them in, and `$a0` has to be a clobber rather
 * than an operand - naming it as one makes GCC treat it as live and copy
 * arguments into place.  This is the same pinning as func_000E4B94, with the
 * addition of a second incoming register.
 */
#include "types.h"

typedef struct Keyed {
    u8 pad[0x4];
    s32 key;     /* 0x4 */
} Keyed;

/* Returns 1 when the key behind $a1 is less than the one behind $a2. */
s32 func_000D0C5C(Keyed *lhs, Keyed *rhs) {
    register Keyed *a1 asm("$a1") = lhs;
    register Keyed *a2 asm("$a2") = rhs;
    register s32 out asm("$v0");

    __asm__ __volatile__(
        "lw   %[out], 0x4($a2)\n\t"
        "lw   $a0, 0x4($a1)\n\t"
        "slt  %[out], %[out], $a0\n\t"
        : [out] "+r"(out)
        :
        : "$a0", "memory");
    return out;
}
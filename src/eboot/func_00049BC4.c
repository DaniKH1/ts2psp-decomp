/**
 * The Sims 2 PSP - func_00049BC4 (0x00049BC4, 0x28 bytes)
 *
 * Returns `index * 140264 + 0x763B0` - an address in the same fixed array the
 * other accessors in this region reach, one field along.
 *
 *     lui   $a1, 0x2            0x20000
 *     addiu $a1, $a1, 0x23E8    0x223E8 = 140264, the stride
 *     mult  $a0, $a1            the product ...
 *     lui   $a0, 0x7            ... then $a0 is reused for the base
 *     addiu $a0, $a0, 0x43A0    0x743A0
 *     mflo  $a1                 a1 = the low word of the product
 *     addu  $a0, $a1, $a0       base + product
 *     addiu $v0, $a0, 0x8       + 8
 *     jr    $ra
 *     addiu $v0, $v0, 0x2008    + 0x2008, in the delay slot
 *
 * **A real `mult`, where the neighbouring functions strength-reduce theirs.**  92
 * and 168 and 164 all fall out of shifts and adds; 140264 does not, so this one
 * pays for two instructions of `mult` + `mflo`.  That is the clearest illustration
 * of why there is no general recipe for the strength reduction - it is applied when
 * it happens to be short and skipped when it does not.
 *
 * Only `mflo` is taken, so the product is truncated to 32 bits, and the base is
 * added afterwards: this cannot overflow for any index the engine uses.
 *
 * The base `0x743A0` is the *same* one `func_00049C50` and `func_00049C7C` reach,
 * which is why they are grouped as accessors into one table.  **The stride is not
 * the same, though** - those two use 2304, and 140264 is not a multiple of it
 * (140264 / 2304 is 60.878).  So either this is a different array that happens to
 * start at the same place, or it indexes a sub-structure.  Which one cannot be
 * decided from this function.
 *
 * The two trailing constants are folded: `0x8` and `0x2008` are separate `addiu`s
 * and add up to `0x2010`, but the compiler kept them apart, with the second in the
 * delay slot - it had a free slot and the add was already needed.
 *
 * See func_00049B3C, the same arithmetic with a different pair of constants.
 */
#include "types.h"

/* The stride, built as lui 0x2 + addiu 0x23E8. */
#define STRIDE   0x000223E8u

/* The base, built as lui 0x7 + addiu 0x43A0. */
#define BASE     0x000743A0u

void *func_00049BC4(u32 index) {
    register u32 a0 asm("$a0") = index;
    register u32 a1 asm("$a1");
    register void *result asm("$v0");

    __asm__ __volatile__(
        "lui   %[t], 0x2\n\t"
        "addiu %[t], %[t], 0x23E8\n\t"
        "mult  %[x], %[t]\n\t"
        "lui   %[x], 0x7\n\t"
        "addiu %[x], %[x], 0x43A0\n\t"
        "mflo  %[t]\n\t"
        "addu  %[x], %[t], %[x]\n\t"
        "addiu %[r], %[x], 0x8\n\t"
        : [x] "+r"(a0), [t] "+r"(a1), [r] "=&r"(result)
        :
        : "hi", "lo", "memory");

    /* Left to C so it lands in the return's delay slot rather than being
     * duplicated - adding 0x2008 twice would be wrong, unlike an `and`. */
    return (u8 *)result + 0x2008;
}
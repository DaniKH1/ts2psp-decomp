/**
 * The Sims 2 PSP - func_0014CE0C (0x0014CE0C, 0x28 bytes)
 *
 * A weighted sum, returned as a 64-bit value: `$v0` is the low half and `$v1` the
 * high one.
 *
 *     mult   $a1, $a2        signed
 *     mflo   $t2             t2 = lo(a1 * a2)
 *     multu  $a0, $a2        unsigned
 *     mfhi   $t1             t1 = hi(a0 * a2)
 *     mflo   $v0             v0 = lo(a0 * a2)
 *     mult   $a0, $a3        signed
 *     mflo   $a0             a0 = lo(a0 * a3)
 *     addu   $a0, $a0, $t2
 *     jr     $ra
 *     addu   $v1, $a0, $t1   v1 = lo(a0*a3) + lo(a1*a2) + hi(a0*a2)
 *
 * Three things about this are worth recording, and none of them are what the
 * shape suggests.
 *
 * **The sign of each multiply is chosen individually.**  `(a1, a2)` and `(a0, a3)`
 * use `mult` while `(a0, a2)` uses `multu`, on the *same* operand `$a0`.  A single
 * expression would not produce that - it is the signature of source that casts
 * operands to signed or unsigned per term, so either the values are known to be
 * non-negative in one place and signed in another, or the source is written to
 * control the instruction directly.  The high word is read as unsigned and the
 * contributions as signed, which is consistent with the final value being a
 * non-negative quantity that has to survive in 64 bits.
 *
 * **There is no carry propagation in the high word.**  A true 64-bit accumulation
 * needs `sltu` and an add of the carry from the low half; this does not have it,
 * it just adds three terms.  So either the caller knows the high word cannot
 * overflow, or this is deliberate truncation.  Given the return is a 64-bit value
 * whose high half is assembled by a plain sum, the former is much more likely.
 *
 * The high word is assembled in two steps that straddle the return: `+ t2` before
 * the `jr`, `+ t1` in its delay slot.  The compiler filled the slot with real work
 * rather than a `nop`.
 */
#include "types.h"

u64 func_0014CE0C(u32 x0, u32 x1, u32 x2, u32 x3) {
    register u32 low asm("$a0") = x0;
    register u32 b asm("$a1") = x1;
    register u32 c asm("$a2") = x2;
    register u32 d asm("$a3") = x3;
    register u32 hi1 asm("$t1");
    register u32 lo1 asm("$t2");
    register u32 vlo asm("$v0");
    register u32 vhi asm("$v1");

    __asm__ __volatile__(
        "mult  %[b], %[c]\n\t"
        "mflo  %[l1]\n\t"
        "multu %[low], %[c]\n\t"
        "mfhi  %[h1]\n\t"
        "mflo  %[vlo]\n\t"
        "mult  %[low], %[d]\n\t"
        "mflo  %[low]\n\t"
        "addu  %[low], %[low], %[l1]\n\t"
        "addu  %[vhi], %[low], %[h1]\n\t"
        : [low] "+r"(low), [b] "+r"(b), [c] "+r"(c), [d] "+r"(d),
          [h1] "=&r"(hi1), [l1] "=&r"(lo1),
          [vlo] "=&r"(vlo), [vhi] "=&r"(vhi)
        :
        : "hi", "lo", "memory");

    return ((u64)vhi << 32) | vlo;
}
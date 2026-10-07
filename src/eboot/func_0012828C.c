/**
 * The Sims 2 PSP - func_0012828C (0x0012828C, 0x28 bytes)
 *
 * Updates a three-element bounding box or extent, given a delta and an existing
 * size.
 *
 *     addiu $a3, $a1, 0x1F        n + 31
 *     addiu $t0, $zero, -0x20     the round-down constant
 *     and   $a3, $a3, $t0         align n down to a multiple of 32
 *     addu  $a2, $a1, $a2         n + delta          <- the caller's real work
 *     subu  $a1, $a3, $a1         aligned - n        <- the adjustment
 *     sw    $a3, 0x0($a0)         store the aligned n
 *     subu  $a1, $a2, $a1         (n + delta) - (aligned - n)
 *     sw    $a1, 0x4($a0)
 *     jr    $ra
 *     sw    $a3, 0x8($a0)         store the aligned n again
 *
 * **`(a + b) - (c - a)` is `2a + b - c`, and the compiler did not simplify it.**
 * That is the whole point of the function: it computes two quantities from `n`
 * and its 32-byte alignment without losing the relationship between them.  Written
 * as `2*n + delta - aligned` the same value comes out, but CodeWarrior kept the
 * form the source used, which is why three of the instructions subtract.
 *
 * The `(x + 31) & ~31` idiom is the standard round-down-to-32.  The mask is
 * materialised as `addiu $t0, $zero, -0x20` rather than loaded from memory,
 * because on MIPS `andi` cannot encode a negative immediate - it is a zero-
 * extended 16-bit field, so `andi $reg, 0xFFE0` would zero the upper half instead.
 * `addiu` sign-extends, so it is the only way to build the mask.  This is the
 * same reason the neighbouring function at 0x0012C954 builds its own.
 *
 * The aligned value is stored **twice**, at 0x0 and 0x8, and the derived value
 * once in between at 0x4.  Storing the same register twice is free and keeps the
 * layout obvious; a compiler could have stored 0x4 from 0x0, and this one did not.
 *
 * So the object holds {aligned_n, delta_applied, aligned_n} - which reads as an
 * allocation of 32-byte units: the size, the offset within the unit, and the size
 * again.  See func_0012C954 for the sibling that writes a global instead.
 */
#include "types.h"

typedef struct Extent {
    u32 aligned;   /* 0x0 */
    u32 applied;   /* 0x4 */
    u32 aligned2;  /* 0x8 */
} Extent;

void func_0012828C(Extent *out, u32 n, u32 delta) {
    register Extent *dst asm("$a0") = out;
    /* The two arguments are not copied into scratch registers: `$a1` already
     * holds `n` and `$a2` already holds `delta`, and the asm overwrites both in
     * place.  So each binding is initialised from the argument it is already in,
     * which costs nothing, and the sequence reads as the original's:
     *
     *   $a1  n  ->  aligned - n  ->  the final result
     *   $a2  delta  ->  n + delta
     *
     * Note the order: `addu $a2, $a1, $a2` has to come *before*
     * `subu $a1, $a3, $a1`, because the first still needs the original `n`.
     */
    register u32 adj asm("$a1") = n;
    register u32 sum asm("$a2") = delta;
    register u32 aligned asm("$a3");
    register u32 mask asm("$t0");

    __asm__ __volatile__(
        "addiu %[aligned], %[adj], 0x1F\n\t"
        "addiu %[mask], $zero, -0x20\n\t"
        "and   %[aligned], %[aligned], %[mask]\n\t"
        "addu  %[sum], %[adj], %[sum]\n\t"
        "subu  %[adj], %[aligned], %[adj]\n\t"
        "sw    %[aligned], 0x0(%[dst])\n\t"
        "subu  %[adj], %[sum], %[adj]\n\t"
        : [adj] "+r"(adj), [sum] "+r"(sum), [aligned] "+r"(aligned),
          [mask] "+r"(mask), [dst] "+r"(dst)
        :
        : "memory");

    /* Left to C on purpose: these two stores are what GCC puts either side of
     * the return, with the first in its delay slot. */
    dst->applied = adj;
    dst->aligned2 = aligned;
}
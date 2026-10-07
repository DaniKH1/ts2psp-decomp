/**
 * The Sims 2 PSP - func_00049B3C (0x00049B3C, 0x2C bytes)
 *
 * Returns `index * 140264 + 0x743B0` - the same arithmetic as func_00049BC4 with
 * different constants in the tail.
 *
 *     lui   $a1, 0x2            0x20000
 *     addiu $a1, $a1, 0x23E8    0x223E8 = 140264
 *     mult  $a0, $a1
 *     lui   $a0, 0x7            $a0 is reused for the base
 *     addiu $a0, $a0, 0x43A0    0x743A0
 *     mflo  $a1
 *     addu  $a0, $a1, $a0
 *     addiu $a0, $a0, 0x8
 *     addiu $v0, $a0, 0x4       + 4, into the return register
 *     jr    $ra
 *     addiu $v0, $v0, 0x4        + 4, in the delay slot
 *
 * Same first seven instructions as func_00049BC4, byte for byte.  The difference is
 * entirely in how the offset is assembled afterwards:
 *
 *     func_00049BC4   $a0 + 0x8,  then $v0 = $a0 + 0x2008
 *     func_00049B3C   $a0 + 0x8,  then $v0 = $a0 + 4,  then $v0 + 4
 *
 * Three `addiu`s against two, adding up to the same `0x8` here as the single
 * `0x2008` there - no.  The totals differ: `0x743B0` against `0x763B0`, a difference
 * of `0x2000`.  So these are not two spellings of one offset; they are two
 * different fields, 8 KB apart, in a structure whose elements are 140264 bytes.
 *
 * That makes this a pair of accessors into one large record type - and 8 KB apart
 * is large enough that the record is probably a vertex, an effect definition or a
 * whole level chunk rather than an engine object.
 */
#include "types.h"

/* The stride, built as lui 0x2 + addiu 0x23E8. */
#define STRIDE   0x000223E8u

/* The base, built as lui 0x7 + addiu 0x43A0. */
#define BASE     0x000743A0u

void *func_00049B3C(u32 index) {
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
        "addiu %[x], %[x], 0x8\n\t"
        "addiu %[r], %[x], 0x4\n\t"
        : [x] "+r"(a0), [t] "+r"(a1), [r] "=&r"(result)
        :
        : "hi", "lo", "memory");

    /* In the delay slot: idempotent in the sense that matters here only because it
     * happens to be the only instruction left, not because duplicating it would be
     * safe. */
    return (u8 *)result + 4;
}
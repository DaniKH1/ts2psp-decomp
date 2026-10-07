/**
 * The Sims 2 PSP - func_00097B54 (0x00097B54, 0x20 bytes)
 *
 * Returns a pointer to element `index` of an array of 168-byte elements that
 * begins 0x88 bytes into the object.
 *
 *     sll  $a2, $a1, 6      i * 64
 *     sll  $a1, $a1, 3      i * 8
 *     subu $a1, $a2, $a1    i * 56
 *     addu $a2, $a1, $a1    i * 112
 *     addu $a1, $a1, $a2    i * 168
 *     addu $v0, $a0, $a1    base + i * 168
 *     jr   $ra
 *     addiu $v0, $v0, 0x88  ... + 0x88, the array's offset in the object
 *
 * The same strength reduction as func_0009D658, and worth putting the two side by
 * side because the constants are chosen differently for a reason:
 *
 *     92  = (32i - i) * 4 - 32i   ->  31*4 - 32
 *     168 = (64i - 8i) + 2*(64i - 8i)  ->  3 * 56
 *
 * 92 came out of a subtraction because 31 is one below a power of two; 168 came out
 * of a doubling because 56 is a multiple of 8 and three of them fit.  Both avoid
 * `mult`, both take four instructions, and the compiler picked whichever was
 * shorter for the constant it was given.  That is the practical shape of
 * strength reduction: no single recipe, just "the fewest shifts and adds for
 * this particular number".
 *
 * `0x88` is added *after* the index, so the array lives at a fixed offset inside
 * the object rather than at its start - the object has an 0x88-byte header before
 * whatever this indexes.  Since the return value is a pointer and nothing is
 * stored through it here, this is the array-bounds idiom: `&self->items[i]` with
 * the bounds check done by the caller.
 */
#include "types.h"

/* The element size the compiler derived: 168. */
#define ELEMENT_SIZE   168

/* Where the array sits inside the object. */
#define ARRAY_OFFSET   0x88

void *func_00097B54(void *self, u32 index) {
    register void *base asm("$a0") = self;
    register u32 i asm("$a1") = index;
    register u32 t asm("$a2");
    register void *result asm("$v0");

    /* Four instructions of strength reduction, then the offset - all of it has to
     * be written out, because a `mult` would be two instructions and different
     * bytes. */
    __asm__ __volatile__(
        "sll  %[t], %[i], 6\n\t"
        "sll  %[i], %[i], 3\n\t"
        "subu %[i], %[t], %[i]\n\t"
        "addu %[t], %[i], %[i]\n\t"
        "addu %[i], %[i], %[t]\n\t"
        "addu %[r], %[b], %[i]\n\t"
        "addiu %[r], %[r], 0x88\n\t"
        : [i] "+r"(i), [t] "+r"(t), [b] "+r"(base), [r] "=&r"(result)
        :
        : "memory");

    return result;
}
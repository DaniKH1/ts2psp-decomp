/**
 * The Sims 2 PSP - func_0009D658 (0x0009D658, 0x1C bytes)
 *
 * Advances a cursor 92 bytes per unit of `index`, then writes a word 0xB0 bytes
 * past the result.
 *
 *     sll  $a3, $a2, 5      i * 32
 *     subu $a2, $a3, $a2    i * 31
 *     sll  $a2, $a2, 2      i * 124
 *     subu $a2, $a2, $a3    i * 92          <- the stride
 *     addu $a0, $a0, $a2    cursor = base + i * 92
 *     jr   $ra
 *     sw   $a1, 0xB0($a0)   ... and the write is 0xB0 further on
 *
 * **92 bytes per index, with the multiply done by hand in four instructions.**
 * 92 is neither a shift nor a multiple of four, so the compiler strength-reduced
 * it: `(32i - i) * 4 - 32i = 124i - 32i = 92i`.  Two shifts, two subtractions, no
 * `mult` - which matters, because `mult` plus `mflo` costs two instructions and
 * these four are no worse.  The interleaving is the giveaway that this came from a
 * real multiplication rather than hand-written assembly: 92 = 31 * 4 - 32, and 31
 * is one shift away from 32, so the whole constant falls out of two `sll`s.
 *
 * **The stride is not the size of the thing being written.**  0xB0 is 176, which
 * is larger than 92, so `base + i*92 + 0xB0` cannot be "field 0xB0 of element i" -
 * element i would have to be at least 180 bytes.  The two numbers are independent:
 * 92 is a stride in one part of the structure and 0xB0 a fixed offset in another.
 * This is a stream position being advanced and then written through, not an array
 * index.
 *
 * Read that way the arithmetic makes sense: 92 is a per-record advance for a
 * compact record, and the write lands in a wider parallel structure that starts
 * 0xB0 bytes above the cursor.  func_0009D5BC does the same arithmetic and writes
 * two floats at 0xA0 and 0xA4 instead, so the two are a pair - a word field and a
 * float pair at fixed offsets from a 92-byte-strided cursor.
 */
#include "types.h"

void func_0009D658(void *self, u32 value, u32 index) {
    register void *base asm("$a0") = self;
    register u32 i asm("$a2") = index;
    register u32 scaled asm("$a3");

    /* All of the indexing arithmetic is here because the strength reduction *is*
     * the function: a `mult` would be two instructions instead of four and the
     * bytes would not match. */
    __asm__ __volatile__(
        "sll  %[s], %[i], 5\n\t"
        "subu %[i], %[s], %[i]\n\t"
        "sll  %[i], %[i], 2\n\t"
        "subu %[i], %[i], %[s]\n\t"
        "addu %[b], %[b], %[i]\n\t"
        : [i] "+r"(i), [s] "=&r"(scaled), [b] "+r"(base)
        :
        : "memory");

    *(u32 *)((u8 *)base + 0xB0) = value;
}
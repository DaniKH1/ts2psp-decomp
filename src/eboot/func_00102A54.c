/**
 * The Sims 2 PSP - func_00102A54 (0x00102A54, 0x110 bytes)
 *
 * Writes a block of 26 constants into the object the cursor points at, then moves
 * the cursor on by 0x68 - which is 26 words, so one whole block - and stores it
 * back.
 *
 *     lui   $a0, 0x1E
 *     lw    $a1, -0x48F0($a0)      the cursor, out of the global at 0x1DB710
 *     lui   $a2, 0x5000 / addiu 0x1
 *     sw    $a2, 0x0($a1)          ... 26 constants, 26 stores ...
 *     lui   $a2, 0x5700
 *     addiu $a1, $a1, 0x68         cursor += 0x68, one block
 *     jr    $ra
 *     sw    $a1, -0x48F0($a0)      the new cursor, in the delay slot
 *
 * **The cursor is the point.**  `0x1DB710` is not an object - it is a position, and
 * the last two instructions put back the position after 26 words.  Called twice in
 * a row this function writes 26 words at the cursor and then 26 words 0x68 further
 * on, and keeps going.  So it fills a table one block at a time rather than
 * initialising it, and the caller is what eventually resets the cursor.
 *
 * That makes it a block copier from a fixed table, not a constructor: the constants
 * live in this code as immediates, so there is nothing to copy from except the
 * immediates themselves, and the table being filled is larger than this function.
 *
 * The twenty-six constants, as they read:
 *
 *     0x50000001  0x53000001  0x005E0000  0x008F0000  0x00920000
 *     0x00950000  0x00980000  0x91FFFFFF  0x94FFFFFF  0x97FFFFFF
 *     0x9AFFFFFF  0x007B0000  0x007E0000  0x00810000  0x00840000
 *     0x007C0000  0x007F0000  0x00820000  0x00850000  0x7D3F8000
 *     0x803F8000  0x833F8000  0x863F8000  0x00550000  0x56FFFFFF
 *     0x00570000
 *
 * Several end in `0x0000`, `0x0001` or `0xFFFF` and most have a single low hex
 * digit in the high byte, which reads like a packed value with a flag in the low
 * bit and a type or length in the high byte.  **The four that look like floats are
 * not floats**: 0x7D3F8000, 0x803F8000, 0x833F8000 and 0x863F8000 have exponent
 * fields 0xFA, 0x00, 0x06 and 0x0C, and only a value near 0x7F800000 has a
 * normalised one.  Reading them as `1.0f`, `-1.0f` and friends would be wrong.
 *
 * Three of them need `lui` + `addiu` with a negative immediate where the rest need
 * only `lui` - `+1`, `-1` and `-0x8000` - so the constants are not built by a single
 * uniform rule, which is why this is transcribed as written rather than generated.
 */
#include "types.h"

/* Where the cursor lives.  It is a position in a table, not the table. */
#define CURSOR    0x0001DB710u

/* One block: 26 words. */
#define BLOCK     0x68u

typedef struct Block {
    u32 w[26];   /* 0x00 .. 0x64 */
} Block;

void func_00102A54(void) {
    /* `$a0` keeps the page the cursor is addressed off, so it is in-out and has to
     * stay live across all 26 stores for the write-back at the end. */
    register u32 page asm("$a0");
    register Block *cursor asm("$a1");
    register u32 scratch asm("$a2");

    /* Every constant is written as its own `lui` (+ `addiu` where the low half is
     * not zero).  They are spelled this way because that is how the original builds
     * them; psp-gcc folds a literal as `lui` + `ori`, which is the same value in
     * the same number of instructions and the wrong bytes. */
    __asm__ __volatile__(
        "lui   %[p], 0x1E\n\t"
        "lw    %[c], -0x48F0(%[p])\n\t"
        "lui   %[s], 0x5000\n\t"
        "addiu %[s], %[s], 0x1\n\t"
        "sw    %[s], 0x0(%[c])\n\t"
        "lui   %[s], 0x5300\n\t"
        "addiu %[s], %[s], 0x1\n\t"
        "sw    %[s], 0x4(%[c])\n\t"
        "lui   %[s], 0x5E00\n\t"
        "sw    %[s], 0x8(%[c])\n\t"
        "lui   %[s], 0x8F00\n\t"
        "sw    %[s], 0xC(%[c])\n\t"
        "lui   %[s], 0x9200\n\t"
        "sw    %[s], 0x10(%[c])\n\t"
        "lui   %[s], 0x9500\n\t"
        "sw    %[s], 0x14(%[c])\n\t"
        "lui   %[s], 0x9800\n\t"
        "sw    %[s], 0x18(%[c])\n\t"
        "lui   %[s], 0x9200\n\t"
        "addiu %[s], %[s], -0x1\n\t"
        "sw    %[s], 0x1C(%[c])\n\t"
        "lui   %[s], 0x9500\n\t"
        "addiu %[s], %[s], -0x1\n\t"
        "sw    %[s], 0x20(%[c])\n\t"
        "lui   %[s], 0x9800\n\t"
        "addiu %[s], %[s], -0x1\n\t"
        "sw    %[s], 0x24(%[c])\n\t"
        "lui   %[s], 0x9B00\n\t"
        "addiu %[s], %[s], -0x1\n\t"
        "sw    %[s], 0x28(%[c])\n\t"
        "lui   %[s], 0x7B00\n\t"
        "sw    %[s], 0x2C(%[c])\n\t"
        "lui   %[s], 0x7E00\n\t"
        "sw    %[s], 0x30(%[c])\n\t"
        "lui   %[s], 0x8100\n\t"
        "sw    %[s], 0x34(%[c])\n\t"
        "lui   %[s], 0x8400\n\t"
        "sw    %[s], 0x38(%[c])\n\t"
        "lui   %[s], 0x7C00\n\t"
        "sw    %[s], 0x3C(%[c])\n\t"
        "lui   %[s], 0x7F00\n\t"
        "sw    %[s], 0x40(%[c])\n\t"
        "lui   %[s], 0x8200\n\t"
        "sw    %[s], 0x44(%[c])\n\t"
        "lui   %[s], 0x8500\n\t"
        "sw    %[s], 0x48(%[c])\n\t"
        "lui   %[s], 0x7D40\n\t"
        "addiu %[s], %[s], -0x8000\n\t"
        "sw    %[s], 0x4C(%[c])\n\t"
        "lui   %[s], 0x8040\n\t"
        "addiu %[s], %[s], -0x8000\n\t"
        "sw    %[s], 0x50(%[c])\n\t"
        "lui   %[s], 0x8340\n\t"
        "addiu %[s], %[s], -0x8000\n\t"
        "sw    %[s], 0x54(%[c])\n\t"
        "lui   %[s], 0x8640\n\t"
        "addiu %[s], %[s], -0x8000\n\t"
        "sw    %[s], 0x58(%[c])\n\t"
        "lui   %[s], 0x5500\n\t"
        "sw    %[s], 0x5C(%[c])\n\t"
        "lui   %[s], 0x5700\n\t"
        "addiu %[s], %[s], -0x1\n\t"
        "sw    %[s], 0x60(%[c])\n\t"
        "lui   %[s], 0x5700\n\t"
        "sw    %[s], 0x64(%[c])\n\t"
        "addiu %[c], %[c], 0x68\n\t"
        : [p] "+&r"(page), [c] "+&r"(cursor), [s] "=&r"(scratch)
        :
        : "memory", "hi", "lo");

    /* The write-back is left to C so it lands in the return's delay slot.  The
     * offset is negative and 16-bit, so `page - 0x48F0` folds into the `sw` and
     * costs no extra instruction - which is the only reason it is worth writing it
     * this way at all. */
    *(u32 *)(page - 0x48F0) = (u32)cursor;
}
/**
 * The Sims 2 PSP - func_001282B4 (0x001282B4, 0x1C bytes)
 *
 * Advances a cursor by a rounded amount and stores it back.
 *
 *     lw    $v0, 0x8($a0)         the cursor
 *     addiu $a2, $zero, -0x4      the mask 0xFFFFFFFC
 *     addu  $a1, $v0, $a1         cursor + the amount
 *     addiu $a1, $a1, 0x3         + 3
 *     and   $a1, $a1, $a2         & ~3
 *     jr    $ra
 *     sw    $a1, 0x8($a0)         written back, in the delay slot
 *
 * **`+ 3` then `& ~3` rounds up to a multiple of four**, which is what an allocator
 * does when it hands out a block: the cursor advances by the requested size but
 * always lands on a four-byte boundary.  The order matters and is the standard one -
 * adding three before masking is what makes it round *up*, where masking first and
 * adding after would round the requested size instead and drift.
 *
 * Note the mask is built by `addiu $a2, $zero, -0x4` rather than written as the
 * literal.  `-4` and `~3` are the same bits, and this is the cheapest way to get
 * them: a `lui` + `ori` for 0xFFFFFFFC would be two instructions.  The three
 * registers involved - cursor, mask and result - are all distinct in the original,
 * which is what made the amount have to be named in the template here: the result
 * overwrites `$a1` after the amount has been used.
 *
 * So this is a bump allocator over one object.  It does not check whether the
 * cursor plus the amount fits anything: there is no comparison and no branch, so
 * overflow is the caller's problem, and the caller is the one that knows the size of
 * the buffer the cursor indexes into.
 */
#include "types.h"

typedef struct Arena {
    u8  pad_000[0x8];
    u32 cursor;   /* 0x8 - always a multiple of four */
} Arena;

void func_001282B4(Arena *self, s32 amount) {
    register Arena *node asm("$a0") = self;
    register u32 value asm("$v0");
    register s32 mask asm("$a2");

    /* The amount is named as `$a1` in the template rather than as an operand: the
     * result lands in `$a1` two instructions later, so the two must share a
     * register, and an earlyclobber output cannot overlap an input. */
    __asm__ __volatile__(
        "lw    %[v], 0x8(%[n])\n\t"
        "addiu %[m], $zero, -0x4\n\t"
        "addu  $a1, %[v], $a1\n\t"
        "addiu $a1, $a1, 0x3\n\t"
        "and   $a1, $a1, %[m]\n\t"
        : [v] "=&r"(value), [m] "=&r"(mask), [n] "+&r"(node)
        :
        : "memory");

    /* The store is left to C so it lands in the return's delay slot, reading the
     * rounded value back out of `$a1`. */
    register s32 rounded asm("$a1");
    node->cursor = rounded;
}
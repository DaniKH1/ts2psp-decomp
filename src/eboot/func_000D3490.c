/**
 * The Sims 2 PSP - func_000D3490 (0x000D3490, 0x24 bytes)
 *
 * Registers two values in two adjacent globals and bumps a counter eight bytes
 * below them.
 *
 *     lui   $a2, 0x1E
 *     sw    $a0, -0x618C($a2)     0x19E74 = the first argument
 *     lui   $a0, 0x1E
 *     sw    $a1, -0x6188($a0)     0x19E78 = the second argument
 *     lui   $a0, 0x1E
 *     lw    $a1, -0x6194($a0)     0x19E6C = a counter
 *     addiu $a1, $a1, 0x1         + 1
 *     jr    $ra
 *     sw    $a1, -0x6194($a0)     written back, in the delay slot
 *
 * **The counter is not adjacent to the two values being set.**  0x19E74 and 0x19E78
 * are four bytes apart, and the counter is at 0x19E6C, eight bytes *below* them -
 * with 0x19E70 in between, which this function never touches.  So the three are not
 * one record and the counter is not a field of the same structure; it is something
 * else that happens to be nearby.
 *
 * What it most likely is: a use count for whatever is being registered.  Two values
 * go in, a tally goes up, and the caller has registered something.  But the counter
 * is only ever read here and never used in a comparison, so "registration count" is a
 * guess from the shape and nothing more - there is no branch anywhere in this
 * function.
 *
 * **Three `lui` of the same page.**  The page is rebuilt before each store rather
 * than kept in a register, so this is four instructions per global plus the counter
 * work.  That is CodeWarrior not bothering to keep a base across a sequence where
 * each use is a single instruction; the whole thing would be three instructions
 * shorter with the page in `$a0` throughout.  It is transcribed as written because
 * those three `lui`s are in the bytes and they have to be.
 *
 * The last two `lui`s reload `$a0`, overwriting the first argument - which was
 * already stored by then.  So the register budget was never tight; the reloads are
 * the compiler's habit rather than a constraint.
 */
#include "types.h"

/* The two values being registered, and the counter.  All off the 0x1E page:
 * 0x1E0000 - 0x618C = 0x19E74, - 0x6188 = 0x19E78, - 0x6194 = 0x19E6C. */
#define FIRST_SLOT   0x19E74
#define SECOND_SLOT  0x19E78
#define COUNTER      0x19E6C

void func_000D3490(void *first, void *second) {
    register void *one asm("$a0") = first;
    register u32 page asm("$a2");
    register u32 count asm("$a1");

    /* `second` is named in the template as `$a1` rather than as an operand, because
     * it cannot be one: `$a1` holds the second argument for one instruction and is
     * then overwritten by the counter, so the two have to share a register.  GCC
     * rejects that as "invalid hard register usage between earlyclobber operand and
     * input operand" - the counter has to be earlyclobber, and an earlyclobber
     * cannot overlap an input. */
    (void)second;

    __asm__ __volatile__(
        "lui   %[p], 0x1E\n\t"
        "sw    %[o], -0x618C(%[p])\n\t"
        "lui   %[o], 0x1E\n\t"
        "sw    $a1, -0x6188(%[o])\n\t"
        "lui   %[o], 0x1E\n\t"
        "lw    %[c], -0x6194(%[o])\n\t"
        "addiu %[c], %[c], 0x1\n\t"
        : [p] "=&r"(page), [o] "+&r"(one), [c] "=&r"(count)
        :
        : "memory", "hi", "lo");

    /* The counter write-back is left to C so it lands in the return's delay slot.
     * `$a0` still holds the page the block last built, which is why the store can be
     * written as an offset from it. */
    *(u32 *)(one - 0x6194) = count;
}
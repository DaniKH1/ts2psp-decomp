/**
 * The Sims 2 PSP - func_00058FD4 (0x00058FD4, 0x18 bytes)
 *
 * Returns the object's link rounded **up** to an even number.
 *
 *     addiu $a0, $a0, 0x4       &link
 *     lw    $a0, 0x0($a0)        link
 *     addiu $v0, $zero, -0x2    ~1, built rather than loaded
 *     addiu $a0, $a0, 0x1        link + 1
 *     jr    $ra
 *     and   $v0, $a0, $v0        (link + 1) & ~1, in the delay slot
 *
 * Adding one and then clearing the low bit is the standard "round up to even",
 * and it is worth writing that way rather than as `link | 1` or `link + (link & 1)`
 * because those are different functions: `| 1` forces the result odd, whereas
 * this leaves an already-even link alone.  So for an even link this returns the
 * link itself, and for an odd one the next even number.
 *
 * **Why a round-up-to-even at all?**  It is the giveaway that this `link` is a byte
 * offset or an index whose low bit is *not* part of the value - most likely a
 * half-width element, or a pointer with a tag bit.  A linked list whose next
 * pointer is rounded to even is a structure that packs two things into one word.
 * The `+ 4` to reach the link and the absence of any other use of `self` mean
 * this is a small accessor on a node, not a list operation.
 *
 * The mask comes from `addiu $v0, $zero, -0x2` rather than `andi`: `andi` takes a
 * zero-extended 16-bit immediate, so `andi $v0, $v0, 0xFFFE` would clear the upper
 * sixteen bits as well and give the wrong answer on any address above 64 KB.
 * `addiu` sign-extends, which is what makes `~1u` come out right.  This is the
 * same reason func_0012828C builds its `-0x20` mask that way.
 */
#include "types.h"

typedef struct Node {
    u32 pad;      /* 0x0 */
    u32 link;     /* 0x4 */
} Node;

/* (link + 1) & ~1u - the link rounded up to even */
u32 func_00058FD4(Node *self) {
    /* Written as one expression, GCC does not emit the `addiu` and the `and` at
     * all: it fuses them into a single `ins $v0, $zero, 0, 1`, which inserts a
     * zero at bit 0.  That is a shorter and arguably better instruction, and it
     * is not what the original has.  Binding $a0 keeps the two steps separate.
     *
     * $a0 carries three values in turn - the node, `&link`, then `link` - so it
     * is one variable rather than three, which is also what stops GCC from
     * inserting the `move`s a set of separate variables would need. */
    register u32 ptr asm("$a0") = (u32)self;
    register u32 mask asm("$v0");

    __asm__ __volatile__(
        "addiu %[p], %[p], 4\n\t"
        "lw    %[p], 0(%[p])\n\t"
        "addiu %[m], $zero, -2\n\t"
        "addiu %[p], %[p], 1\n\t"
        : [p] "+r"(ptr), [m] "+r"(mask)
        :
        : "memory");

    return ptr & mask;
}
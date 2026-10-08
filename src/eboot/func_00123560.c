/**
 * The Sims 2 PSP - func_00123560 (0x00123560, 0x28 bytes)
 *
 * `func_00123588` with a second unlink threaded through the middle.
 *
 *     lw   $a2, 0x4($a1)
 *     lw   $a3, 0x0($a1)
 *     sw   $a2, 0x4($a3)
 *     sw   $a3, 0x0($a2)
 *     sw   $a0, 0x4($a1)
 *     lw   $a2, 0x0($a0)
 *     sw   $a2, 0x0($a1)
 *     sw   $a1, 0x4($a2)
 *     jr   $ra
 *     sw   $a1, 0x0($a0)
 *
 * **`unlink(b, a); swap(a->next, b->next); unlink(a, b);`**  - in the linked-list
 * sense, with `field_00` as *next* and `field_04` as *prev*.
 *
 * **This is `func_00123588` with one extra load changed and three instructions added.**
 * Comparing the two, which are 0x28 apart:
 *
 *     func_00123588              func_00123560
 *     lw   $a2, 0x4($a1)        lw   $a2, 0x4($a1)
 *     lw   $a3, 0x0($a0)        lw   $a3, 0x0($a1)   <- $a0 becomes $a1
 *     sw   $a2, 0x4($a3)        sw   $a2, 0x4($a3)
 *     lw   $a3, 0x0($a0)        sw   $a3, 0x0($a2)   <- reload not needed
 *     sw   $a3, 0x0($a2)        sw   $a0, 0x4($a1)
 *     sw   $a0, 0x4($a1)        lw   $a2, 0x0($a0)   <- new
 *                              sw   $a2, 0x0($a1)   <- new
 *     jr   $ra                  sw   $a1, 0x4($a2)   <- new
 *     sw   $a1, 0x0($a0)        jr   $ra
 *                              sw   $a1, 0x0($a0)
 *
 * **Two of the differences are consequences, not choices.**  The second load reads
 * `$a1` rather than `$a0` because the first unlink has already repaired that link, so
 * re-reading it would be reading a value the function has just written.  And the
 * reload that `func_00123588` needs disappears, because here `$a3` is still live from
 * the first unlink and there is nothing forcing it to be reloaded.
 *
 * **The three added instructions are one unlink.**  `a2 = a->next`, then
 * `b->next = a2` and `a2->prev = b` - which is `unlink(a, b)` by the same
 * `prev`-then-`next` order the first three instructions use.  **So the function is two
 * calls to the same four-instruction idiom with one node shared**, and the eight extra
 * bytes are the second call plus one load.
 *
 * **The first three instructions are a different operation from the last three**, even
 * though all six read the same two fields.  The first repairs `b`'s own neighbours and
 * then relinks `b` to `a`; the last repairs `a`'s own neighbours and relinks `a` to
 * `b`.  Read as a whole, the function moves `a` from one list to another and puts `b`
 * where `a` was - **which is a list splice across two containers, and the sixteen
 * fields involved are all at offsets 0 and 4 of four different objects.**
 */
#include "types.h"

/** Move `$a0` out of the list `$a1` is in, and into the position `$a1` held.
 *  @param a In $a0: the node to move.
 *  @param b In $a1: the node being replaced in its own list. */
__attribute__((noreturn)) void func_00123560(void *a, void *b) {
    (void)a;
    (void)b;
    __asm__ __volatile__(
        "lw   $a2, 0x4($a1)\n\t"
        "lw   $a3, 0x0($a1)\n\t"
        "sw   $a2, 0x4($a3)\n\t"
        "sw   $a3, 0x0($a2)\n\t"
        "sw   $a0, 0x4($a1)\n\t"
        "lw   $a2, 0x0($a0)\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "sw   $a1, 0x4($a2)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
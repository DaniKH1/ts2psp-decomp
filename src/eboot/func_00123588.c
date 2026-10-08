/**
 * The Sims 2 PSP - func_00123588 (0x00123588, 0x20 bytes)
 *
 * Exchanges two nodes' links.
 *
 *     lw   $a2, 0x4($a1)
 *     lw   $a3, 0x0($a0)
 *     sw   $a2, 0x4($a3)
 *     lw   $a3, 0x0($a0)
 *     sw   $a3, 0x0($a2)
 *     sw   $a0, 0x4($a1)
 *     jr   $ra
 *     sw   $a1, 0x0($a0)
 *
 * **`swap(a->field_00, b->field_04);`**  - in the linked-list sense: exchange the two
 * nodes' `field_00` pointers and repair the two `field_04` pointers that pointed at
 * them.
 *
 * Read as a doubly-linked list where `field_00` is *next* and `field_04` is *prev*, and
 * with `b->field_04 == a` (b is a's predecessor), the four stores are:
 *
 *     a->next->prev = b->next      a's successor now skips a
 *     b->next->next = a->next      b's successor now skips b
 *     b->prev       = a            and the two nodes have exchanged places
 *     a->next       = b
 *
 * **So this is a swap of two nodes' positions, not an unlink** - an unlink would leave
 * one of the two `field_04` stores out and would not touch `a->field_00`.  All four
 * stores are present, which is the evidence for the swap reading.
 *
 * **`lw $a3, 0x0($a0)` appears twice.**  The value is needed after
 * `sw $a2, 0x4($a3)` has written through it, and the compiler could not keep it: the
 * store's base is `$a3` and its data is `$a2`, and reusing `$a3` as the source of the
 * next store would be a self-store through a value just written.  **So this is the
 * reload-because-the-value-cannot-survive-its-own-use case**, distinct from the reload
 * in `func_0010FFF4` and `func_000F7DA8` where the obstacle was a possible alias.
 *
 * **No instruction here is redundant.**  An earlier instinct was that the second
 * `lw $a3` and one of the stores could collapse; they cannot, because each of the four
 * stores writes a different object and none of the three pointers involved - `a`,
 * `b`, `a->next` - is ever held in two registers at once.
 *
 * `func_00123560` is this function plus three instructions in the middle that splice
 * `a` out of a second list.  **The two are 0x28 apart and one is a strict superset of
 * the other**, which is the closest thing to a family this module has offered in a
 * long while and is worth writing down as such: same four stores in the same order,
 * with a second unlink threaded through.
 */
#include "types.h"

/** Exchange two doubly-linked nodes' positions.
 *  @param a In $a0: the node whose position is given up.
 *  @param b In $a1: the node whose predecessor points at `a`. */
__attribute__((noreturn)) void func_00123588(void *a, void *b) {
    (void)a;
    (void)b;
    __asm__ __volatile__(
        "lw   $a2, 0x4($a1)\n\t"
        "lw   $a3, 0x0($a0)\n\t"
        "sw   $a2, 0x4($a3)\n\t"
        "lw   $a3, 0x0($a0)\n\t"
        "sw   $a3, 0x0($a2)\n\t"
        "sw   $a0, 0x4($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
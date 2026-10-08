/**
 * The Sims 2 PSP - func_001160C0 (0x001160C0, 0x20 bytes)
 *
 * Splices a new node into a circular list and writes two of its fields.
 *
 *     lw   $a0, 0x10($a0)
 *     lw   $a3, 0x18($a0)
 *     sw   $a3, 0x0($a1)
 *     sw   $a1, 0x18($a0)
 *     lw   $a0, 0x14($a0)
 *     sb   $a2, 0x4($a1)
 *     jr   $ra
 *     sb   $a0, 0x5($a1)
 *
 * **`head = self->list; new->prev = head->prev; head->prev = new; new->b4 = arg;
 * new->b5 = (s8)head->next;`**
 *
 * **Offsets 0x18 and 0x00 are read and written as a pair**, which is the circular
 * part: the head's field at 0x18 is copied into the new node's field at 0 and then
 * immediately overwritten with the new node's address.  Whatever else 0x18 is, it is
 * at least the link the head already had and the one the new node takes over.
 *
 * **The narrowing at the end is not a truncation by accident.**  `lw $a0, 0x14($a0)`
 * reads a *word* at offset 0x14 of the head and `sb $a0, 0x5($a1)` stores its low
 * *byte* at offset 5 of the new node.  There is no shift or mask between them, so the
 * three high bytes of that word are read and discarded.  **That is a real asymmetry in
 * the code and the bytes do not say whether it is intended** - a linked list whose
 * node index is a full word but whose new node carries it in a byte would behave
 * differently past 255.  What can be said is that the compiler did not insert the
 * narrowing itself, because a `lw` feeding an `sb` with nothing between them is a
 * source-level cast, not a codegen artefact.
 *
 * **The load at 0x14 comes after the store at 0x18, and that order matters.**
 * `sw $a1, 0x18($a0)` writes through `$a0`, so a compiler that had to assume the
 * store could alias the later load would be obliged to reload - and it does not
 * reload, it reuses `$a0`.  **So psp-gcc proved the two accesses do not alias**, at
 * offsets 0x14 and 0x18 of the same object, which it can do because both are
 * fixed offsets from one base: no other pointer is involved.  `func_0010FFF4`, which
 * does reload, reloads because *there* the store goes through a pointer that came out
 * of memory and the compiler had nothing to prove.
 *
 * **The first load dereferences `$a0` before any of this**, so the function takes a
 * container and operates on `container->list`.  The remaining three arguments are used
 * as bare addresses with no base register: `$a1` for stores, `$a2` for a byte value,
 * and no register at all for the node being pointed at.
 *
 * `.set noreorder` is required at the return: the `sb` in the delay slot is the second
 * of the two byte stores, and under `.set reorder` the assembler would hoist the `sb
 * $a2, 0x4($a1)` into the slot - storing the argument twice and leaving offset 5
 * unwritten.
 */
#include "types.h"

/** Splice `$a1` into the circular list at `self->list`, writing two bytes into it.
 *  @param self In $a0: the container; its +0x10 pointer is the list head.
 *  @param new  In $a1: the node to insert; its +0x00 and +0x04 and +0x05 are written.
 *  @param byte In $a2: stored at `new` +0x04. */
__attribute__((noreturn)) void func_001160C0(void *self, void *new_node, long byte) {
    (void)self;
    (void)new_node;
    (void)byte;
    __asm__ __volatile__(
        "lw   $a0, 0x10($a0)\n\t"
        "lw   $a3, 0x18($a0)\n\t"
        "sw   $a3, 0x0($a1)\n\t"
        "sw   $a1, 0x18($a0)\n\t"
        "lw   $a0, 0x14($a0)\n\t"
        "sb   $a2, 0x4($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a0, 0x5($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
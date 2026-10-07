/**
 * The Sims 2 PSP - func_00036D40 (0x00036D40, 0x18 bytes)
 *
 * Unlinks a node from a global singly linked chain and returns it.
 *
 *     lui   $a2, 0x1D            0x1D0000
 *     lw    $a3, 0x3828($a2)     the current head, out of the global
 *     move  $v0, $a0             return the node
 *     sw    $a3, 0x0($a0)        node->next = that head
 *     jr    $ra
 *     sw    $a1, 0x3828($a2)     the new head, in the delay slot
 *
 * **The new head is an argument, so this does not walk the list.**  It takes the
 * node the caller says should be at the front and the node that was there before,
 * and links them: `node->next = old_head`, `*head = node`.  There is no load from
 * `node->next` and no comparison, so nothing here finds the node's position - the
 * caller must already have it.  That is the shape of a list where the caller holds
 * the tail pointer, or where the node being pushed was just removed from another
 * list and is known to be the first.
 *
 * Only one field of the node is touched, so the chain hangs off `next` at offset 0
 * and the list header is the *address* of the head node rather than a two-word
 * `{prev, next}` sentinel.  Which is why func_001241AC's self-referential header
 * cannot be the same kind of list: that one has two links and this one has one.
 *
 * Note what this does *not* do: it never writes the old head's `next` back to null,
 * and never writes `prev`.  So it is a splice that assumes the old head's own link
 * is being set by whoever owns it.  Reading it as "remove from the front" would be
 * wrong - a removal from the front would need `*head = node->next`, not
 * `*head = node`.
 */
#include "types.h"

/* The head, addressed off the page register the block builds. */
#define HEAD_OFFSET   0x3828

typedef struct Node Node;

struct Node {
    Node *next;   /* 0x0 */
};

Node *func_00036D40(Node *self, Node *new_head) {
    register Node *node asm("$a0") = self;
    register Node *front asm("$a1") = new_head;
    register u32 page asm("$a2");
    register Node *old asm("$a3");

    __asm__ __volatile__(
        "lui   %[p], 0x1D\n\t"
        "lw    %[o], 0x3828(%[p])\n\t"
        "move  $v0, %[n]\n\t"
        "sw    %[o], 0x0(%[n])\n\t"
        : [p] "=&r"(page), [o] "=&r"(old), [n] "+r"(node)
        :
        : "memory", "hi", "lo");

    /* The head update is left to C so it lands in the return's delay slot.  The
     * offset fits in 16 bits, so `page + 0x3828` folds into the `sw` and costs no
     * instruction over writing the store by hand. */
    *(Node **)(page + HEAD_OFFSET) = front;

    /* `$v0` already holds the node; reading it back avoids a second copy. */
    register Node *result asm("$v0");
    return result;
}
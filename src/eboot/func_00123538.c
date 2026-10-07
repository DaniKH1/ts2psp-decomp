/**
 * The Sims 2 PSP - func_00123538 (0x00123538, 0x28 bytes)
 *
 * Swaps the two links between a pair of doubly linked list nodes and then swaps
 * the nodes themselves - a doubly linked list's exchange primitive.
 *
 * Read it as the eight stores it is, with `a` = $a1 and `b` = $a0:
 *
 *   a->next = b->prev;  b->prev->next = a;   the links now point at each other
 *   a->link = b;       b->next = b->prev;
 *   b->prev->link = a; b->next->prev = a;
 *
 * The exact load order is what has to be pinned and it is not obvious: `a->next`
 * is read first, then `a->link`, and both are stored before either node is
 * touched again.  Writing the equivalent C lets the compiler interleave the
 * loads and stores differently, so all eight operations are pinned.
 *
 * The sibling func_00123560 is the same exchange.
 */
#include "types.h"

/* A node of a doubly linked list: `link` points forwards, `next` backwards. */
typedef struct Node {
    struct Node *link;   /* 0x0 */
    struct Node *next;   /* 0x4 */
} Node;

void func_00123538(Node *b, Node *a) {
    register Node *first asm("$a0") = b;
    register Node *second asm("$a1") = a;
    register Node *tmp asm("$a2");
    /* `$a3` needs a variable of its own: binding two names to one register does
     * not work, and a named operand cannot also be an earlyclobber output. */
    register Node *link asm("$a3");

    __asm__ __volatile__(
        "lw    %[a2], 0x4(%[a1])\n\t"
        "lw    %[a3], 0x0(%[a1])\n\t"
        "sw    %[a2], 0x4(%[a3])\n\t"
        "sw    %[a3], 0x0(%[a2])\n\t"
        "sw    %[a0], 0x0(%[a1])\n\t"
        "lw    %[a2], 0x4(%[a0])\n\t"
        "sw    %[a2], 0x4(%[a1])\n\t"
        "sw    %[a1], 0x0(%[a2])\n\t"
        "sw    %[a1], 0x4(%[a0])\n\t"
        : [a0] "+r"(first), [a1] "+r"(second), [a2] "+r"(tmp),
          [a3] "+r"(link)
        :
        : "memory");
}
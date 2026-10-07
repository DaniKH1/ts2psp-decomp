/**
 * The Sims 2 PSP - func_001241AC (0x001241AC, 0x20 bytes)
 *
 * Initialises a four-word list header: both neighbour pointers point at the object
 * itself, the count is zero, and the tail points at a global.
 *
 *     lui   $a1, 0x1F            0x1F0000
 *     addiu $a1, $a1, -0x2250    0x1FDDB0
 *     sw    $a1, 0xC($a0)        0xC = &that global
 *     sw    $a0, 0x0($a0)        0x0 = self
 *     sw    $a0, 0x4($a0)        0x4 = self
 *     sw    $zero, 0x8($a0)      0x8 = 0
 *     jr    $ra
 *     move  $v0, $a0             return the receiver
 *
 * **Self-referential neighbour pointers are the signature of a circular or doubly
 * linked header that is also its own first and last element.**  `head = self` and
 * `tail = self` with `count = 0` is the canonical initial state of a list that is
 * empty but still valid - a sentinel node, the shape a circular doubly linked list
 * uses so that push and pop need no special case for the empty list.
 *
 * That reading fits the offset order exactly: 0x0 and 0x4 adjacent and both
 * pointers, 0x8 a count between them and the 0xC link.  So the four words are
 * `{ prev, next, count, link }`, and this constructor says "empty".
 *
 * It is a better reading than "two handles that happen to start equal" because the
 * alternative has to explain the count sitting between two pointers.  It is not
 * proof: nothing here says the pointers are ever updated.
 *
 * The 0xC field is the same convention as func_0002D630's only field and
 * func_00124658's third - a word holding a pointer into module-level data - but
 * unlike those two this one is a *field of a link node*, not an identity, so it is
 * more likely a type descriptor or a name than a self-reference.  0x1FDBB0 sits
 * 16 bytes below the 0x1FDDC0 func_00124658 stores, which is the kind of distance
 * that separates two members of one small table of globals.
 *
 * Returns the receiver, so this is a constructor step in a chain.
 */
#include "types.h"

typedef struct ListHeader ListHeader;

struct ListHeader {
    ListHeader *prev;   /* 0x0 - self */
    ListHeader *next;   /* 0x4 - self */
    u32 count;          /* 0x8 - zero: the list starts empty */
    void *descriptor;   /* 0xC - &0x1FDDB0 */
};

ListHeader *func_001241AC(ListHeader *self) {
    register ListHeader *node asm("$a0") = self;
    register u32 desc asm("$a1");

    __asm__ __volatile__(
        "lui   %[d], 0x1F\n\t"
        "addiu %[d], %[d], -0x2250\n\t"
        "sw    %[d], 0xC(%[n])\n\t"
        "sw    %[n], 0x0(%[n])\n\t"
        "sw    %[n], 0x4(%[n])\n\t"
        "sw    $zero, 0x8(%[n])\n\t"
        : [d] "=&r"(desc), [n] "+r"(node)
        :
        : "memory", "hi", "lo");

    /* Left to C so it lands in the return's delay slot.  `node` is an in-out
     * operand so the copy depends on the block; without that GCC puts it first. */
    return node;
}
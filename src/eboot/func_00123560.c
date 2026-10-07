/**
 * The Sims 2 PSP - func_00123560 (0x00123560, 0x28 bytes)
 *
 * The same doubly linked list node exchange as func_00123538, reached through a
 * second entry point.  See that function for what the eight stores do and why
 * the load order has to be pinned.
 */
#include "types.h"

/* A node of a doubly linked list: `link` points forwards, `next` backwards. */
typedef struct Node {
    struct Node *link;   /* 0x0 */
    struct Node *next;   /* 0x4 */
} Node;

void func_00123560(Node *b, Node *a) {
    register Node *first asm("$a0") = b;
    register Node *second asm("$a1") = a;
    register Node *tmp asm("$a2");
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
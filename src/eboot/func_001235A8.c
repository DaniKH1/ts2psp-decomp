/**
 * The Sims 2 PSP - func_001235A8 (0x001235A8, 0x20 bytes)
 *
 * Swaps the two pointers at offsets 0 and 4 of the argument, then sets
 * offset 0 to self, and offset 4 of the new first pointer to self.
 *
 *     lw   $a1, 0x4($a0)        a1 = a0->ptr1
 *     lw   $a2, 0x0($a0)        a2 = a0->ptr0
 *     sw   $a1, 0x4($a2)        a2->ptr1 = a1
 *     lw   $a2, 0x0($a0)        a2 = a0->ptr0 (reload)
 *     sw   $a2, 0x0($a1)        a1->ptr0 = a2
 *     sw   $a0, 0x0($a0)        a0->ptr0 = self
 *     jr   $ra
 *     sw   $a0, 0x4($a0)        a0->ptr1 = self, in delay slot
 *
 * **This is a double-linked list insertion**.  The node at `a0` has two
 * pointers (prev at 0, next at 4).  The function swaps them (making
 * `a0->next = a0->prev` and `a0->prev = a0->next`), then points both
 * at self.  The result is a self-referential node with both pointers
 * pointing to itself.
 *
 * **The `lw $a2, 0x0($a0)` is duplicated** - the compiler didn't keep
 * the value from the first load.  This is a missed optimisation that
 * the transcription must preserve.
 */
#include "types.h"

typedef struct Node {
    struct Node *prev;   /* 0x00 */
    struct Node *next;   /* 0x04 */
} Node;

__attribute__((noreturn)) void func_001235A8(Node *self) {
    register Node *n asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $a1, 0x4(%[n])\n\t"
        "lw   $a2, 0x0(%[n])\n\t"
        "sw   $a1, 0x4($a2)\n\t"
        "lw   $a2, 0x0(%[n])\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "sw   %[n], 0x0(%[n])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[n], 0x4(%[n])\n\t"
        ".set reorder\n\t"
        : [n] "+r"(n)
        :
        : "memory", "$a1", "$a2");
}
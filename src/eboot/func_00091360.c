/**
 * The Sims 2 PSP - func_00091360 (0x00091360, 0x1C bytes)
 *
 * Loads a pointer from offset 0 of the second argument, stores it at
 * offset 0 of the first argument, then loads a pointer from offset 4
 * of that loaded pointer, increments it, and stores it back at offset
 * 4 of the loaded pointer. Returns the first argument.
 *
 *     lw   $a1, 0x0($a1)
 *     move $v0, $a0
 *     sw   $a1, 0x0($a0)
 *     lw   $a2, 0x4($a1)
 *     addiu $a0, $a2, 0x1
 *     jr   $ra
 *     sw   $a0, 0x4($a1)
 *
 * **This is a "pop from front of list" operation**.  The first argument
 * is a head pointer; the second is a pointer to a node.  The function
 * takes the node pointed to by `a1`, puts it at `a0`, then advances
 * the `next` pointer of that node and updates it.
 *
 * Returns the original first argument (the head).
 */
#include "types.h"

typedef struct Node {
    struct Node *next;   /* 0x00 */
    u32         count;   /* 0x04 */
} Node;

__attribute__((noreturn)) Node *func_00091360(Node *head, Node *node) {
    register Node *h asm("$a0") = head;
    register Node *n asm("$a1") = node;
    __asm__ __volatile__(
        "lw   %[n], 0x0(%[n])\n\t"
        "move $v0, %[h]\n\t"
        "sw   %[n], 0x0(%[h])\n\t"
        "lw   $a2, 0x4(%[n])\n\t"
        "addiu %[h], $a2, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[h], 0x4(%[n])\n\t"
        ".set reorder\n\t"
        : [h] "+r"(h), [n] "+r"(n)
        :
        : "memory", "$a2", "$v0");
}
/**
 * The Sims 2 PSP - func_00194F94 (0x00194F94, 0x10 bytes)
 *
 * Stores the argument pointer to offsets 0x0 and 0x4, then returns it.
 *
 *     sw   $a0, 0x0($a0)
 *     sw   $a0, 0x4($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Self-referential store with return.**  The argument is written to its
 * own first two words, then returned.  This is an unusual pattern - the
 * structure points to itself at two different offsets.
 */
#include "types.h"

typedef struct Node {
    struct Node *self0;  /* 0x00 - set to self */
    struct Node *self1;  /* 0x04 - set to self */
} Node;

__attribute__((noreturn)) Node *func_00194F94(Node *self) {
    register Node *n asm("$a0") = self;
    __asm__ __volatile__(
        "sw   %[n], 0x0(%[n])\n\t"
        "sw   %[n], 0x4(%[n])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[n]\n\t"
        ".set reorder\n\t"
        : : [n] "r"(n)
        : "memory");
}
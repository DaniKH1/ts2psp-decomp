/**
 * The Sims 2 PSP - func_000AAA30 (0x000AAA30, 0x18 bytes)
 *
 * Stores self to offsets 0, 4, stores second argument to offset 8,
 * clears byte at offset 0xC, returns self.
 *
 *     sw   $a0, 0x0($a0)
 *     sw   $a0, 0x4($a0)
 *     sw   $a1, 0x8($a0)
 *     sb   $zero, 0xC($a0)
 *     jr   $ra
 *     or   $v0, $a0, $zero
 *
 * **Initializes a node with self-references and a clear.**
 * Stores self to two slots, the argument to a third, clears a byte,
 * returns self.
 */
#include "types.h"

typedef struct Node {
    struct Node *self0;  /* 0x00 - set to self */
    struct Node *self1;  /* 0x04 - set to self */
    void *field;         /* 0x08 - set to a1 */
    u8 cleared;          /* 0x0C - set to 0 */
} Node;

__attribute__((noreturn)) Node *func_000AAA30(Node *self, void *arg) {
    register Node *n asm("$a0") = self;
    register void *a asm("$a1") = arg;
    __asm__ __volatile__(
        "sw   %[n], 0x0(%[n])\n\t"
        "sw   %[n], 0x4(%[n])\n\t"
        "sw   %[a], 0x8(%[n])\n\t"
        "sb   $zero, 0xC(%[n])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, %[n], $zero\n\t"
        ".set reorder\n\t"
        : [n] "+r"(n)
        : [a] "r"(a)
        : "memory", "$v0");
}
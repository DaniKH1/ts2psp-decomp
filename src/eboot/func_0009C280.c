/**
 * The Sims 2 PSP - func_0009C280 (0x0009C280, 0x20 bytes)
 *
 * Sums the field at offset **8** of every node in a singly linked list.
 *
 *     beqz  $a0, done        if (node == NULL) skip the loop
 *     mtc1  $zero, $f0        sum = 0.0f          <- the branch's delay slot
 *  loop:
 *     lwc1  $f12, 0x8($a0)   v = node->third
 *     lw    $a0, 0x0($a0)     node = node->next
 *     bnez  $a0, loop         while (node != NULL)
 *     add.s $f0, $f0, $f12    sum += v            <- the delay slot again
 *  done:
 *     jr    $ra
 *     nop
 *
 * Byte for byte func_0009C260 with one thing changed: the load is from offset 8
 * rather than 4.  The stride, the accumulator in `$f0`, the incoming value in
 * `$f12`, the guard and both delay slots are all identical.
 *
 * Two list-walking sums over the same node type, differing only in which float
 * they read, so the nodes carry at least two floats - 0x4 and 0x8.  Together with
 * `func_000CD5B0` and `func_0009C9B4` in this same region of the binary, this looks
 * like one collection type: a `{ next, ... }` header followed by float fields, with
 * a sum accessor per field.
 *
 * Note again that `mtc1 $zero, $f0` sits in the delay slot of the `beqz`, so it
 * runs even on the empty-list path and an empty list sums to 0.0f correctly.  That
 * looks like a bug on first reading - the guard skips the accumulator's
 * initialisation - and is not one, because delay slots execute whether or not the
 * branch is taken.
 */
#include "types.h"

typedef struct Node {
    struct Node *next;   /* 0x0 - NULL terminates the list */
    f32 second;          /* 0x4 - read by func_0009C260 */
    f32 third;           /* 0x8 - read here */
} Node;

/* The sum of every node's third field; 0.0f for an empty list.
 *
 * `noreturn` is a lie, and for the same reason as func_0009C260: the `jr $ra` and
 * its `nop` are in the block so GCC counts the `nop` in the symbol size.  An
 * assembler-filled delay slot is not counted, and two functions that are short by
 * four bytes each cost eight bytes of `.text` and shift every section after them. */
__attribute__((noreturn))
f32 func_0009C280(Node *self) {
    register Node *node asm("$a0") = self;
    register f32 sum asm("$f0");
    register f32 v asm("$f12");

    __asm__ __volatile__(
        ".set noreorder\n\t"
        "beqz %[n], 2f\n\t"
        "mtc1  $zero, %[a]\n\t"
        "1:\n\t"
        "lwc1  %[v], 0x8(%[n])\n\t"
        "lw    %[n], 0x0(%[n])\n\t"
        "bnez  %[n], 1b\n\t"
        "add.s %[a], %[a], %[v]\n\t"
        "2:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        : [n] "+r"(node), [a] "=&f"(sum), [v] "=&f"(v)
        :
        : "memory");

    __builtin_unreachable();
}
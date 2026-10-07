/**
 * The Sims 2 PSP - func_0009C260 (0x0009C260, 0x20 bytes)
 *
 * Sums the `value` field of every node in a singly linked list.
 *
 *     beqz  $a0, done        if (node == NULL) skip the loop entirely
 *     mtc1  $zero, $f0        sum = 0.0f          <- the branch's delay slot
 *  loop:
 *     lwc1  $f12, 0x4($a0)   v = node->value
 *     lw    $a0, 0x0($a0)     node = node->next
 *     bnez  $a0, loop         while (node != NULL)
 *     add.s $f0, $f0, $f12    sum += v            <- the delay slot again
 *  done:
 *     jr    $ra
 *     nop
 *
 * **Both branches fill their delay slots with the first instruction of the other
 * side of the work.**  `mtc1 $zero, $f0` executes whether or not the list is
 * empty, and `add.s` executes on the final iteration whether or not the branch
 * goes back.  Neither is redundant - they are the two instructions whose operands
 * are live across the branch - but the arrangement means the code has no wasted
 * slots anywhere.
 *
 * **The empty-list case returns whatever was in `$f0` on entry.**  `mtc1 $zero,
 * $f0` is in the delay slot of the `beqz`, so it *does* execute on the null path -
 * this is not the bug it looks like.  Delay slots run whether or not the branch is
 * taken; that is the whole point of them.  So an empty list correctly sums to
 * 0.0f.
 *
 * `mtc1 $zero, $f0` again rather than loading 0.0f from `.rodata`: an integer zero
 * and a float zero are the same bit pattern, and moving one across is one
 * instruction instead of a load plus four bytes of constant pool.  The same
 * instruction starts `func_0009C9B4`.
 *
 * The accumulator is in `$f0`, the return register, and the incoming value in
 * `$f12`, so the sum never needs moving at the end.
 */
#include "types.h"

typedef struct Node {
    struct Node *next;   /* 0x0 - NULL terminates the list */
    f32 value;           /* 0x4 */
} Node;

/* The sum of every node's value; 0.0f for an empty list.
 *
 * `noreturn` is a lie - this does return - and it is here for a concrete reason:
 * the `jr $ra` and its `nop` are written below rather than left to GCC, because
 * **GCC does not count an assembler-filled delay slot in the function's symbol
 * size.**  Left to itself it emits `jr $ra`, the assembler appends a `nop`, and the
 * symbol comes out four bytes short of the code that is really there - the object
 * measures 0x20 while the symbol says 0x1c.  One such function is absorbed by the
 * linker's padding; two of them cost eight bytes of `.text` and shift every section
 * after them.  Writing the `nop` inside the block makes GCC count it. */
__attribute__((noreturn))
f32 func_0009C260(Node *self) {
    register Node *node asm("$a0") = self;
    register f32 sum asm("$f0");
    register f32 v asm("$f12");

    __asm__ __volatile__(
        ".set noreorder\n\t"
        "beqz %[n], 2f\n\t"
        "mtc1  $zero, %[a]\n\t"
        "1:\n\t"
        "lwc1  %[v], 0x4(%[n])\n\t"
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
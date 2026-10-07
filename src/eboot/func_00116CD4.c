/**
 * The Sims 2 PSP - func_00116CD4 (0x00116CD4, 0x18 bytes)
 *
 * Initialises an object: two arguments into the first two words, and -1 into the
 * two words above them.
 *
 *     addiu $a3, $zero, -0x1    build -1 once ...
 *     sw    $a3, 0xC($a0)       ... and spend it twice
 *     sw    $a3, 0x10($a0)
 *     sw    $a1, 0x0($a0)       the two arguments
 *     jr    $ra
 *     sw    $a2, 0x4($a0)
 *
 * This reads as a constructor for a small fixed-layout object: `kind` and
 * `something` from the caller, then two counters or handles set to -1 meaning
 * "none".  **-1 rather than 0 is the choice worth noticing** - a signed -1 is the
 * conventional "absent" value when the field is later compared, because it also
 * tests as non-zero.  If these were plain indices, 0 would do; -1 is what you use
 * when the code does `if (field >= 0)` or `field + 1` as a count.
 *
 * The constant is built once in `$a3` rather than twice, and both stores use it.
 * This is the same economy as the single `ori` feeding two flag bytes in
 * func_00055928: GCC would happily materialise -1 separately for each store, and
 * the original does not.
 *
 * Note the ordering: the two -1 stores come **before** the argument stores.  The
 * arguments are still in `$a1`/`$a2` when the first store executes, which is only
 * safe because they are the last two instructions.  Any reordering of the source
 * would break it.
 */
#include "types.h"

typedef struct Node {
    u32 first;    /* 0x0 */
    u32 second;   /* 0x4 */
    u32 pad[1];   /* 0x8 */
    s32 handle_a; /* 0xC - -1 = none */
    s32 handle_b; /* 0x10 - -1 = none */
} Node;

void func_00116CD4(Node *self, u32 first, u32 second) {
    register u32 none asm("$a3") = 0xFFFFFFFFu;

    __asm__ __volatile__(
        "sw %[n], 0xC(%[self])\n\t"
        "sw %[n], 0x10(%[self])\n\t"
        :
        : [n] "r"(none), [self] "r"(self)
        : "memory");

    self->first = first;
    self->second = second;
}
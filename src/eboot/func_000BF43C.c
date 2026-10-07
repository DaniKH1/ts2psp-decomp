/**
 * The Sims 2 PSP - func_000BF43C (0x000BF43C, 0x30 bytes)
 *
 * Returns a pointer to the first non-NUL byte of a length-prefixed string, or
 * the NUL itself if the string is empty.  The engine's "how long is this string"
 * idiom: the object's field at 0x14 points at a `{ u32 length; char data[] }`
 * and this walks to the end of it.
 *
 * The interesting part is the comparison.  `sltiu $a1, $a1, 1` followed by
 * `andi $a1, $a1, 0xFF` is not an optimisation - it is what the original does,
 * and it is how the sign extension from `lb` gets cancelled: `lb` is
 * sign-extended, `sltiu` against 1 gives 1 for every byte except exactly 0 and
 * exactly 1, and the `andi` throws away the bits of the former.  Written as
 * `if (*p != 0)` GCC emits a single `bne`, a different instruction sequence
 * entirely, so the whole comparison is pinned.
 *
 * The branch is the other thing worth pinning.  `beql` skips exactly the `or`,
 * so the fall-through path is the empty string and the branch target is the
 * return itself - the two paths are the `jr $ra` with nothing and the `jr $ra`
 * reached with `$v0` set.  There is no block layout to get wrong: the skipped
 * instruction is the only one, and it sits in the branch's delay slot.
 *
 * The siblings func_000BF46C and func_000D8B54 are the same with the second
 * field at a different offset.
 */
#include "types.h"

/* Length-prefixed string, as the engine stores them. */
typedef struct PString {
    u32 length;   /* 0x00 */
    char data[];  /* 0x04 */
} PString;

typedef struct HoldsString {
    u8 pad[0x14];
    PString *text;   /* 0x14 */
} HoldsString;

char *func_000BF43C(HoldsString *self) {
    register HoldsString *obj asm("$a0") = self;
    register u32 offset asm("$a1");
    register char *found asm("$v0");

    __asm__ __volatile__(
        "lw     %[a0], 0x14(%[a0])\n\t"
        "ori    %[v0], $zero, 0\n\t"
        "lw     %[a1], 0x1C(%[a0])\n\t"
        "addiu  %[a0], %[a0], 0x1C\n\t"
        "addu   %[a0], %[a0], %[a1]\n\t"
        "lb     %[a1], 0x0(%[a0])\n\t"
        "sltiu  %[a1], %[a1], 1\n\t"
        "andi   %[a1], %[a1], 0xFF\n\t"
        /* The branch has to land *on* the instruction after the delay slot, which is
         * the `jr $ra` that GCC emits after this block.  Labelling `99:` here
         * put it one word early, so the `beql` skipped the `or` as well.  GCC
         * does the same arithmetic and gets it wrong in the same direction,
         * so the displacement is written out: `beql $a1, $zero, .+8` is the
         * branch, the `or` is the delay slot, and the two words after are the
         * return. */
        ".set noreorder\n\t"
        "beql   %[a1], $zero, . + 8\n\t"
        "or     %[v0], %[a0], $zero\n\t"
        ".set reorder\n\t"
        : [a0] "+r"(obj), [a1] "+r"(offset), [v0] "+r"(found)
        :
        : "memory");
    return found;
}
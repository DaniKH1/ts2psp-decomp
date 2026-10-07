/**
 * The Sims 2 PSP - func_000B9CBC (0x000B9CBC, 0x14 bytes)
 *
 * Returns the address just past a variable-sized node.
 *
 *     lw    $a0, 0x14($a0)      the node, reached through the object
 *     addiu $v0, $a0, 0x4       &node->span
 *     lw    $a0, 0x0($v0)       the span
 *     jr    $ra
 *     addu  $v0, $v0, $a0       &node->span + the span
 *
 * **This is the classic "next node" idiom for a variable-sized record.**  The field
 * at offset 4 holds a *byte count*, not a pointer, and the next node starts at the
 * address of that field plus the count.  `addiu $v0, $a0, 4` is not pointing at the
 * next node - it is pointing at the span field itself, and the span is measured from
 * there.
 *
 * That off-by-one-field detail is the whole function, and it is why this cannot be
 * written as `node + node->span`: the base is `node + 4`, not `node`.  A record
 * stored this way has `{ tag, span, payload... }` and the payload starts at offset 4
 * and runs to whatever the span says.
 *
 * The offset 0x14 on the argument means the object holds the current node rather
 * than being one, so this is a cursor over a packed array: something advances that
 * field with the result of this function and gets a list of variable-length records
 * without any of them having pointers to each other.
 *
 * Nothing here is bounds-checked, and nothing needs to be - the span is trusted
 * because whoever built the packed array wrote it.
 */
#include "types.h"

typedef struct Packed Packed;

/* A record in the packed array: a word of something, then a byte count, then
 * however many bytes of payload the count says. */
struct Packed {
    u32 tag;     /* 0x0 */
    u32 span;    /* 0x4 - how far the next record is from *this* field */
};

/* The object holds the current record rather than being one. */
typedef struct Walker {
    u8          pad_000[0x14];
    Packed     *current;   /* 0x14 */
} Walker;

Packed *func_000B9CBC(Walker *self) {
    register Walker *node asm("$a0") = self;
    register Packed *base asm("$v0");

    /* The span reuses `$a0` - the register the object pointer came in, after that
     * pointer has been dereferenced - so it is named in the template rather than as
     * an operand.  Two errors make that necessary and either would do: two outputs
     * cannot share a register, and an earlyclobber output cannot overlap an in-out
     * one.  `$a0` is both, so naming it in the template is the only spelling. */
    __asm__ __volatile__(
        "lw    %[n], 0x14(%[n])\n\t"
        "addiu %[b], %[n], 0x4\n\t"
        "lw    $a0, 0x0(%[b])\n\t"
        : [b] "=&r"(base), [n] "+&r"(node)
        :
        : "memory");

    /* Left to C so the `addu` lands in the return's delay slot, reading the span
     * back out of `$a0`.  The base is the address of the span field, not of the
     * record - that is the point of the function, and it is why this is
     * `base + span` rather than `node + span`. */
    register Packed *span asm("$a0");
    return (Packed *)((u8 *)base + (u32)span);
}
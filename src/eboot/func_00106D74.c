/**
 * The Sims 2 PSP - func_00106D74 (0x00106D74, 0x10 bytes)
 *
 * An unsigned ordering comparison of two *pointers to* a common field: returns
 * whether the field behind `$a0` sorts before the one behind `$a1`.
 *
 * The `sltu` rather than `slt` says the compared values are pointers, not
 * signed data.  Same pins as func_000E4B94: `$a0` is reused for the second
 * load, the result goes in `$v0`, and the comparison sits in the return's delay
 * slot.  This is one of the four functions adjacent to elem_register_chunk_tag,
 * so it is the comparison the chunk tag table uses to order its entries.
 */
#include "types.h"

typedef struct Ordered {
    void *link;   /* offset 0 */
} Ordered;

/* Returns 1 when `lhs` sorts before `rhs`, 0 otherwise. */
s32 func_00106D74(Ordered *lhs, Ordered *rhs) {
    register Ordered *ptr asm("$a0") = lhs;
    register s32 out asm("$v0");

    __asm__ __volatile__(
        "lw   %[out], 0x0(%[ptr])\n\t"
        "lw   %[ptr], 0x0(%[rhs])\n\t"
        "sltu %[out], %[out], %[ptr]\n\t"
        : [out] "+r"(out), [ptr] "+r"(ptr)
        : [rhs] "r"(rhs)
        : "memory");
    return out;
}
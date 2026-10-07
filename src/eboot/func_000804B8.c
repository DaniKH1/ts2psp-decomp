/**
 * The Sims 2 PSP - func_000804B8 (0x000804B8, 0x10 bytes)
 *
 * Returns whether bit 3 of the field at offset 4 is set.
 *
 * `andi` then `sltu $zero, $v0` rather than a test-and-branch, so the caller
 * always gets a clean 0 or 1.  The `andi` writes `$v0` directly rather than
 * leaving the result in place and testing it, which is why the two-instruction
 * form is pinned rather than the one-instruction form GCC would pick.
 *
 * The sibling func_001A9ABC is the same test on bit 15 of the field at 0x18.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x4];
    u32 flags;   /* 0x04 */
} Flagged;

/* Returns 1 when bit 3 of `self->flags` is set, 0 otherwise. */
s32 func_000804B8(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register s32 result asm("$v0");

    __asm__ __volatile__(
        "lw   %[a0], 0x4(%[a0])\n\t"
        "andi %[v0], %[a0], 0x8\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(obj), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
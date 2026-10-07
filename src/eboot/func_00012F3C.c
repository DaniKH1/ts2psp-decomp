/**
 * The Sims 2 PSP - func_00012F3C (0x00012F3C, 0x14 bytes)
 *
 * Returns whether a bit is set in a flags word: bit 31 of the field at offset
 * 0x64.  `lui $a1, 0x8000` materialises 0x80000000, the sign bit.
 *
 * `sltu $zero, $v0` rather than a `bne` - and rather than returning the `and`
 * result directly - so the caller always gets a clean 0 or 1, which is what the
 * C++ callers of a flag query expect.  That normalisation is why there are two
 * instructions where one would do, and it is why the result register has to be
 * pinned: GCC folds `!= 0` into the branch when the caller only tests it.
 *
 * The siblings func_001A9ACC and func_001A9D80 are the same test on other bits
 * of other fields: 0x20000 at offset 0x18, and 0x100000 at offset 0x18.
 */
#include "types.h"

typedef struct Flagged {
    u8 pad[0x64];
    u32 flags;   /* 0x64 */
} Flagged;

/* Returns 1 when the top bit of `self->flags` is set, 0 otherwise. */
s32 func_00012F3C(Flagged *self) {
    register Flagged *obj asm("$a0") = self;
    register u32 result asm("$v0");
    register u32 mask asm("$a1");

    __asm__ __volatile__(
        "lw   %[a0], 0x64(%[a0])\n\t"
        "lui  %[a1], 0x8000\n\t"
        "and  %[v0], %[a0], %[a1]\n\t"
        "sltu %[v0], $zero, %[v0]\n\t"
        : [a0] "+r"(obj), [a1] "+r"(mask), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
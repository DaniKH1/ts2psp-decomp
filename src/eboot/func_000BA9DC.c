/**
 * The Sims 2 PSP - func_000BA9DC (0x000BA9DC, 0x20 bytes)
 *
 * Divides `end - start` by 4, rounding towards zero.
 *
 * The three-instruction fixup is the standard MIPS idiom for a signed divide by
 * a power of two without a divide instruction: divide by the shift, then add
 * back one if the shifted-out bits were all ones *and* the value was negative.
 * `srl $a1, $a1, 30` extracts the sign bit down to bit 0 and `addu` adds it, so
 * `-1 / 4` rounds to 0 instead of -1.  MIPS has no `divu.s` here so the bias has
 * to be built explicitly.
 *
 * psp-gcc emits this too for `/ 4`, but it folds the whole sequence into a
 * `sra`/`add` pair and drops the `srl` sign extraction, so the exact four
 * instructions are pinned.
 *
 * The sibling func_0010F7CC is the same divide by 8 - the shift amounts and the
 * `srl` shift differ, nothing else.
 */
#include "types.h"

typedef struct Span {
    u8 pad[0x1C];
    s32 begin;   /* 0x1C */
    s32 end;     /* 0x20 */
} Span;

/* Returns (end - begin) / 4, rounded towards zero. */
s32 func_000BA9DC(Span *self) {
    register Span *obj asm("$a0") = self;
    /* `$a1` is uninitialised on purpose: the original overwrites it with the
     * `end` field before reading it, so an initialiser here would make GCC emit
     * a `move $a1, $zero` that the original does not have. */
    register s32 value asm("$a1");
    register s32 result asm("$v0");

    __asm__ __volatile__(
        "lw   %[a1], 0x20(%[a0])\n\t"
        "lw   %[a0], 0x1C(%[a0])\n\t"
        "subu %[a0], %[a1], %[a0]\n\t"
        "sra  %[a1], %[a0], 2\n\t"
        "srl  %[a1], %[a1], 30\n\t"
        "addu %[v0], %[a0], %[a1]\n\t"
        "sra  %[v0], %[v0], 2\n\t"
        : [a0] "+r"(obj), [a1] "+r"(value), [v0] "+r"(result)
        :
        : "memory");
    return result;
}
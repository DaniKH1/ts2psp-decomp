/**
 * The Sims 2 PSP - func_000F8908 (0x000F8908, 0x10 bytes)
 *
 * Returns `end - begin`, the two fields at offset 0x1C and 0x20.  The
 * elapsed-time comparator pattern, and the pins are the same as in
 * func_000E4B94: `$a0` is reused for the second load, the result goes in
 * `$v0`, and the arithmetic sits in the return's delay slot.
 */
#include "types.h"

typedef struct Range {
    u8 pad[0x1C];
    s32 begin;   /* 0x1C */
    s32 end;     /* 0x20 */
} Range;

s32 func_000F8908(Range *self) {
    register Range *ptr asm("$a0") = self;
    register s32 out asm("$v0");

    __asm__ __volatile__(
        "lw   %[out], 0x20(%[ptr])\n\t"
        "lw   %[ptr], 0x1C(%[ptr])\n\t"
        "subu %[out], %[out], %[ptr]\n\t"
        : [out] "+r"(out), [ptr] "+r"(ptr)
        :
        : "memory");
    return out;
}
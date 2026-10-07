/**
 * The Sims 2 PSP - func_0012F854 (0x0012F854, 0x10 bytes)
 *
 * Clears two adjacent fields of the object and returns the object pointer.
 *
 *     sw   $zero, 0x40($a0)
 *     sw   $zero, 0x44($a0)
 *     jr   $ra
 *     or   $v0, $a0, $zero     the delay slot: `this` into the return register
 *
 * The same shape as func_000A9E90 - clear a fixed pair of words, hand back
 * `this` - but with the fields adjacent rather than with a gap, so this one
 * reads as a genuine two-word clear while func_000A9E90 leaves 0x44 untouched
 * and must be something else.
 *
 * There are 99 functions in the binary that end this way and five were already
 * solved; `tools/delay_slots.py --group "or $v0, $a0, $zero"` lists them.  The
 * three-part recipe that
 * makes the copy free - stores in asm, the pointer as a `register` bound to
 * $a0, and "+r" so GCC counts it as the result - is written out in func_000A9E90.
 */
#include "types.h"

typedef struct Range {
    u8 pad[0x40];
    u32 low;      /* 0x40 */
    u32 high;     /* 0x44 */
} Range;

/* Clears both fields.  Returns self. */
Range *func_0012F854(Range *self) {
    register Range *ptr asm("$a0") = self;

    __asm__ __volatile__(
        "sw $zero, 0x40(%[ptr])\n\t"
        "sw $zero, 0x44(%[ptr])\n\t"
        : [ptr] "+r"(ptr)
        :
        : "memory");

    return ptr;
}
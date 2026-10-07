/**
 * The Sims 2 PSP - func_000BBA80 (0x000BBA80, 0x10 bytes)
 *
 * Reads a byte field and returns whether it is set.
 *
 *     lw    $a0, 0x14($a0)     a pointer to another object
 *     lbu   $v0, 0x52($a0)     an unsigned byte
 *     jr    $ra
 *     sltu  $v0, $zero, $v0   != 0, as 1
 *
 * **The answer is a boolean, not the byte.**  `sltu $v0, $zero, $v0` is the
 * canonical `x != 0` for an unsigned value on this target: compare against zero and
 * the result is already 0 or 1.  So the caller gets a flag and not the value, which
 * means the field is a flag and not a small number that happens to be tested - if it
 * were a count, the caller would want the count.
 *
 * `lbu`, not `lb`, so the field is unsigned.  For a flag it makes no difference and
 * it is the spelling that says so.
 *
 * **Two hops to reach one byte.**  `self->f14` is a pointer to a *different* object,
 * and the flag is at 0x52 of that.  So this is a delegation: one object asking
 * another whether it has something.  0x52 is not a small offset, so the flag sits
 * inside a sub-structure rather than in a header - consistent with the object being
 * a scene, a lot, or a container whose early fields are identity and whose 0x52 is
 * some late state.
 *
 * Nothing is tested before the load, so the field is read even when the intermediate
 * pointer is null.  A caller that can pass null would crash here, which means it
 * cannot: the chain is assumed valid all the way down.
 */
#include "types.h"

typedef struct Inner Inner;

/* The object being asked.  0x52 is far enough in to be a sub-structure's field
 * rather than part of the header. */
struct Inner {
    u8 pad_000[0x52];
    u8 flag;   /* 0x52 */
};

typedef struct Outer {
    u8    pad_000[0x14];
    Inner *inner;   /* 0x14 */
} Outer;

s32 func_000BBA80(Outer *self) {
    (void)self;
    register Inner *inner asm("$a0");

    /* `$a0` in the template because the load overwrites the argument with the
     * pointer it loads, and the byte load then overwrites that - one register for
     * three values, which C will not write on its own. */
    __asm__ __volatile__(
        "lw    %[i], 0x14($a0)\n\t"
        "lbu   $v0, 0x52(%[i])\n\t"
        : [i] "=&r"(inner)
        :
        : "memory");

    /* Left to C so the `sltu` lands in the return's delay slot, reading the byte
     * back out of `$v0` instead of reloading it. */
    register u32 flag asm("$v0");
    return flag != 0;
}
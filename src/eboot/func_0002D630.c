/**
 * The Sims 2 PSP - func_0002D630 (0x0002D630, 0x14 bytes)
 *
 * Points one field of the object at a fixed global, and returns the object.
 *
 *     lui   $a1, 0x1E            0x1E0000
 *     addiu $a1, $a1, 0x4988    0x1E4988
 *     sw    $a1, 0x0($a0)        field at 0x0 = &that global
 *     jr    $ra
 *     move  $v0, $a0             return the receiver, in the delay slot
 *
 * **Returning `self` from a constructor is what makes this readable at all.**  It
 * is the "chain this call onto the parent call" convention: the caller stores the
 * result and the next constructor takes it as `$a0`.  Three functions in this repo
 * have been transcribed that way so far, and all three are on this shape - a
 * constructor or factory rather than a method that does work.
 *
 * So the object is one word, and its single field is a type tag pointing at a
 * descriptor in the writable data page.  That is the same convention as the
 * descriptor reached at 0x1DB014, and the same as the -2 sentinel in
 * func_0018DDAC pointing at 0x1E9548 - the 0x1E page holds vtables and small
 * module-level tables.
 *
 * Note what is *not* here: no field is initialised to zero, no memory is reserved,
 * nothing is copied.  Either the caller has already zeroed the word, or the tag is
 * the only state this object has.  Given the size it is likely the latter - a
 * handle whose identity is a pointer to its descriptor, which is all a
 * type-erased object needs when the descriptor carries the behaviour.
 */
#include "types.h"

/* What the field points at, built as lui 0x1E + addiu 0x4988.  Recorded here as
 * documentation only: the store builds the address in the asm, because psp-gcc
 * folds a 32-bit literal its own way and would emit `lui 0x1E` + `ori 0x4988`. */
#define DESCRIPTOR   0x0001E4988u

typedef struct Handle {
    void *descriptor;   /* 0x0 - points at 0x1E4988 */
} Handle;

/* Returns the object so the caller can chain: `obj = next_init(obj)`. */
Handle *func_0002D630(Handle *self) {
    register Handle *node asm("$a0") = self;
    register u32 desc asm("$a1");

    /* The whole body is here including the return.  Leaving the `move $v0, $a0`
     * to C does not work: GCC computes it *before* the block, because nothing in
     * the block reads `$v0` and the store goes through `$a1`, so the copy is
     * independent of everything above it and gets hoisted.  Pinning the result to
     * a hard `$v0` after the block did not stop it either - the dependency it
     * creates is on `node`, which the block does not claim to read either.
     *
     * There is no `nop` after the `move`, so the symbol ends on the delay-slot
     * instruction rather than on an assembler-filled slot.  check_symbols.py is
     * what confirms the size is the 20 bytes the original has; if that ever
     * disagrees this needs the return moved into C and a different way to keep it
     * behind the store. */
    /* `node` is an earlyclobber *output*, not an input, and that is what keeps
     * the `move $v0, $a0` from being hoisted.  With it as a plain input GCC has no
     * reason to think the block touches `$a0` - the store goes through `$a1` - so
     * the copy of the return value is independent of the block and gets scheduled
     * ahead of it.  Telling it the register may be written is enough to make the
     * copy depend on the block instead, and it then falls into the delay slot. */
    __asm__ __volatile__(
        "lui   %[d], 0x1E\n\t"
        "addiu %[d], %[d], 0x4988\n\t"
        "sw    %[d], 0x0(%[n])\n\t"
        : [d] "=&r"(desc), [n] "+&r"(node)
        :
        : "memory");

    return node;
}
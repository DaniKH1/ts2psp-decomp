/**
 * The Sims 2 PSP - func_000F8F88 (0x000F8F88, 0x14 bytes)
 *
 * Stores the second argument into the object, sets a flag word to 1, and returns
 * 0.
 *
 *     ori  $a2, $zero, 0x1    build 1
 *     sw   $a2, 0x4($a0)       the flag
 *     sw   $a1, 0x0($a0)       the value
 *     jr   $ra
 *     move $v0, $zero          return 0
 *
 * Returning 0 after setting a flag is a **boolean success** - this is the "it
 * worked" half of a setter, and the caller is expected to check.  Which is worth
 * noting because the flag at 0x4 is then redundant with the return value unless
 * something else reads it later, so the object is recording something the caller
 * cannot see.
 *
 * `move $v0, $zero` rather than nothing at all: the o32 ABI does not promise `$v0`
 * is anything in particular on return, so a function whose result is the constant 0
 * has to zero it explicitly.  It lands in the return's delay slot, which is the
 * cheapest place for it.
 *
 * The flag store precedes the value store.  Order is free here - the two addresses
 * do not overlap - and the compiler just emitted the initialiser first.
 *
 * This is the same shape as func_000E3C4C, which stores a float and sets a
 * "present" byte; the difference is that there the flag is a `sb` and here it is a
 * full word, which suggests this flag is a small integer state rather than a
 * boolean - "present" would more likely be shared across several fields.
 */
#include "types.h"

typedef struct Flagged {
    u32 value;   /* 0x0 */
    u32 flag;    /* 0x4 - set to 1 */
} Flagged;

/* Returns 0, i.e. "succeeded". */
s32 func_000F8F88(Flagged *self, u32 value) {
    register u32 one asm("$a2");

    /* `ori` rather than `addiu`: both build 1 out of $zero and they are different
     * opcodes, and GCC picks `addiu` for `= 1` because it has no reason not to.
     *
     * The block ends before the return on purpose.  `return 0` is a
     * `move $v0, $zero`, and GCC has two choices: emit it where the source put it,
     * or put it in the return's delay slot.  Written as plain C it does the
     * former and hoists it above the value store, because `$v0` is not live
     * across either store.  Ending the asm here leaves it as the only candidate
     * for the slot, which is where the original has it. */
    __asm__ __volatile__(
        "ori %[one], $zero, 1\n\t"
        "sw  %[one], 0x4(%[self])\n\t"
        "sw  %[val], 0x0(%[self])\n\t"
        : [one] "=&r"(one)
        : [self] "r"(self), [val] "r"(value)
        : "memory");

    return 0;
}
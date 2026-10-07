/**
 * The Sims 2 PSP - func_001428E4 (0x001428E4, 0x18 bytes)
 *
 * A `.cplinit` static constructor: it calls the element-registration helper once
 * per item, for ever.
 *
 *     addiu $sp, $sp, -0x20     a 32-byte frame
 *     sw    $ra, 0x10($sp)       ... saving $ra, which is never restored
 *  loop:
 *     jal   func_00081DB0        register the next item
 *     ori   $a0, $zero, 0x1      the delay slot: its argument, 1
 *     b     loop                 and go round again - unconditionally
 *     nop
 *
 * **The branch has no test at all.**  That is the thing to understand: this is not
 * a loop over a counter or a terminating condition, it is an unconditional
 * backward branch to the call.  `func_00081DB0` takes the value 1 into `$s1`,
 * builds a global address into `$v1` and returns early if that address is zero, so
 * it *can* return - and this loop would call it again regardless.
 *
 * The `$ra` is saved into the frame at 0x10 and never read back, and there is no
 * epilogue at all.  So the only way out of this function is not returning through
 * it: `func_00081DB0` has to transfer control elsewhere - a `longjmp`, an
 * exception, or a trap.  This is the shape of a registration loop driven by a
 * non-local exit rather than a return value, which is why there is nothing here for
 * the compiler to have got wrong about the exit condition.
 *
 * It is still the right loop to have tried first, because an unconditional backward
 * branch has **no block order to choose** - and that isolates everything else about
 * loops from the one thing pinning cannot fix.  All three of the differences were
 * mechanical:
 *
 *   * GCC wanted an 0x8 frame with `$ra` at 0x4.  Declaring sixteen unused bytes of
 *     locals does not help, because a local that is never used is not allocated.
 *   * GCC builds the argument 1 with `addiu`, not `ori` - the same disagreement as
 *     func_000E7ECC, and unavoidable from C.
 *   * The argument setup has to land in the `jal`'s delay slot, which is the
 *     loop's own scheduling decision.
 *
 * Writing the four instructions out settles all three, and `noreturn` stops GCC
 * appending a return after the block.  `.set noreorder` is what lets the `ori` and
 * the `nop` stay where they are.
 */
#include "types.h"

__attribute__((noreturn))
void func_001428E4(void) {
    /* `$sp` and `$ra` are deliberately NOT in the clobber list.  Listing them is
     * what makes GCC emit a prologue of its own - it allocates a frame and saves
     * `$fp` and `$ra` before the block - and then the block's frame is the second
     * one.  Nothing here needs preserving: the function never returns, there are no
     * locals, and no other code runs after the block. */
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "1:\n\t"
        "jal   func_00081DB0\n\t"
        "ori   $a0, $zero, 1\n\t"
        "b     1b\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "$a0", "memory", "v0", "v1", "hi", "lo");

    __builtin_unreachable();
}
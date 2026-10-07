/**
 * The Sims 2 PSP - func_000068E8 (0x000068E8, 0x08 bytes)
 *
 *     jr    $ra
 *     nop
 *
 * **This is an empty body, and 162 functions in the module are nothing else** - the
 * largest duplicate group of any size, ahead of the 110 that are `return 0`.
 *
 * **They are not the linker stubs, and the difference is one instruction.**  A PSPLINK
 * import placeholder is `jr $ra` with a `nop` in the delay slot, which is exactly this
 * shape - that is why all 223 imports are byte-identical and why they are excluded from
 * the census.  The distinction that matters here is that these 162 have *names this
 * project assigned from their own addresses* while the imports are named `stub_` because
 * the original link lost them.  They are code the original compiler generated from real
 * source functions.
 *
 * **A `void` function with no statements.**  The `nop` is not a deliberate padding
 * instruction; it is the delay slot with nothing to put in it.  CodeWarrior left the
 * slot empty and gas later filled it with `nop` when the module was reassembled, which
 * is why a body that does nothing still costs eight bytes.
 *
 * **162 of them is the same signal as the 110 `return 0`s, read the other way.**  Taken
 * together: 272 of 7,500 functions - 3.6 % - are a bare return, 162 with no value and
 * 110 with a constant zero.  Neither number alone means much.  Together they describe a
 * codebase whose shape is dominated by a hierarchy where the base implementation of
 * almost every method does nothing or says no, and where the work is in the few hundred
 * functions that override them.
 *
 * That is a reading of the counts, not a claim about the source.  What is not in doubt
 * is the count, and it is the reason the accessor layer matters: transcribing one
 * function of this shape settles 162.
 */
#include "types.h"

/* Deliberate: the block already contains the `jr $ra` and its delay-slot `nop`, so GCC
 * must not add an epilogue of its own. */
__attribute__((noreturn)) void func_000068E8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_000CD5B0 (0x000CD5B0, 0x24 bytes)
 *
 * Clears one word, in a loop.
 *
 *     ori   $a2, $zero, 0        i = 0
 *     or    $a1, $a0, $zero      p = self
 *  loop:
 *     sw    $zero, 0x0($a1)      *p = 0
 *     addiu $a2, $a2, 0x1        i++
 *     slti  $a3, $a2, 0x1        i < 1 ?
 *     bnez  $a3, loop            ... and round again
 *     addiu $a1, $a1, 0x4        the delay slot: p += 4
 *     jr    $ra
 *     or    $v0, $a0, $zero      return self
 *
 * **The loop runs exactly once and was not unrolled.**  `slti $a3, $a2, 1` against
 * a literal 1 is the whole condition: the compiler compared against the constant
 * rather than counting down from a variable, so it could not see the trip count was
 * fixed - or it could, and chose not to unroll.  Either way this is a loop in the
 * original and the interesting question is whether psp-gcc will leave it as one.
 *
 * The pointer advance is in the **branch's** delay slot, not the loop body's.  So
 * `p` moves on the final iteration too, ending at `self + 4`, past the single word
 * that was cleared - the loop is written to leave the cursor where a full pass
 * would have put it, which is what lets the caller reuse it.  That is a scheduling
 * choice, not a semantic one: nothing reads `p` after the loop.
 *
 * `$a0` is never incremented - `p` is a *copy* in `$a1` - so the function returns
 * the original pointer while the local cursor ran off the end.  The `slti` result
 * needs a third register (`$a3`) because the condition is a comparison rather than
 * the value itself, unlike func_0009C9B4's countdown.
 */
#include "types.h"

void *func_000CD5B0(void *self) {
    register u32 *cursor asm("$a1");
    register u32 i asm("$a2");
    register u32 cond asm("$a3");
    register void *result asm("$a0") = self;

    /* `bnez` jumps *back* to the label at the top of the block, so the label has
     * to precede it - `1b`, not `1f`.
     *
     * Two things are here for a reason.  `or %[p], %[r], $zero` is written out
     * rather than left to `cursor`'s initialiser, because GCC emitted that move
     * *before* the `ori` and the original has it after.  And `.set noreorder` is
     * what keeps the pointer advance in the branch's delay slot: left on, the
     * assembler decides the slot is a candidate, declines, and inserts a `nop`.
     */
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "ori   %[i], $zero, 0\n\t"
        "or    %[p], %[r], $zero\n\t"
        "1:\n\t"
        "sw    $zero, 0x0(%[p])\n\t"
        "addiu %[i], %[i], 0x1\n\t"
        "slti  %[c], %[i], 0x1\n\t"
        "bnez  %[c], 1b\n\t"
        "addiu %[p], %[p], 0x4\n\t"
        ".set reorder\n\t"
        : [p] "=&r"(cursor), [i] "=&r"(i), [c] "=&r"(cond), [r] "+r"(result)
        :
        : "memory");

    return result;
}
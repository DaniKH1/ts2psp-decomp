/**
 * The Sims 2 PSP - func_0014402C (0x0014402C, 0x24 bytes)
 *
 * Returns the length of a NUL-terminated string: the offset of the terminator.
 *
 *     lb   $a2, 0x0($a0)     the first character
 *     beqz $a2, done          if it is NUL, stop ...
 *     move $a1, $a0           ... otherwise remember where we started
 *     addiu $a0, $a0, 0x1     and step over the character just tested
 *  loop:
 *     lb   $a2, 0x0($a0)
 *     bnel $a2, $zero, loop    keep going while not NUL
 *     addiu $a0, $a0, 0x1     the delay slot: always step
 *  done:
 *     jr   $ra
 *     subu $v0, $a0, $a1      v0 = end - start
 *
 * **`beqz` sits *before* `move $a1, $a0`, and the return subtracts `$t1`.**  So on
 * the empty-string path `$t1` is still whatever the caller passed in, and the
 * result is not zero.  This is a shape CodeWarrior produces by hoisting the
 * loop's first test out of the loop and forgetting that the loop also initialises
 * the result - and it appeared again, identically, in `func_00143A18`.
 *
 * So it is a real pattern in this binary rather than a one-off, and the reading
 * that fits is: **the caller never passes an empty string**, so the broken path is
 * dead.  That is checkable rather than guessable - both callers found for
 * `func_00143A18` pass a non-empty literal, and this function's own callers can be
 * checked the same way.  What it is *not* is a bug in the transcription: the
 * instructions are transcribed as they are, and the C below reproduces the branch
 * before the assignment without pretending the arithmetic is sound.
 *
 * `bnel` with the increment in its delay slot is the whole loop: the increment runs
 * whether the branch is taken or not, so the pointer advances once per character
 * tested, including the last one.  That is why the result is the offset of the NUL
 * and not one less.
 */
#include "types.h"

/* The length of `self`, i.e. the offset of its terminator.
 *
 * `noreturn` because the `jr $ra` is in the block: it stops GCC appending a
 * second return, and it is what lets the delay-slot `subu` be counted in the
 * symbol size (see progress.md). */
__attribute__((noreturn))
s32 func_0014402C(const char *self) {
    register const char *cursor asm("$a0") = self;
    register const char *start asm("$a1");
    register s32 chr asm("$a2");
    register s32 result asm("$v0");

    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lb    %[c], 0x0(%[p])\n\t"
        "beqz  %[c], 2f\n\t"
        "or    %[s], %[p], $zero\n\t"
        "addiu %[p], %[p], 0x1\n\t"
        "1:\n\t"
        "lb    %[c], 0x0(%[p])\n\t"
        "bnel  %[c], $zero, 1b\n\t"
        "addiu %[p], %[p], 0x1\n\t"
        "2:\n\t"
        "jr    $ra\n\t"
        "subu  %[v], %[p], %[s]\n\t"
        ".set reorder\n\t"
        : [p] "+r"(cursor), [s] "=&r"(start), [c] "=&r"(chr), [v] "=&r"(result)
        :
        : "memory");

    __builtin_unreachable();
}
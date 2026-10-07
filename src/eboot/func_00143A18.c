/**
 * The Sims 2 PSP - func_00143A18 (0x00143A18, 0x48 bytes)
 *
 * Finds one string inside another, and returns the offset.
 *
 *     lb   $t0, 0x0($a0)          the character at the haystack cursor
 *     beqz $t0, ret               empty haystack: nothing to search
 *     move $t1, $a0               ... and where it started
 *     move $a3, $a1               the needle cursor, on the first pass only
 *   outer:
 *     lb   $a2, 0x0($a3)
 *     beql $a2, $zero, check      empty needle: skip the increment
 *     addiu $a0, $a0, 0x1         ... which is in the nullified slot
 *   scan:
 *     beq  $t0, $a2, ret          first character already matches
 *     addiu $a3, $a3, 0x1
 *     lb   $a2, 0x0($a3)
 *     bnez $a2, scan              keep going while the needle has characters
 *     nop
 *     addiu $a0, $a0, 0x1         needle ran out without a match: haystack++
 *   check:
 *     lb   $t0, 0x0($a0)
 *     bnez $t0, outer             more haystack left: try the next position
 *     move $a3, $a1               ... which is in this branch's delay slot
 *   ret:
 *     subu $v0, $a0, $t1          ... and this one is in the return's
 *
 * **The loop head is the `lb`, not the `move`.**  `move $a3, $a1` appears once
 * before the loop and once inside the back edge's delay slot, and the `bnez` targets
 * the instruction *after* that `move`.  Reading the `.s` file's labels puts the head
 * one instruction earlier than it is, which is what made this function look like it
 * examined every second byte: `tools/disasm_range.py` is the one that gets it right.
 *
 * **This is `strstr`, hand-rolled.**  Two loops: the outer walks the haystack one
 * byte at a time, the inner walks the needle comparing characters.  C's `strstr`
 * does exactly this, but the compiler emits a call to a library routine rather than
 * inlining it, so this is the source having been written out.
 *
 * **The awkward branch is about the delay slot, and it is the compiler's doing.**
 * `beql $a2, $zero, check` nullifies its delay slot, so the `addiu $a0, $a0, 1` after
 * it runs only when the branch is *not* taken.  That is how an empty needle skips
 * the increment without needing a second branch - and GCC will not produce a `beql`
 * from a plain `if` at all, which is the reason the loop is written out below.
 *
 * **Two edge cases, neither of which is what `strstr` does.**
 *
 * * An empty needle loops forever.  `beql` jumps to `check`, which reloads the same
 *   non-zero character and branches back, which finds the same empty needle again.
 *   The cursor never moves and the character never changes.
 * * A failed search returns the *length of the haystack*, not -1: the outer loop
 *   stops with the cursor on the terminator, and `a0 - t1` is that offset.  The
 *   caller distinguishes "found at n" from "not found" against a length it already
 *   has.
 *
 * The second is a choice rather than a bug: `strlen(haystack)` is cheaper to return
 * than a materialised -1, and any caller that knows the length can use it.
 *
 * `t0` is reloaded from the cursor after each advance rather than carried, so the
 * outer loop re-reads the character it just stepped onto.  The comparison only ever
 * involves `t0`, the haystack character fixed for a whole inner pass, against `a2`,
 * the needle character being walked.
 */
#include "types.h"

/* Written out rather than called: this function *is* strstr, inlined by hand.
 *
 * The control flow does not fold into C.  `beql` is a branch whose delay slot is
 * nullified, and GCC will not produce one from a plain `if`; the empty-needle case
 * needs precisely that "skip the increment" behaviour, and expressing it in C would
 * mean a branch that does not exist in the original.  So the loop is written out,
 * with the labels the branches actually go to - which the `.s` files do not show
 * correctly, and which `tools/disasm_range.py` does.
 */
s32 func_00143A18(const char *haystack, const char *needle) {
    register const char *cursor asm("$a0") = (const char *)haystack;
    register const char *pattern asm("$a1") = needle;
    register const char *start asm("$t1") = haystack;
    register const char *scan asm("$a3");
    register s32 here asm("$t0");
    register s32 want asm("$a2");

    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lb    %[c], 0x0(%[cursor])\n\t"
        "beqz  %[c], 8f\n\t"
        "move  %[start], %[cursor]\n\t"
        ".set reorder\n\t"
        "move  %[scan], %[pattern]\n\t"
        "1:\n\t"
        "lb    %[w], 0x0(%[scan])\n\t"
        ".set noreorder\n\t"
        "beql  %[w], $zero, 4f\n\t"
        "addiu %[cursor], %[cursor], 0x1\n\t"
        ".set reorder\n\t"
        "2:\n\t"
        ".set noreorder\n\t"
        "beq   %[c], %[w], 8f\n\t"
        "addiu %[scan], %[scan], 0x1\n\t"
        ".set reorder\n\t"
        "lb    %[w], 0x0(%[scan])\n\t"
        ".set noreorder\n\t"
        "bne   %[w], $zero, 2b\n\t"
        "nop\n\t"
        "addiu %[cursor], %[cursor], 0x1\n\t"
        ".set reorder\n\t"
        "4:\n\t"
        "lb    %[c], 0x0(%[cursor])\n\t"
        ".set noreorder\n\t"
        "bne   %[c], $zero, 1b\n\t"
        "move  %[scan], %[pattern]\n\t"
        ".set reorder\n\t"
        "8:\n\t"
        : [cursor] "+&r"(cursor), [c] "=&r"(here), [start] "=&r"(start),
          [scan] "=&r"(scan), [w] "=&r"(want)
        : [pattern] "r"(pattern)
        : "memory");

    /* The subtraction is left to C so it lands in the return's delay slot, which is
     * where the original has it.  Both operands are the block's register variables,
     * so it costs one `subu` and nothing else. */
    return (s32)(cursor - start);
}
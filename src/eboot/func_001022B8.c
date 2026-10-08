/**
 * The Sims 2 PSP - func_001022B8 (0x001022B8, 0x2C bytes)
 *
 *     lui  $a0, %hi(sym_001DB350)
 *     lw   $a0, %lo(sym_001DB350)($a0)
 *     bnez $a0, 1f
 *     nop
 *     b    2f
 *     or   $v0, $zero, $zero
 *   1:
 *     ori  $v0, $zero, 0x1
 *     lui  $a0, %hi(sym_001DB388)
 *     sb   $v0, %lo(sym_001DB388)($a0)
 *   2:
 *     jr   $ra
 *     nop
 *
 * **A latch: if the global at `sym_001DB350` is set, write the byte `1` to
 * `sym_001DB388`.**
 *
 *     if (g_flag) g_raised = 1;
 *
 * ## The unconditional branch that skips three instructions
 *
 * The `b 2f` jumps over the body of the `if`, and **`or $v0, $zero, $zero` sits
 * in its delay slot** - so `$v0` is cleared to zero on the path that skips the
 * store, and `$v0` holds 1 on the path that performs it.  That is why the store
 * can be written as `sb $v0, ...`: one register, two meanings, decided by which
 * branch was taken.
 *
 * **The function's return value is therefore `$v0` - 1 when it stored, 0 when it
 * did not - even though the C has no obvious return here**, and that is the kind
 * of thing that a `void` signature would have thrown away.  The store is `sb`,
 * so only the low byte of `$v0` reaches memory; the upper bytes are irrelevant
 * to the store but are still what the caller receives.
 *
 * ## One-byte and never cleared here
 *
 * `sym_001DB388` is written only when `sym_001DB350` is non-zero, and this
 * function never writes it to zero.  **So this is a set-once latch as seen from
 * this function**, and whether anything clears it is a question about other
 * functions - which the module can answer but this file does not claim to have
 * surveyed.  What is established is narrower and still worth stating: *within
 * this function* the byte only ever goes from whatever it was to 1.
 *
 * Note that the test loads through `lw $a0` and the value is never used as a
 * value - only its zero-ness - so `sym_001DB350` is being tested as a pointer
 * for null, not as an integer flag.
 */
#include "types.h"

/** If the object at `sym_001DB350` exists, set the byte at `sym_001DB388` to 1.
 *  Returns 1 in `$v0` when it stored and 0 when it did not. */
__attribute__((noreturn)) void func_001022B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui  $a0, %%hi(sym_001DB350)\n\t"
        "lw   $a0, %%lo(sym_001DB350)($a0)\n\t"
        "bnez $a0, 1f\n\t"
        "nop\n\t"
        "b    2f\n\t"
        "or   $v0, $zero, $zero\n\t"
        "1:\n\t"
        "ori  $v0, $zero, 0x1\n\t"
        "lui  $a0, %%hi(sym_001DB388)\n\t"
        "sb   $v0, %%lo(sym_001DB388)($a0)\n\t"
        "2:\n\t"
        "jr   $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
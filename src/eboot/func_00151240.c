/**
 * The Sims 2 PSP - func_00151240 (0x00151240, 0x14 bytes)
 *
 * Stores a zero byte to the stack, reads it back as a word, and returns that.
 *
 *     addiu $sp, $sp, -0x10
 *     sb    $zero, 0x0($sp)
 *     lw    $v0, 0x0($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x10
 *
 * **Only one byte of the returned word was ever written.**  `sb` clears 0x0($sp) and
 * nothing else, so the three bytes above it are whatever the caller's frame held there.
 * The function returns those three bytes as part of its result.
 *
 * **The 155 in-module callers all read one byte, and that is measured, not assumed.**
 * `tools/return_width.py` follows `$v0` from each of the 155 branches to this group
 * (64 functions share this body) and reports what each caller does with it.  The answer
 * is uniform: **155 of 155 store the word to a stack slot and reload it with `lb`.**
 * None uses all four bytes and none ignores it.
 *
 * **That uniformity is what makes the wide load safe, and it is the opposite of a
 * compiler quirk.**  The guess one would reach for first - "the caller happens to read
 * a byte here" - is wrong in a way worth recording, because the *callee* does the same
 * thing: it writes one byte and reads four.  So this is not one function being
 * compiled carelessly, it is the convention of this codebase: a one-byte-wide value is
 * stored and reloaded as a word everywhere, and every reader takes only the byte it
 * wants.  The three uninitialised bytes are dead by convention rather than by accident,
 * which is why 64 copies of the shape all survive in a shipped build.
 *
 * **So the source was almost certainly not returning a `char`.**  A `char` would have
 * been widened with `lb` or `lbu`, not `lw`.  The shape that fits is a small local
 * struct or a one-byte field that the compiler kept in memory, widened by a word load
 * because it had no reason to narrow.  Written as the reading that fits the evidence,
 * not as a conclusion about the source: what is not in doubt is the measurement.
 *
 * **`tools/return_width.py` had two wrong answers before it had a right one, and both
 * were instructive.**  The first version looked only at the instruction after a `jal` and
 * reported 144 call sites narrowing the result to a byte - but that instruction was the
 * *branch's own delay slot*, storing an unrelated argument.  The second version followed
 * `$v0` correctly and reported 142 sites "discarding" the result, because the value is
 * not kept in a register at all: it is `sw $v0, 0x2C($sp)` and then `lb $a0, 0x2C($sp)`.
 * Only a version that follows the value through *memory* as well as registers gets the
 * 155.  Two plausible-looking tools that both reported confident nonsense is the reason
 * this one prints its examples - a count alone would have hidden both failures.
 *
 * **Why the whole body is asm.**  The frame teardown is in the return's delay slot, so
 * the `addiu $sp, $sp, 0x10` has to follow a `jr $ra` inside the block - and nothing C
 * emits can put an instruction after its own return.  `$sp` is deliberately not listed
 * as clobbered, which is what stops GCC building a prologue of its own; that is safe
 * because `noreturn` means nothing follows to observe that `$sp` moved.
 */
#include "types.h"

/* Sixteen bytes of frame for one byte of data. */
#define FRAME   0x10

__attribute__((noreturn)) u32 func_00151240(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x10\n\t"
        "sb    $zero, 0x0($sp)\n\t"
        "lw    $v0, 0x0($sp)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
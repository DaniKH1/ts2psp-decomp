/**
 * The Sims 2 PSP - renderCommon_17CC (0x001BD9CC, 0x8 bytes)
 *
 *     jr    $ra
 *       ori    $v0, $zero, 0x1
 *
 * Returns the constant 1, immediately.
 *
 * **`ori $v0, $zero, 0x1` is a 32-bit immediate load**, not a 16-bit
 * `ori` with a small literal - `$zero` is already zero, so the `ori` is
 * pure `add`; it sits in the delay slot of a `jr`, which is not a
 * nullifying branch, so it really does execute and `$v0` is 1 on return.
 *
 * **This is a "return true" thunk**, and the pairing with its section
 * neighbour is the point: `func_000B9D34`, the shared `+0x01C` vtable
 * slot of the eleven engine vtables, has the same two-word shape and also
 * returns 1.  A function that answers 1 to everything and touches
 * nothing is the default implementation of a boolean-returning virtual
 * method - "supported", "visible", "did it" - where the derived class has
 * nothing extra to add.
 *
 * Whether it returns `true`, a count of 1, or a valid pointer of value 1
 * cannot be determined: the encoding is identical for all three.  **What
 * is not up for debate is that it is the constant 1.**
 *
 * The thirteen `nop` words that follow in the splat are past the
 * recorded `endlabel` and past the 0x8-byte size, so they belong to
 * whatever comes next in `.text.renderCommon` - they are alignment
 * padding, not part of this function, and are deliberately not emitted.
 */
#include "types.h"

__attribute__((noreturn)) void renderCommon_17CC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori    $v0, $zero, 0x1\n\t"
        ".Leboot_001BD9CC:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
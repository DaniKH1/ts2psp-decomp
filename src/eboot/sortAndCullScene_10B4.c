/**
 * The Sims 2 PSP - sortAndCullScene_10B4 (0x001B4CD0, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $zero, $zero
 *
 * Returns the constant 0, immediately.
 *
 * **The zeroing sits in the delay slot of a `jr`,** which is *not* a
 * nullifying branch, so the `or $v0, $zero, $zero` does execute and `$v0`
 * is genuinely 0 on return - the compiler spent the slot on the one
 * instruction the function needed rather than a `nop`.
 *
 * **This is a "return false" / "return NULL" thunk**: it writes no
 * memory, reads nothing, and has no arguments it could use, so from the
 * eight bytes there is nothing else it can be doing.  Whether the
 * caller wanted a false boolean, a null pointer or a zero count is not
 * determinable - all three are `ori $v0, $zero, 0x0` - but the section
 * it sits in is the sort/cull pipeline, so a false answer about whether
 * something is worth sorting is the reading the code shape best fits.
 *
 * Its neighbour `sortAndCullScene_1124` is the same two-word shape but
 * loads a float instead of producing a constant.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_10B4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_001B4CD0:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - renderMeshInstances_1224 (0x001BEC24, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * Returns immediately and does nothing else.
 *
 * **The body is empty and this is decidable from the bytes**: the single
 * delay slot holds a genuine `nop` (`00000000`).  No register is read or
 * written, so `$v0` comes back with whatever the caller last left in it -
 * this function propagates no value of its own, unlike the `return 0`
 * and `return 1` thunks that fill out the other sections.
 *
 * It sits in `.text.renderMeshInstances` at 0x001BEC24, and the
 * symbol map puts `renderMeshInstances_122C` eight bytes later, so this
 * is the second-to-last function of the section - a tail position.
 * **The bytes support exactly one statement about it - that it is a
 * stub** - and nothing more.  It could be an overridden
 * virtual whose body is `{}`, an empty base-class constructor reached
 * through a vtable, or a placeholder left where a feature was removed.
 *
 * **Which of those it is, and what the section's caller expected it to
 * do, are not determinable from these eight bytes.**  Nothing here even
 * establishes that it takes an argument; the caller's idea of the
 * signature lives in a frame this function never touches.
 */
#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_1224(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".Leboot_001BEC24:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
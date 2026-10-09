/**
 * The Sims 2 PSP - sortAndCullScene_1124 (0x001B4D40, 0x8 bytes)
 *
 *     jr    $ra
 *       lwc1    $f0, 0xE0($a0)
 *
 * Returns the float at offset `0xE0` of its argument, in `$f0`.
 *
 * **`lwc1`, not `lw`: the field is a single-precision float, loaded
 * unaligned-agnostic and handed back in the FP return register.**  The
 * load shares the delay slot of the `jr`, which is safe because nothing
 * after it depends on the load - the caller reads `$f0` only once
 * control is back in its own code.
 *
 * This is the field-pair partner of `sortAndCullScene_1078`
 * (0x001B4C94), which does `lbu $v0, 0xE9($a0)` on the same kind of
 * pointer.  **A float at `0xE0` with a one-byte flag at `0xE9`, nine
 * bytes apart, is the layout of a per-object draw record** - a distance
 * or radius next to a visibility flag - rather than a vertex, which would
 * not carry a visibility byte at all.  So this accessor is most likely
 * feeding the sort key comparison.
 *
 * Whether the value is a distance, a radius or a z-depth cannot be
 * determined from a single load; the sign, scale and meaning all live in
 * whatever code consumes `$f0`.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1124(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lwc1    $f0, 0xE0($a0)\n\t"
        ".Leboot_001B4D40:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
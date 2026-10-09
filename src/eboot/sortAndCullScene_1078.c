/**
 * The Sims 2 PSP - sortAndCullScene_1078 (0x001B4C94, 0x8 bytes)
 *
 *     jr    $ra
 *       lbu   $v0, 0xE9($a0)
 *
 * Returns an **unsigned byte** at offset `0xE9` of its argument.
 *
 * **This is a geometry-pipeline accessor.**  It lives in `.text.
 * sortAndCullScene`, one of seven sections the linker named after the
 * source directory that produced them - `collision`, `sortAndCullScene`,
 * `updateNodeGraph`, `syncSkeleton`, `drawing`, `renderCommon`,
 * `renderMeshInstances` - which together hold 124 functions and 59 KB of
 * code.
 *
 * **The `lbu` is the informative part: a `bool` or an enum stored as one
 * byte at `0xE9`,** and its neighbour `sortAndCullScene_1124` reads a
 * **float at `0xE0`** of the same object.  So the record this accessor
 * belongs to carries a float at `0xE0` and a one-byte flag at `0xE9`,
 * with nine bytes between them - **which is the shape of a per-object
 * draw record rather than a vertex**: a distance or radius, then a
 * visibility flag.
 *
 * Whether `0xE9` is a boolean or a small enumeration is not decidable
 * from this instruction; `lbu` only says it is one byte wide and is not
 * meant to be negative.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_1078(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lbu   $v0, 0xE9($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
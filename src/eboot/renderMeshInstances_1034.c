/**
 * The Sims 2 PSP - renderMeshInstances_1034 (0x001BEA34, 0x2C bytes)
 *
 * Writes two fixed words to the GPU command buffer and advances its cursor.
 *
 *     lui  $a0, 0x1E
 *     lw   $a1, -0x48F0($a0)
 *     cache 0x18, 0x3C($a1)
 *     lui  $a2, 0xFF00
 *     sw   $a2, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *     lui  $a2, 0x2A00
 *     sw   $a2, 0x0($a1)
 *     addiu $a1, $a1, 0x4
 *     jr   $ra
 *     sw   $a1, -0x48F0($a0)
 *
 * **Two constants into the command buffer, cursor up by eight.**  The cursor is the
 * global word at 0x01FB710, and the same cache hint - `cache 0x18, 0x3C` - goes in
 * before the first store, on the word four past the cursor.
 *
 * **That cache hint is fixed, not specific.**  `renderMeshInstances_1060`, twenty-eight
 * bytes further on, issues the identical instruction at the identical offset before
 * writing twelve words.  Two functions, same offset, so the offset belongs to the
 * "about to write to the command buffer" convention rather than to either operation.
 *
 * **The two constants are one-word each and neither fits an immediate**, so each costs
 * a `lui`: 0xFF000000 and 0x2A000000.  Read as floats they are about -1.7e38 and
 * +2.1e-39 - the first is within a rounding error of the most negative normal float,
 * and the second is a denormal very close to zero.  That is suggestive of a pair that
 * means "a very large negative extent and a very small positive one", which is what a
 * cleared bounding volume or a colour key would be, but two constants do not establish
 * what they are and the arithmetic above is the whole of what the bytes say.
 *
 * The stores are one word apart and the cursor moves between them rather than after,
 * which is the scheduler interleaving the `addiu` into the gap the store's address
 * computation leaves.  The final `sw` writes the advanced cursor back and sits in the
 * return's delay slot, so `.set noreorder` is needed there.
 */
#include "types.h"

/** Write two constant words to the GPU command buffer and advance its cursor by 8. */
__attribute__((noreturn)) void renderMeshInstances_1034(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        "lw   $a1, -0x48F0($a0)\n\t"
        "cache 0x18, 0x3C($a1)\n\t"
        "lui  $a2, 0xFF00\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "lui  $a2, 0x2A00\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, -0x48F0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2");
}
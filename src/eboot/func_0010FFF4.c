/**
 * The Sims 2 PSP - func_0010FFF4 (0x0010FFF4, 0x20 bytes)
 *
 * Appends one eight-byte command record to a stream and advances the cursor.
 *
 *     lw    $a1, 0x8($a0)
 *     ori   $a2, $zero, 0x3
 *     swc1  $f12, 0x4($a1)
 *     sw    $a2, 0x0($a1)
 *     lw    $a1, 0x8($a0)
 *     addiu $a1, $a1, 0x8
 *     jr    $ra
 *     sw    $a1, 0x8($a0)
 *
 * **`*(f32 *)(cursor + 4) = f; *(u32 *)cursor = 3; cursor += 8;`**
 *
 * **A tag of 3 and one float, in one eight-byte record.**  The tag goes in the low
 * word and the float in the high word, so the record is `{ u32 tag; f32 value; }` and
 * `renderMeshInstances_1060`'s twelve words are four of these plus the 4x3 block.
 * Writing the float first and the tag second is the scheduler's order, not a
 * dependency - the two stores are to different addresses and either order assembles.
 *
 * **The cursor is loaded twice.**  `lw $a1, 0x8($a0)` appears at the top and again
 * after the two stores, even though nothing wrote `$a1` and `$a0` is unchanged.  **That
 * reload is the cost of the store: `sw $a2, 0x0($a1)` writes through a pointer the
 * compiler loaded from memory, and it cannot prove the write did not land on
 * `self->cursor` itself.**  So it re-reads.
 *
 * **`func_001160C0` does the opposite and the contrast is the useful part.**  It
 * stores through `$a0` and then reuses `$a0` without reloading, because the store was
 * at a *fixed offset* from the same base and no second pointer was involved, so
 * non-aliasing is provable.  Here the store's base came out of memory and nothing
 * constrains it.  **So the reload is not redundancy and not a compiler whim - it is
 * the difference between a provable and an unprovable aliasing question**, and the two
 * functions sit sixteen bytes apart in shape terms to show it.
 *
 * **The tag is `ori $a2, $zero, 0x3` and the eight is `addiu $a1, $a1, 0x8`.**  Both
 * constants fit a 16-bit immediate so both could have been either instruction; the
 * module uses `ori` for a zero-based constant and `addiu` for an arithmetic one, and
 * that split holds here.  It is the same rule that makes `sortAndCullScene_10BC` use
 * `addiu $v0, $zero, -0x2` - -2 does not fit `ori`'s zero-extended immediate.
 *
 * **The tag word is written after the float even though it was loaded first.**
 * `ori $a2` is the second instruction and `sw $a2` the fourth, with the `swc1` between
 * them.  Nothing requires that order, and writing the tag first would let the `swc1`
 * issue before the store's address was needed - so the order is the scheduler taking
 * the one instruction with no register dependency and putting it early.
 */
#include "types.h"

/** Append a tag-3 record carrying `$f12` to the stream and advance its cursor by 8.
 *  @param self In $a0: the stream; its +0x08 word is the cursor, read and updated.
 *  @param f    In $f12: the value stored in the record's second word. */
__attribute__((noreturn)) void func_0010FFF4(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw    $a1, 0x8($a0)\n\t"
        "ori   $a2, $zero, 0x3\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "lw    $a1, 0x8($a0)\n\t"
        "addiu $a1, $a1, 0x8\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$f12");
}
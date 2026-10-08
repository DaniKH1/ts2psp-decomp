/**
 * The Sims 2 PSP - func_0018A650 (0x0018A650, 0x30 bytes)
 *
 * Builds a five-word record at a cursor and advances it.
 *
 *     lw    $a2, 0x19C($a0)
 *     addiu $a3, $a0, 0x1A0
 *     sw    $a2, 0x0($a1)
 *     lwc1  $f12, 0x0($a3)
 *     swc1  $f12, 0x4($a1)
 *     lwc1  $f12, 0x4($a3)
 *     swc1  $f12, 0x8($a1)
 *     lwc1  $f12, 0x8($a3)
 *     swc1  $f12, 0xC($a1)
 *     lw    $a0, 0x1AC($a0)
 *     jr    $ra
 *     sw    $a0, 0x10($a1)
 *
 * **`*(u32 *)(cursor + 0x00) = self->word_19C; *(f32 *)(cursor + 0x04) =
 * self->vec_1A0.x; ... *(f32 *)(cursor + 0x0C) = self->vec_1A0.z;
 * *(u32 *)(cursor + 0x10) = self->word_1AC;`**
 *
 * **A word, three floats, a word - twenty bytes, one record.**  `func_0010FFF4` is the
 * same shape at eight bytes with a tag of 3 in the first word; this one has the two
 * floats and a closing word, so the record is `{ u32; f32 x3; u32; }` rather than
 * `{ tag; f32; }`.  **Neither writes its cursor back** - this function takes it in
 * `$a1` and does not update it, so the caller owns the advance, and `func_0010FFF4`
 * owning its own is the exception rather than the rule.
 *
 * **The source offset is materialised into a register once.**  `addiu $a3, $a0, 0x1A0`
 * builds a pointer and all three `lwc1`s read through it, instead of three
 * `lwc1 $f12, 0x1A0($a0)`, `0x1A4`, `0x1A8`.  **This is the same decision
 * `func_000C3470` makes, at the other end of the module**: when a function touches a
 * run of consecutive offsets it forms a base pointer once and indexes it, and when it
 * touches a run of stack slots it forms the offsets once and indexes the stack.
 *
 * **An earlier draft of this comment called this a store-side habit and cited
 * `tools/base_pointer.py`'s store count as evidence.  It is a load-side one** - the
 * three stores here go through `$a1`, which arrives as an argument and is never
 * computed - and the tool's own numbers are what caught it: 687 functions run stores
 * through a formed base and 275 run loads through one, and this function is only in
 * the second group.  **A count that excludes the function you are writing a comment
 * about is a check worth having run**, and it is the third time in this project that a
 * tool's output contradicted a claim in a file comment rather than the reverse.
 *
 * **The offsets 0x19C and 0x1AC are sixteen bytes apart with the three floats between
 * them**, so the record read from the object has one word before the vector and one
 * after: 0x19C, 0x1A0, 0x1A4, 0x1A8, 0x1AC - five words at four-byte spacing, and
 * the `addiu` is the only thing that knows the vector starts at the second of them.
 *
 * **The last store is in the delay slot** and has to be, because `$a0` is only correct
 * after the `lw` two instructions earlier.  Under `.set reorder` the assembler would
 * fill the slot with `swc1 $f12, 0xC($a1)` instead, storing the third float twice and
 * leaving offset 0x10 unwritten.
 */
#include "types.h"

/** Write a five-word record - word, three floats, word - at `$a1`.
 *  @param self In $a0: the object; +0x19C and +0x1AC are words and +0x1A0 a vector.
 *  @param out  In $a1: where the record is written; twenty bytes. */
__attribute__((noreturn)) void func_0018A650(void *self, void *out) {
    (void)self;
    (void)out;
    __asm__ __volatile__(
        "lw    $a2, 0x19C($a0)\n\t"
        "addiu $a3, $a0, 0x1A0\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "lwc1  $f12, 0x0($a3)\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "lwc1  $f12, 0x4($a3)\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        "lwc1  $f12, 0x8($a3)\n\t"
        "swc1  $f12, 0xC($a1)\n\t"
        "lw    $a0, 0x1AC($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a0, 0x10($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$f12");
}
/**
 * The Sims 2 PSP - func_00102C84 (0x00102C84, 0x24 bytes)
 *
 * Copies twelve bytes to a fixed address.
 *
 *     lw    $a1, 0x0($a0)
 *     lw    $a2, 0x4($a0)
 *     lui   $a3, 0xF
 *     lw    $a0, 0x8($a0)
 *     addiu $a3, $a3, -0x3898
 *     sw    $a1, 0x0($a3)
 *     sw    $a2, 0x4($a3)
 *     jr    $ra
 *     sw    $a0, 0x8($a3)
 *
 * **`*(u32 *)0x0EC768 = arg->word_0; *(u32 *)0x0EC76C = arg->word_4;
 * *(u32 *)0x0EC770 = arg->word_8;`**  - three words, in order, to one fixed address.
 *
 * **The address is 0x0EC768 and it is worth writing down carefully**, because it is
 * easy to get wrong by one hex digit:
 *
 *     lui   $a3, 0xF          ->  0x000F0000
 *     addiu $a3, $a3, -0x3898 ->  0x000F0000 - 0x3898 = 0x000EC768
 *
 * A first pass at this function computed 0x0C768 and went looking for a writer of that
 * address in `tools/code_writers.py`, which had nothing.  **The address was wrong, not
 * the tool** - the tool lists 0x0ec768 with this function as its single writer.  The
 * slip is worth recording because it is the third time in this project that a hand
 * computation of a `lui`/`addiu` pair disagreed with the tool, and the tool was right
 * all three times; `addiu` sign-extends its immediate, so a negative low half subtracts
 * from a sixteen-digit high half and the result is nearly always a third digit longer
 * than intuition expects.
 *
 * **Three loads then three stores with nothing in between**, and the loads are
 * interleaved with the address construction: `lw $a0, 0x8($a0)` sits between the `lui`
 * and the `addiu`.  `base_of` in `tools/stride_table.py` allows that deliberately -
 * the two halves of an address need not be adjacent - and it is what lets this
 * function's address be found at all.
 *
 * **The destination has no symbol in the project's map**, so nothing marks it as data.
 * It lands inside `.text`, 0x180 bytes into `func_000EC5E8` and before that function's
 * own last `jr $ra`, and six relocations in the module target it - every one from a
 * `lui`/`addiu` pair, one of which is this function's own.  **What that is is not
 * established**: the three addresses already verified this way (`tools/code_writers.py
 * --real`) are writable data inside live code, but the same test cannot tell a
 * deliberate runtime patch from a data block whose label the symbol map lacks, and
 * splat's function sizes are "distance to the next label".  At 0x0EC768 the bytes are a
 * `nop` in a branch delay slot, which looks more like a data block than a patch target,
 * but that is an impression and not a measurement.
 *
 * `.set noreorder` is needed at the return: the third store is in the delay slot, and
 * under `.set reorder` the assembler would hoist the second store into the slot and
 * leave the third word unwritten.
 */
#include "types.h"

/** Copy the three words at offsets 0, 4 and 8 of the argument to the fixed address
 *  0x0EC768.
 *  @param arg In $a0: three consecutive words. */
__attribute__((noreturn)) void func_00102C84(void *arg) {
    (void)arg;
    __asm__ __volatile__(
        "lw    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "lui   $a3, 0xF\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "addiu $a3, $a3, -0x3898\n\t"
        "sw    $a1, 0x0($a3)\n\t"
        "sw    $a2, 0x4($a3)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a0, 0x8($a3)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
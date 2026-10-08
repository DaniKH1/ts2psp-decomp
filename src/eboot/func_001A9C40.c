/**
 * The Sims 2 PSP - func_001A9C40 (0x001A9C40, 0x2C bytes)
 *
 * Copies three words into a member of the first argument, then sets a flag bit.
 *
 *     lw    $a2, 0x0($a1)
 *     lw    $t0, 0x4($a1)
 *     addiu $a3, $a0, 0x3C
 *     lw    $a1, 0x8($a1)
 *     sw    $a2, 0x0($a3)
 *     sw    $t0, 0x4($a3)
 *     sw    $a1, 0x8($a3)
 *     lw    $a1, 0x18($a0)
 *     ori   $a1, $a1, 0x80
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **Two jobs in one function: `a0->field_3C = *a1; a0->flags_18 |= 0x80;`**
 *
 * The copy is the shape of `func_0016BC70` - three words rather than two, so it
 * needs a third register (`$t0`) and a third pointer register (`$a3`) - and the
 * flag is one of the same accessors as `func_001A9D54` and `func_001A9D94`.  Two
 * functions in the module have this combined shape, differing only in the
 * destination offset and in which bit they set.
 *
 * **Why `ori` here and `lui` in the standalone setters.**  Bit 7 fits in a signed
 * 16-bit immediate, so the mask is one instruction; bits 18 and 20 do not, so
 * `func_001A9D94` and `func_001A9D54` each spend a `lui`.  The immediate here is
 * 0x80 unsigned but still positive, so no sign extension is involved - had it been
 * bit 15 the assembler would have needed a different spelling entirely.
 *
 * **The copy is interleaved with the pointer arithmetic on purpose.**  The first
 * two words are read out of `*a1` before `$a1` is itself overwritten by the third
 * load, and `$a3` - the destination - is formed between them, which is where the
 * scheduler had a free slot.  Read the three loads together and the two loads into
 * `$a2` and `$t0` are not independent of the third in the original's ordering.
 */
#include "types.h"

/** Copy the first three words of the second argument into offset 0x3C of the
 *  first, then set bit 7 of its flags word at offset 0x18. */
__attribute__((noreturn)) void func_001A9C40(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "lw    $a2, 0x0($a1)\n\t"
        "lw    $t0, 0x4($a1)\n\t"
        "addiu $a3, $a0, 0x3C\n\t"
        "lw    $a1, 0x8($a1)\n\t"
        "sw    $a2, 0x0($a3)\n\t"
        "sw    $t0, 0x4($a3)\n\t"
        "sw    $a1, 0x8($a3)\n\t"
        "lw    $a1, 0x18($a0)\n\t"
        "ori   $a1, $a1, 0x80\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2", "$a3", "$t0");
}
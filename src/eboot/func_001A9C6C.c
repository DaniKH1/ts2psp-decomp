/**
 * The Sims 2 PSP - func_001A9C6C (0x001A9C6C, 0x2C bytes)
 *
 * Copies three words into a member of the first argument, then sets a flag bit.
 *
 *     lw    $a2, 0x0($a1)
 *     lw    $t0, 0x4($a1)
 *     addiu $a3, $a0, 0x48
 *     lw    $a1, 0x8($a1)
 *     sw    $a2, 0x0($a3)
 *     sw    $t0, 0x4($a3)
 *     sw    $a1, 0x8($a3)
 *     lw    $a1, 0x18($a0)
 *     ori   $a1, $a1, 0x100
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`a0->field_48 = *a1; a0->flags_18 |= 0x100;`**  - the same two jobs as
 * `func_001A9C40` forty-four bytes earlier, into offset 0x48 instead of 0x3C and
 * setting bit 8 instead of bit 7.  The two are the only functions in the module of
 * this combined shape.
 *
 * Offset 0x48 is the same member `func_0016BC70`, `func_0016F698` and
 * `func_0016F6D8` copy two words into, and 0x3C is new here - so the pair looks
 * like two structs, or one struct with two similar members, each initialised from
 * a three-word source and then marked with its own flag.
 *
 * See `func_001A9C40` for the scheduling of the interleaved copy and for why the
 * mask here needs one instruction where the standalone setters need a `lui`.
 */
#include "types.h"

/** Copy the first three words of the second argument into offset 0x48 of the
 *  first, then set bit 8 of its flags word at offset 0x18. */
__attribute__((noreturn)) void func_001A9C6C(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "lw    $a2, 0x0($a1)\n\t"
        "lw    $t0, 0x4($a1)\n\t"
        "addiu $a3, $a0, 0x48\n\t"
        "lw    $a1, 0x8($a1)\n\t"
        "sw    $a2, 0x0($a3)\n\t"
        "sw    $t0, 0x4($a3)\n\t"
        "sw    $a1, 0x8($a3)\n\t"
        "lw    $a1, 0x18($a0)\n\t"
        "ori   $a1, $a1, 0x100\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2", "$a3", "$t0");
}
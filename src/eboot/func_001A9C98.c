/**
 * The Sims 2 PSP - func_001A9C98 (0x001A9C98, 0x24 bytes)
 *
 * Sets one flag bit, clears another, and writes the sentinel float.
 *
 *     lw    $a1, 0x18($a0)
 *     lui   $a2, 0xBF80
 *     mtc1  $a2, $f12
 *     ori   $a1, $a1, 0x1000
 *     addiu $a2, $zero, -0x2001
 *     swc1  $f12, 0x1C($a0)
 *     and   $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 = (self->flags_18 | 0x1000) & ~0x2000; self->float_1C = -1.0f;`**
 *
 * Three functions in the module do this kind of double-ended flag write, and this
 * is the clearest of them because the two bits are different: bit 12 goes on, bit 13
 * goes off, and the float at 0x1C is reset to the sentinel.  `func_001A9CD4` is the
 * mirror image, and `func_001A9CBC` clears both and takes the float from its caller.
 *
 * **The sentinel is 0xBF800000, which is `-1.0f`.**  `lui` puts the bit pattern in
 * `$a2`, `mtc1` moves it into `$f12`, and `swc1` stores it - three instructions for
 * a constant, because the PSP has no immediate form for a float.  The `mtc1` also
 * needs `.set noreorder`: `$f12` is not an architectural dependency of anything the
 * assembler can see between the `lui` and the `mtc1`, and it must not be reordered
 * across the store either.
 *
 * **`-0x2001`, not `-0x2000`.**  The extra one is the mask for the bit *below* bit
 * 13: `0xFFFFDFFF` clears bit 13 and leaves everything else, and it is built as
 * `-0x2001` because the field has to be positive to fit an `addiu`.  Reading it as
 * `~0x2000` and expecting `-0x2000` is off by one instruction's worth of meaning;
 * `-0x2001 == 0xFFFFDFFF == ~0x2000`, which is the point.
 *
 * `tools/flag_accessors.py` is the census for this cluster: six plain flag
 * accessors over four bits, and three of these combined ones.
 */
#include "types.h"

/* The bit patterns of the sentinel, which is -1.0f. */
#define SENTINEL_HI 0xBF80

/** Set bit 12 and clear bit 13 of the flags word at offset 0x18, and reset the
 *  float at offset 0x1C to -1.0f. */
__attribute__((noreturn)) void func_001A9C98(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "lui   $a2, %[hi]\n\t"
        ".set noreorder\n\t"
        "mtc1  $a2, $f12\n\t"
        ".set reorder\n\t"
        "ori   $a1, $a1, 0x1000\n\t"
        "addiu $a2, $zero, -0x2001\n\t"
        "swc1  $f12, 0x1C($a0)\n\t"
        "and   $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (SENTINEL_HI)
        : "memory", "$a1", "$a2", "$f12");
}
/**
 * The Sims 2 PSP - func_001A9CD4 (0x001A9CD4, 0x24 bytes)
 *
 * Sets one flag bit, clears another, and writes the sentinel float.
 *
 *     lw    $a1, 0x18($a0)
 *     lui   $a2, 0xBF80
 *     mtc1  $a2, $f12
 *     ori   $a1, $a1, 0x2000
 *     addiu $a2, $zero, -0x1001
 *     swc1  $f12, 0x1C($a0)
 *     and   $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 = (self->flags_18 | 0x2000) & ~0x1000; self->float_1C = -1.0f;`**
 *
 * **The exact mirror image of `func_001A9C98`**, fifty-eight bytes earlier: this one
 * sets bit 13 and clears bit 12, that one sets bit 12 and clears bit 13, and both
 * reset the float at 0x1C to -1.0f.  Two functions that are each the other's inverse
 * on one bit, in a class that also has `func_001A9CBC` clearing both at once.
 *
 * Together with `func_001A9CF8` - which asks whether bit 13 is set, and if not falls
 * back to comparing the float against -1.0f - these three and a query make a
 * coherent little cluster: two independent states, each either a flag or a sentinel
 * value, and a setter for each combination.
 *
 * See `func_001A9C98` for the full write-up, including why the mask is `-0x1001`
 * rather than `-0x1000` and why the `mtc1` needs `.set noreorder`.
 */
#include "types.h"

/* The bit pattern of the sentinel, which is -1.0f. */
#define SENTINEL_HI 0xBF80

/** Set bit 13 and clear bit 12 of the flags word at offset 0x18, and reset the
 *  float at offset 0x1C to -1.0f. */
__attribute__((noreturn)) void func_001A9CD4(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "lui   $a2, %[hi]\n\t"
        ".set noreorder\n\t"
        "mtc1  $a2, $f12\n\t"
        ".set reorder\n\t"
        "ori   $a1, $a1, 0x2000\n\t"
        "addiu $a2, $zero, -0x1001\n\t"
        "swc1  $f12, 0x1C($a0)\n\t"
        "and   $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (SENTINEL_HI)
        : "memory", "$a1", "$a2", "$f12");
}
/**
 * The Sims 2 PSP - func_001A9ACC (0x001A9ACC, 0x14 bytes)
 *
 * Tests one bit of a flags word held at offset 0x18 of the first argument.
 *
 *     lw    $a0, 0x18($a0)
 *     lui   $a1, 0x2
 *     and   $v0, $a0, $a1
 *     jr    $ra
 *     sltu  $v0, $zero, $v0
 *
 * **`return (self->flags_18 & 0x20000) != 0;`**  - bit 17 of the word at offset
 * 0x18.
 *
 * The last two instructions are how the original turns a mask into a predicate.
 * `and` leaves the whole masked word in `$v0`, and `sltu` then compares it against
 * zero *unsigned*, so the answer is 0 or 1 rather than the mask itself.  psp-gcc
 * will compile `(x & 0x20000) != 0` to something in this family but not to these
 * two instructions at these offsets, so the narrowing is written out.
 *
 * **The mask is built with `lui`, not an immediate,** because 0x20000 does not fit
 * a signed 16-bit field - it needs the shift by sixteen that `lui` performs, and
 * `ori` with a sign-extended immediate would give 0xFFFF0000 instead.  That is the
 * whole reason there are two instructions where one would do, and it is the same
 * reason the two neighbours at 0x1A9D54 and 0x1A9D94 spend two instructions on
 * 0x100000 and 0x40000.
 *
 * **Bit 17 has a getter but no setter in the module.**  `tools/flag_accessors.py`
 * is the census; it finds six accessors of this shape over four (offset, mask,
 * direction) rows.  Bit 20 of the same word is the one that is complete - getter,
 * setter and clearer all present - so the asymmetry at bit 17 is worth recording
 * as a fact about the link rather than reading as a second meaning.
 */
#include "types.h"

/** Test bit 17 (0x20000) of the flags word at offset 0x18 of the first argument.
 *  @return 1 if the bit is set, 0 otherwise. */
__attribute__((noreturn)) u32 func_001A9ACC(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a0, 0x18($a0)\n\t"
        "lui   $a1, 0x2\n\t"
        "and   $v0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
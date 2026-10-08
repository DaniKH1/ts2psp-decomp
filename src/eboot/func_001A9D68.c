/**
 * The Sims 2 PSP - func_001A9D68 (0x001A9D68, 0x18 bytes)
 *
 * Clears one bit in a flags word held at offset 0x18 of the first argument.
 *
 *     lw    $a1, 0x18($a0)
 *     lui   $a2, 0xFFF0
 *     addiu $a2, $a2, -0x1
 *     and   $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 &= 0xFFEFFFFF;`**  - clears bit 20.
 *
 * **The mask takes two instructions where the setter's took one.**  `lui` supplies
 * the top sixteen bits and `addiu` finishes the bottom, because the mask wanted
 * here is `~0x100000`: its low half is 0xFFFF, which no single `lui` can express,
 * and a `nor` against 0x100000 would be two instructions as well but would need the
 * mask built first.  Adding -1 to 0xFFF00000 gives 0xFFEFFFFF directly, so the
 * compiler chose the addition.  This is the only six-instruction member of the
 * accessor family and the only one that is neither a getter nor a setter.
 *
 * The rest is `func_001A9D54` with `or` for `and`: `$a0` keeps the object pointer,
 * `$a1` holds the modified word, and the store is in the return's delay slot.
 *
 * **This is what completes bit 20's family.**  `tools/flag_accessors.py` finds six
 * accessors of this shape in the module, over four (offset, mask, direction) rows;
 * this, `func_001A9D54` and `func_001A9D80` are the three directions for one bit.
 */
#include "types.h"

/** Clear bit 20 (0x100000) of the flags word at offset 0x18 of the first argument. */
__attribute__((noreturn)) void func_001A9D68(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "lui   $a2, 0xFFF0\n\t"
        "addiu $a2, $a2, -0x1\n\t"
        "and   $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2");
}
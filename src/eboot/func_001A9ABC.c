/**
 * The Sims 2 PSP - func_001A9ABC (0x001A9ABC, 0x10 bytes)
 *
 * One instruction of arithmetic, and it is a boolean conversion.
 *
 *     lw   $a0, 0x18($a0)
 *     andi $v0, $a0, 0x8000
 *     jr   $ra
 *     sltu $v0, $zero, $v0
 *
 * **`return (*(u16 *)((char *)self + 0x18) & 0x8000) != 0;`**
 *
 * **Bit 15 of the halfword at offset 0x18.**  The mask is `andi $v0, $a0, 0x8000`, so
 * the result before the branch back is either 0 or 0x8000, and `sltu $v0, $zero, $v0`
 * turns "nonzero" into 1.  C would have said `(x & 0x8000) ? 1 : 0` and psp-gcc
 * lowered the boolean to the branchless unsigned compare, which is two instructions
 * where a `slt` would be one - and **`sltu` with `$zero` as the left operand is the
 * spelling for `!= 0` on an unsigned value**, so the choice is deliberate about the
 * mask's own unsignedness rather than accidental.
 *
 * **The mask's width is in the operand, not the instruction.**  `andi` is
 * zero-extending, so `$v0` is 0 or 0x8000 regardless of the source.  If the field were
 * signed and the intent were a sign test, the instruction would be `sra` or `slt`
 * against `$zero`; the fact that it is `andi` says the original treated 0x8000 as a
 * named bit and not as a sign.
 *
 * **`.set noreorder` around the return is required and is not cosmetic.**  The
 * `sltu` is the whole result, and under `.set reorder` the assembler would fill
 * `jr $ra`'s slot with the `andi` - returning 0 or 0x8000 instead of 0 or 1, which is
 * a value no caller of a boolean accessor expects.
 *
 * This is one of the flag accessors `tools/flag_accessors.py` censuses; see that tool
 * for how many there are and what the bits are.
 */
#include "types.h"

/** Test bit 15 of the halfword at offset 0x18.
 *  @param self In $a0: the object.
 *  @return     0 or 1, in `$v0`. */
__attribute__((noreturn)) long func_001A9ABC(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a0, 0x18($a0)\n\t"
        "andi $v0, $a0, 0x8000\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltu $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "$a0");
}
/**
 * The Sims 2 PSP - func_001A9B78 (0x001A9B78, 0x14 bytes)
 *
 * Sets bit 2 of the word at offset 0x18 and stores a float at offset 0x34.
 *
 *     lw   $a1, 0x18($a0)
 *     swc1 $f12, 0x34($a0)
 *     ori  $a1, $a1, 0x4
 *     jr   $ra
 *     sw   $a1, 0x18($a0)
 *
 * **`self->flags_18 |= 0x4; self->float_34 = f;`**
 *
 * **This is the flag-accessor family, and it is a shape the census does not have.**
 *
 * `tools/flag_accessors.py` has `get`, `set` and `clear`, and all three build the mask
 * with `lui` because every mask it had found was above bit 15.  Bit 2 fits a 16-bit
 * immediate, so the mask is `ori $a1, $a1, 0x4` - one instruction instead of three -
 * and the accessor is four instructions instead of five.  **The census's `set` shape
 * requires `lui` followed by `or`, and this is `ori`; the two are the same operation
 * and the difference is only that `ori` cannot hold a mask wider than a halfword.**
 *
 * **The reason `ori` can be a setter at all is that its destination and its source are
 * the same register.**  `ori rt, rs, imm` computes `rt = rs | imm`, so with `rt == rs`
 * - here `$a1` on both sides - it is a read-modify-write on `$a1` in one instruction.
 * The `lui` spelling needs a *separate* register for the mask (`lui $t0, HI` then
 * `or $a1, $a1, $t0`) because `or rd, rs, rt` has two distinct inputs and neither can
 * be both the destination and the value being preserved.
 *
 * **So the accessor length tracks the mask width: five instructions above bit 15, four
 * at or below it, and the saved instruction is the scratch register the wide form
 * needed.**  That is the same rule `flag_accessors.py` already states for `get` - bit 15
 * uses `andi`, bit 17 uses `lui` - now shown to apply to `set` as well, from a function
 * the tool does not match.  `tools/boolean_shapes.py` counts the immediate form at
 * **18 functions** (31 occurrences, so several set more than one bit), against the
 * eight rows `flag_accessors.py` finds for the wide form.
 *
 * **The float at 0x34 is written from `$f12` between the load and the store**, so the
 * order is: read the flags, store the float, update the flags, store the flags.  Nothing
 * depends on that order and the compiler had a free choice; `flag_accessors.py` has a
 * separate path for accessors that also write a float, but that one looks for -1.0f at
 * offset 0x1C and this float comes in a register at 0x34, so this is a third
 * combination: **a flag setter that also writes a float, at an offset the tool does not
 * know and from a register rather than a constant.**
 *
 * It sits in the flag cluster at 0x001A9ABC-0x001A9D94, whose other members
 * `flag_accessors.py` does list: `func_001A9ABC` gets bit 15 and `func_001A9ACC` gets
 * bit 17 of the same word, and `func_001A9D54`/`func_001A9D68`/`func_001A9D80` work on
 * bits 18 and 20.  **Bit 2 is the lowest bit anyone in the cluster touches.**
 */
#include "types.h"

/** Set bit 2 of the word at offset 0x18, and store `$f12` at offset 0x34.
 *  @param self In $a0: the object. */
__attribute__((noreturn)) void func_001A9B78(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a1, 0x18($a0)\n\t"
        "swc1 $f12, 0x34($a0)\n\t"
        "ori  $a1, $a1, 0x4\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$f12");
}
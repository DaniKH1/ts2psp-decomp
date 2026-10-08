/**
 * The Sims 2 PSP - sortAndCullScene_10BC (0x001B4CD8, 0x18 bytes)
 *
 * Returns one of two fields depending on a third.
 *
 *     lw    $a1, 0x14C($a0)
 *     addiu $v0, $zero, -0x2
 *     bnel  $a1, $zero, . + 4 + (0x1 << 2)
 *     lw    $v0, 0xEC($a0)
 *     jr    $ra
 *     nop
 *
 * **`return self->field_14C ? -2 : self->field_0EC;`**
 *
 * **The conditional load is the delay slot of the branch, and that is the whole
 * instruction.**  `bnel` is branch-if-not-equal-**likely**: the branch is taken when
 * `$a1` is not zero, and the instruction in its delay slot executes *only* if the
 * branch was taken.  So the `lw` in the slot runs exactly when `$a1 == 0`, which is
 * the opposite of what a plain `bne` followed by the load would do.
 *
 * **The target is `$pc + 8`, not `$pc + 4`.**  The word is `54a00001`, offset 1, and
 * the displacement is relative to the address of the delay slot, so `1 << 2` lands on
 * `jr $ra` at 0x001B4CE8 - the `lw` is skipped.  Both facts have to come from the
 * word; a listing that prints a resolved branch target makes it easy to write the
 * condition the wrong way round, and the project has been caught by that once, so
 * `tools/disasm_range.py` is what settles it.
 *
 * **`-2` is a sentinel, not an error code that anything here can explain.**  It is
 * built with `addiu` from `$zero`, so it is the ordinary way a negative constant
 * arrives, and it is the *default*: written into `$v0` before the branch, so the
 * not-taken path already has its answer.
 *
 * **There is no twin in this translation unit, and there are forty-two others in
 * the module.**  The claim this file would like to make is that some other function
 * reads another field with a different default the same way - and the next symbol,
 * `sortAndCullScene_10D4` at 0x001B4CF0, is eighty bytes with a 0x20 frame that calls
 * out twice, so there is no twin *here*.  `tools/branch_load.py` counts the shape
 * properly:
 *
 *   38,336   `beq`/`bne`/`beql`/`bnel` in the module
 *    3,214   the likely forms, 8.4 %
 *    4,041   loads in some branch's delay slot
 *      616   functions with a load in a *likely* branch's delay slot
 *       43   of those set the destination to a constant first
 *
 * **43, not 1** - so the `value or sentinel` accessor is an idiom here, and the
 * conditional load in a likely branch's delay slot is the mechanism, at 616 functions
 * and 8 % of the module's branches.  `func_00052950`, 0x00052950, is one of the 43
 * and is the same three instructions with a different default: `ori $a2, $zero, 0x0`
 * then `bnel` with `lw $a2, 0x48($a0)`.  **Two examples is not a family and 43 is
 * not a proof of what the default means** - the defaults range over at least `-2` and
 * `0`, so what a sentinel is for is still not established, only that the shape is
 * deliberate enough for the compiler to have an idiom for it.
 *
 * **The likely form is the whole trick and it is used only 8 % of the time.**  A plain
 * `bne` would need a label, so a conditional load through one costs two branches; a
 * likely branch makes its delay slot conditional, so the same thing costs one branch
 * and no label.  That the module uses it for 3,214 branches and not more suggests the
 * gain is only available in the narrow case where the branch's *only* purpose is to
 * guard the delay slot - which is exactly this function and `func_00052950`, and
 * exactly what makes the -2 a default rather than a value.
 *
 * The trailing `nop` is in `jr $ra`'s delay slot and must be written out: under
 * `.set noreorder` the assembler inserts nothing, so without it the slot would be
 * whatever followed in the C output.
 */
#include "types.h"

/** Return the field at +0xEC if the field at +0x14C is zero, else -2.
 *  @param self In $a0: the object.
 *  @return     The value, in `$v0`. */
__attribute__((noreturn)) long sortAndCullScene_10BC(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw    $a1, 0x14C($a0)\n\t"
        "addiu $v0, $zero, -0x2\n\t"
        /* `.set noreorder` from here, and the target written as a local label rather
         * than a displacement: with a computed displacement gas does not recognise
         * the branch as "likely" and expands it to a plain `bne` plus a `nop`, which
         * is four bytes too many. */
        ".set noreorder\n\t"
        "bnel  $a1, $zero, 1f\n\t"
        "lw    $v0, 0xEC($a0)\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1");
}
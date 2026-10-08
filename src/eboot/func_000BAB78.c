/**
 * The Sims 2 PSP - func_000BAB78 (0x000BAB78, 0x24 bytes)
 *
 * Returns a pointer into an array, or NULL, gated on a field equalling one.
 *
 *     lw    $a1, 0x30($a0)
 *     ori   $v0, $zero, 0x0
 *     addiu $a1, $a1, 0x1C
 *     lw    $a0, 0x0($a1)
 *     ori   $a2, $zero, 0x1
 *     bnel  $a0, $a2, . + 4 + (0x1 << 2)
 *     addu  $v0, $a1, $a0
 *     jr    $ra
 *     nop
 *
 * **`return *(u32 *)((char *)self->ptr_30 + 0x1C) == 1 ? self->ptr_30 + 0x1D : NULL;`**
 *
 * **The delay slot adds the value it just tested against one.**  `addu $v0, $a1, $a0`
 * executes only when the branch is taken, and `$a0` is 1 in exactly that case, so the
 * pointer returned is `$a1 + 1` - one byte past the field that was tested.  **So this
 * is the general shape `result = base + loaded_index`, guarded on the index, with NULL
 * as the default - and here the index is 1 because the test is equality against 1.**
 *
 * **The general shape is `func_0000EBDC`'s, three times over.**  At 0x0000EBDC, 696
 * bytes, there are three separate instances:
 *
 *     ori   $fp, $zero, 0x0
 *     ...   lw $a2, 0x0($a1)
 *     bnel  $a2, $t1, . + 4 + (0x1 << 2)
 *     addu  $fp, $a1, $a2
 *
 * and the same two lines again into `$s7`, and into `$s5`.  So `$fp = $s7 = $fp =
 * NULL` before the work and a table lookup after it, and **all three are one idiom, not
 * three coincidences.**  `func_0000E04C` is the same idea with the index used as a
 * pointer instead of an offset: `bnel $a0, $zero` guarding `addiu $s1, $a0, 0x8`, so
 * `s1 = node ? node + 8 : NULL` - **"the field at +8, if the link exists"**, which is
 * the clearest statement of what the NULL default is for that this project has found.
 *
 * **An earlier draft of this file called the addend "the constant the branch has just
 * proved" and treated that as the whole story.**  It is true here and it is the
 * degenerate case: because the test is `== 1`, the index and the tested constant are
 * the same register, so `$a0` fills both roles.  In `func_0000EBDC` the two roles are
 * distinct - `$t1` is what `$a2` is compared against and `$a2` is what is added - and
 * the idiom is plainly about indexing rather than about proving anything.  **So the
 * honest statement is that `$a0` is the index, and the reason it is the constant is
 * that the test pins it to one.**
 *
 * **The count of one is loaded into a register to compare against**, which is two
 * instructions (`ori $a2, $zero, 0x1` plus the `bnel`) where `bnel $a0, $zero` would
 * test the *opposite* condition.  There is no `beql`-with-compare-to-one form that
 * avoids the register, so the constant is materialised.  Zero, by contrast, needs no
 * register - which is why `func_000BF49C`, `sortAndCullScene_10BC` and all three
 * instances in `func_0000EBDC` spend nothing on the comparison.
 *
 * **`ori` is used for the constant 1 and for the NULL default**, both from `$zero`.
 * 1 fits `ori`'s zero-extended immediate exactly, and `addiu $a2, $zero, 1` would
 * assemble to the same word; `sortAndCullScene_10BC`'s default of -2 uses `addiu`
 * because -2 does not fit.  So the choice tracks the immediate, not a preference.
 *
 * **The field is at 0x1C and the result is at 0x1D, one byte later.**  That is what
 * makes the `addu` an increment rather than a scaling: the tested field is a word at
 * 0x1C and the pointer returned is a byte at 0x1D inside it.  Nothing here says the
 * container is byte-addressed - the word read is a `lw` and the pointer arithmetic is
 * in bytes because that is what MIPS does - so the two accesses are of different
 * widths to overlapping storage.
 */
#include "types.h"

/** Return one byte past the word at +0x1C if that word is 1, else NULL.
 *  @param self In $a0: the container; its +0x30 pointer is the array.
 *  @return     The address, or NULL, in `$v0`. */
__attribute__((noreturn)) void *func_000BAB78(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw    $a1, 0x30($a0)\n\t"
        "ori   $v0, $zero, 0x0\n\t"
        "addiu $a1, $a1, 0x1C\n\t"
        "lw    $a0, 0x0($a1)\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "bnel  $a0, $a2, 1f\n\t"
        "addu  $v0, $a1, $a0\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "memory", "$a0", "$a1", "$a2");
}
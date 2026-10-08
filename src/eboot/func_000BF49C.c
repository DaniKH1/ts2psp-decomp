/**
 * The Sims 2 PSP - func_000BF49C (0x000BF49C, 0x30 bytes)
 *
 * Returns a pointer to an indexed element, or NULL if that element is not empty.
 *
 *     lw    $a0, 0x14($a0)
 *     ori   $v0, $zero, 0x0
 *     lw    $a1, 0x24($a0)
 *     addiu $a0, $a0, 0x24
 *     addu  $a0, $a0, $a1
 *     lb    $a1, 0x0($a0)
 *     sltiu $a1, $a1, 0x1
 *     andi  $a1, $a1, 0xFF
 *     beql  $a1, $zero, . + 4 + (0x1 << 2)
 *     move  $v0, $a0
 *     jr    $ra
 *     nop
 *
 * **`return ((s8 *)((char *)self->vec + 0x24))[index] == 0 ? &that_byte : NULL;`**
 *
 * **This is the `NULL if absent` accessor with an ALU op in the delay slot, and that
 * is the part `tools/branch_load.py` could not see.**  Its default count counts a
 * *load* in a likely branch's delay slot; here the delay slot holds a `move`.  Same
 * idiom, same `beql`, same default of zero written before the branch - and the
 * load-only count of 616 does not include this function.  **The tool's `--all` mode
 * counts any register write in a likely delay slot and gets 1,173 functions, of which
 * 311 have a constant default first - so 43 was an undercount by more than seven
 * times, and the undercount was the tool's, not the module's.**
 *
 * **The comparison is three instructions where one would do, and the third is dead.**
 *
 *     sltiu $a1, $a1, 0x1     unsigned `$a1 < 1`, i.e. `$a1 == 0`
 *     andi  $a1, $a1, 0xFF    dead: the value is already 0 or 1
 *     beql  $a1, $zero, ...   test it against zero anyway
 *
 * `sltiu` against an immediate of 1 produces exactly 0 or 1, so `andi 0xFF` cannot
 * change the result.  **It is a fourth instance of a mask that cannot alter its
 * operand** - with `func_0014EAAC`'s `and` against 0xFFFFFFFF, `func_00080758`'s
 * `and` against 0xFFFFFF0F, and this one - and the first of the four where the
 * redundant mask is a *zero*-extending `andi` rather than a full-word `and`.
 *
 * **The redundant mask and the branch against zero together are what C's
 * `if (x)` would have said.**  The source is a truth test on a signed byte, and psp-gcc
 * lowered it as "is it zero" via an unsigned compare, kept the mask from a wider
 * expression it had already narrowed, and then tested the result against zero rather
 * than branching on the compare directly.  Three instructions to say `if (!byte)`.
 *
 * **The address is built twice and the index is reached through the object's own
 * pointer**, so the function dereferences `$a0` once at +0x14 before it can even find
 * the array.  `lw $a1, 0x24($a0)` reads the index out of the *container*, while
 * `addiu $a0, $a0, 0x24` points at the array itself - **two different things at the
 * same offset**, one a load and one an address, which is the only reason the `lw` and
 * the `addiu` are both there and neither is redundant.
 *
 * `ori $v0, $zero, 0x0` is the NULL default, spelled `ori` rather than `addiu` because
 * zero fits either and `ori` is the mnemonic the rest of the module uses for a zero
 * default.  `sortAndCullScene_10BC` defaults with `addiu $v0, $zero, -0x2` instead,
 * because -2 does not fit `ori`'s zero-extended immediate.
 *
 * **The branch target must be a local label.**  Written as a computed displacement,
 * gas does not assemble `beql` as a likely branch - it emits a plain `beq` and an
 * inserted `nop`, so the delay slot ends up holding the `nop` rather than the
 * `move`, and the function both grows by four bytes and returns the un-overwritten
 * zero.
 */
#include "types.h"

/** Return a pointer to the indexed byte if it is zero, else NULL.
 *  @param self In $a0: the container; its +0x14 pointer holds the array and its
 *              +0x24 word holds the index.
 *  @return     The address of the byte, or NULL, in `$v0`. */
__attribute__((noreturn)) void *func_000BF49C(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw    $a0, 0x14($a0)\n\t"
        "ori   $v0, $zero, 0x0\n\t"
        "lw    $a1, 0x24($a0)\n\t"
        "addiu $a0, $a0, 0x24\n\t"
        "addu  $a0, $a0, $a1\n\t"
        "lb    $a1, 0x0($a0)\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "andi  $a1, $a1, 0xFF\n\t"
        ".set noreorder\n\t"
        "beql  $a1, $zero, 1f\n\t"
        "move  $v0, $a0\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "memory", "$a0", "$a1");
}
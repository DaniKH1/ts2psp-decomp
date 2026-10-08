/**
 * The Sims 2 PSP - func_001022F0 (0x001022F0, 0x4C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_00064424)
 *     lbu   $a0, %lo(sym_00064424)($a0)
 *     sw    $ra, 0x10($sp)
 *     beqz  $a0, 1f
 *     nop
 *     lui   $a0, %hi(sym_001DB158)
 *     lw    $a0, %lo(sym_001DB158)($a0)
 *     lui   $a1, %hi(sym_001DB15C)
 *     lw    $a1, %lo(sym_001DB15C)($a1)
 *     ori   $a2, $zero, 0x16
 *     sra   $a0, $a0, 1
 *     jal   func_0010215C
 *     sra   $a1, $a1, 1
 *     lui   $a0, %hi(sym_001DB12C)
 *     sw    $v0, %lo(sym_001DB12C)($a0)
 *   1:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **If the byte at `sym_00064424` is non-zero, allocate a render object of
 * element count 0x16 and both dimensions halved, and store it in
 * `sym_001DB12C`.**
 *
 *     if (g_enabled) g_render = make(0x16, g_w >> 1, g_h >> 1);
 *
 * ## The two `sra` instructions are in the delay slot, and that is the trap
 *
 * The halving of both arguments happens in the `jal`'s delay slot:
 *
 *     jal func_0010215C
 *       sra $a1, $a1, 1
 *
 * **The `$a0` shift is a separate instruction three lines earlier**, so the
 * three arguments are in a different order than a reader expects: the shift of
 * `$a0` is emitted *before* the call is placed, and the shift of `$a1` lands in
 * the slot.  Writing this as ordinary C would let the compiler choose, and it
 * would not choose this.  Under `.set noreorder` both stay where they are.
 *
 * As in `func_00102280`, `sra` is arithmetic: a signed halving that rounds odd
 * values toward negative infinity, then nothing narrows it further, so the
 * full 32-bit result is passed on.
 *
 * ## `0x16` is the third argument and it is a plain immediate
 *
 * `ori $a2, $zero, 0x16` sets the third parameter to 22 with no flag bit
 * packed into it.  **A count of 22 is a plausible vertex or element total for a
 * small fixed shape, and 22 is also small enough to be a struct size** - the
 * three readings cannot be separated from the call alone, because
 * `func_0010215C` has not been transcribed yet.  This file does not pick one.
 *
 * ## It is the setter that `func_00102280` reads
 *
 * Both functions touch `sym_001DB12C`.  That one writes a half-word at offset
 * `0xE` of the object and returns it; this one creates it and returns nothing.
 * **So the pair is create-then-configure, in that order, with the configure
 * step re-reading the global rather than being handed the pointer** - which is
 * the same defensive pattern noted in `func_00102280.c`.
 */
#include "types.h"

/** If `sym_00064424` is non-zero, create the render object and store it at
 *  `sym_001DB12C`.  Returns nothing. */
__attribute__((noreturn)) void func_001022F0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_00064424)\n\t"
        "lbu   $a0, %%lo(sym_00064424)($a0)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "beqz  $a0, 1f\n\t"
        "nop\n\t"
        "lui   $a0, %%hi(sym_001DB158)\n\t"
        "lw    $a0, %%lo(sym_001DB158)($a0)\n\t"
        "lui   $a1, %%hi(sym_001DB15C)\n\t"
        "lw    $a1, %%lo(sym_001DB15C)($a1)\n\t"
        "ori   $a2, $zero, 0x16\n\t"
        "sra   $a0, $a0, 1\n\t"
        "jal   func_0010215C\n\t"
        "sra   $a1, $a1, 1\n\t"
        "lui   $a0, %%hi(sym_001DB12C)\n\t"
        "sw    $v0, %%lo(sym_001DB12C)($a0)\n\t"
        "1:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
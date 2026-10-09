/**
 * The Sims 2 PSP - func_0019D564 (0x0019D564, 0x5C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_0019D5AC
 *       or    $s0, $a1, $zero
 *     lui   $a0, %hi(sym_001EA588)
 *     addiu $a0, $a0, %lo(sym_001EA588)
 *     sw    $a0, 0x18($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_000B9C88
 *       or    $a1, $zero, $zero
 *     andi  $a0, $s0, 0x1
 *     beqz  $a0, .Leboot_0019D5AC
 *     nop
 *     jal   func_0012771C
 *       or    $a0, $s1, $zero
 *   .Leboot_0019D5AC:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * The third of the three, with `sym_001EA588` as its vtable.
 *
 * **Read the three vtables and the shape of the hierarchy is not what
 * the constructors suggest.**  `sym_001EA3E8`, `sym_001EA4B8` and
 * `sym_001EA588` are exactly 0xD0 apart, each 26 entries of 8 bytes
 * (a null word and a pointer), with slots at +0x08 through +0xC8.
 * Comparing all three entry by entry, **they differ in exactly two
 * slots:**
 *
 *     +0x08   the constructor itself: 0019d4ac / 0019d508 / 0019d564
 *     +0x30   000bbd84 / 000bbdc8 / 000bbe78
 *
 * Every other 24 slots are the same function in all three.
 *
 * So these are **three siblings that override one virtual method each
 * and nothing else** - not three classes with independent interfaces.
 * The differing method is at slot +0x30, the seventh virtual entry.
 * The 24 shared slots are inherited unchanged from the base, which is
 * why the constructors are identical too.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019D564(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_0019D5AC\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, %%hi(sym_001EA588)\n\t"
        "addiu $a0, $a0, %%lo(sym_001EA588)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "beqz  $a0, .Leboot_0019D5AC\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_0019D5AC:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
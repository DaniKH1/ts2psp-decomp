/**
 * The Sims 2 PSP - func_0019D4AC (0x0019D4AC, 0x5C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_0019D4F4
 *       or    $s0, $a1, $zero
 *     lui   $a0, %hi(sym_001EA3E8)
 *     addiu $a0, $a0, %lo(sym_001EA3E8)
 *     sw    $a0, 0x18($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_000B9C88
 *       or    $a1, $zero, $zero
 *     andi  $a0, $s0, 0x1
 *     beqz  $a0, .Leboot_0019D4F4
 *     nop
 *     jal   func_0012771C
 *       or    $a0, $s1, $zero
 *   .Leboot_0019D4F4:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * A constructor: stamps the vtable pointer `sym_001EA3E8` into
 * 0x18($a0), calls the base constructor `func_000B9C88` with a null
 * second argument, and if bit 0 of the flags word is set, calls
 * `func_0012771C`.
 *
 * **The vtable goes in at 0x18, after the base constructor's
 * territory, not at 0x0.**  That offset is the first hard number this
 * cluster of constructors gives for the object layout: the derived
 * class has at least six words of its own before the vtable pointer,
 * and the `sw` happens *before* `func_000B9C88` runs, so the base
 * constructor cannot be clobbering it.
 *
 * **Nothing is returned.**  `$v0` is never written, and the function
 * ends on the `jr` with no `or $v0` anywhere - so a caller that reads
 * `$v0` here is reading whatever the last callee left there.  The
 * constructor returns its argument by convention through `$a0`, not
 * `$v0`.
 *
 * `func_0019D508` and `func_0019D564` are this function with
 * `sym_001EA4B8` and `sym_001EA588` substituted: **three constructors,
 * one per derived class, differing in nothing but the vtable address.**
 *
 * Reading the three vtables shows the same thing about the classes
 * themselves: they differ in exactly one virtual slot (at +0x30) and
 * share the other 24.  See `func_0019D564.c` for that comparison.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019D4AC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_0019D4F4\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, %%hi(sym_001EA3E8)\n\t"
        "addiu $a0, $a0, %%lo(sym_001EA3E8)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "beqz  $a0, .Leboot_0019D4F4\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_0019D4F4:\n\t"
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
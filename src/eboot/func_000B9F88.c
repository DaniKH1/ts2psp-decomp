/**
 * The Sims 2 PSP - func_000B9F88 (0x000B9F88, 0xE0 bytes)
 *
 * The shared virtual method at slot +0x38 of `sym_001EA3E8`.  It looks
 * a name up in the interned-symbol table and returns its index, or -1
 * if it is absent.
 *
 * **-1 is the failure value and it is written unconditionally on every
 * non-match exit** - three separate `addiu $v0, $zero, -1` sites all
 * converge on `.Leboot_000BA044`.  Three exits for one answer means the
 * function gives up early in three different ways: the table pointer
 * was null, the search loop found nothing, and the early bail at
 * `.Leboot_000BA048` after the last iteration.
 *
 * **The same sentinel-1 interning test as func_000B9D3C, written out
 * longhand instead of with `bnel`:**
 *
 *     ori   $a0, $zero, 0x1
 *     lw    $a3, 0x0($a2)
 *     bnel  $a3, $a0, .Leboot_000B9FC8
 *       addu  $a1, $a2, $a3       ; nullified when a3 == 1
 *
 * Identical meaning to the pair in func_000B9E2C, and **the constant 1
 * loaded into `$s3` at the top of func_000BA16C is the same sentinel**
 * kept alive in a callee-saved register across a call.
 *
 * On a hit the index is reconstructed as
 *
 *     result = lhu(0x14(0x14($s0))) - remaining + i
 *
 * which turns "how many entries are left to scan" back into an absolute
 * index.  **`$a0` is read with `lhu` from `0x14($s0)` and again from
 * `0x8($s2)`** - the same u16 count reached by two different paths,
 * one through the object and one through the intern node.
 *
 * The inner loop calls `func_00143984` per candidate and **bails
 * immediately on a zero return**, jumping back to the table header
 * rather than to the failure path - so a failed comparison restarts the
 * whole lookup, and the count `$a0` is re-read from `lhu 0x8($s2)` in
 * the delay slot.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B9F88(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "ori   $a0, $zero, 0x1\n\t"
        "or    $s1, $a1, $zero\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        "sw    $s3, 0x1C($sp)\n\t"
        "sw    $s4, 0x20($sp)\n\t"
        "sw    $ra, 0x24($sp)\n\t"
        "lw    $a2, 0x14($s0)\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "addiu $a2, $a2, 0xC\n\t"
        "lw    $a3, 0x0($a2)\n\t"
        "bnel  $a3, $a0, .Leboot_000B9FC8\n\t"
        "addu  $a1, $a2, $a3\n\t"
        ".Leboot_000B9FC8:\n\t"
        "or    $s2, $a1, $zero\n\t"
        "beqz  $s2, .Leboot_000BA044\n\t"
        "nop\n\t"
        "or    $a3, $a0, $zero\n\t"
        "lw    $a2, 0x0($s2)\n\t"
        "ori   $s4, $zero, 0x0\n\t"
        "lhu   $a0, 0x8($s2)\n\t"
        "bnel  $a2, $a3, .Leboot_000B9FEC\n\t"
        "addu  $s4, $a1, $a2\n\t"
        ".Leboot_000B9FEC:\n\t"
        "ori   $s3, $zero, 0x0\n\t"
        "slt   $a0, $s3, $a0\n\t"
        "beqz  $a0, .Leboot_000BA044\n\t"
        "ori   $a0, $zero, 0x0\n\t"
        "addu  $s4, $s4, $a0\n\t"
        ".Leboot_000BA000:\n\t"
        "lw    $a1, 0x0($s4)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_00143984\n\t"
        "addu  $a1, $s4, $a1\n\t"
        "beqz  $v0, .Leboot_000BA030\n\t"
        "lhu   $a0, 0x8($s2)\n\t"
        "addiu $s3, $s3, 0x1\n\t"
        "slt   $a0, $s3, $a0\n\t"
        "bnez  $a0, .Leboot_000BA000\n\t"
        "addiu $s4, $s4, 0x4\n\t"
        "b     .Leboot_000BA044\n\t"
        "nop\n\t"
        ".Leboot_000BA030:\n\t"
        "lw    $a1, 0x14($s0)\n\t"
        "lhu   $a1, 0x14($a1)\n\t"
        "subu  $v0, $a1, $a0\n\t"
        "b     .Leboot_000BA048\n\t"
        "addu  $v0, $v0, $s3\n\t"
        ".Leboot_000BA044:\n\t"
        "addiu $v0, $zero, -0x1\n\t"
        ".Leboot_000BA048:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $s2, 0x18($sp)\n\t"
        "lw    $s3, 0x1C($sp)\n\t"
        "lw    $s4, 0x20($sp)\n\t"
        "lw    $ra, 0x24($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
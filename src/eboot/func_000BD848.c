/**
 * The Sims 2 PSP - func_000BD848 (0x000BD848, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a1, $zero
 *     sw    $s1, 0x14($sp)
 *     sw    $ra, 0x18($sp)
 *     jal   func_000A7AC8
 *       or    $s1, $a0, $zero
 *     bnez  $v0, .Leboot_000BD874
 *       nop
 *     b     .Leboot_000BD884
 *       or    $v0, $zero, $zero
 *   .Leboot_000BD874:
 *     jal   func_0019E5F8
 *       lw    $a0, 0x18($s0)
 *     sw    $v0, 0x24($s1)
 *     sltu  $v0, $zero, $v0
 *   .Leboot_000BD884:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x014 of `sym_001EAB50`.  **19 of its 20 words are identical to
 * `func_000BD6D4`**, which fills the same slot in the first vtable;
 * the single word that differs is the `jal` at word 11, here
 * `func_0019E5F8` against `func_0019E4F8` there.
 *
 * **+0x014 is therefore a one-word template across the family**, the
 * same shape as +0x0C but narrower: nine of the ten siblings differ
 * from the first in exactly one instruction, where the constructors
 * needed two.
 *
 * What the template does is unchanged from `func_000BD6D4`: ask
 * `func_000A7AC8` whether to proceed, return 0 early if it says no,
 * otherwise call the per-class function on `0x18($s0)`, store the count
 * into `0x24($s1)`, and return it cast to a boolean with `sltu $v0,
 * $zero, $v0`.  **The caller gets "did anything happen", not the count.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD848(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s1, $a0, $zero\n\t"
        "bnez  $v0, .Leboot_000BD874\n\t"
        "nop\n\t"
        "b     .Leboot_000BD884\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BD874:\n\t"
        "jal   func_0019E5F8\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "sw    $v0, 0x24($s1)\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".Leboot_000BD884:\n\t"
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
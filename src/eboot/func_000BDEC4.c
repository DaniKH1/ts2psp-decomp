/**
 * The Sims 2 PSP - func_000BDEC4 (0x000BDEC4, 0x54 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a1, $zero
 *     sw    $s1, 0x14($sp)
 *     sw    $ra, 0x18($sp)
 *     jal   func_000A7AC8
 *       or    $s1, $a0, $zero
 *     bnez  $v0, .Leboot_000BDEF0
 *       nop
 *     b     .Leboot_000BDF04
 *       or    $v0, $zero, $zero
 *   .Leboot_000BDEF0:
 *     lw    $a0, 0x18($s0)
 *     jal   func_0019EA2C
 *       or    $a1, $s0, $zero
 *     sw    $v0, 0x24($s1)
 *     sltu  $v0, $zero, $v0
 *   .Leboot_000BDF04:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x014 of `sym_001EAEB0` - the sixth class, and **the one member
 * of this slot that is not the 20-word template.**
 *
 * The extra word buys a second argument.  In the other nine, the
 * `jal`'s delay slot holds `lw $a0, 0x18($s0)` and the per-class function
 * is called with one argument.  Here that load has been hoisted above the
 * `jal` so the delay slot is free, and it carries
 * `or $a1, $s0, $zero` instead - **so `func_0019EA2C` receives the
 * caller's second argument as well.**
 *
 * **That is a real difference in calling convention, not padding.**  The
 * callee is also not in the sequence the other nine come from: they call
 * `func_0019E4F8`, `5F8`, `6EC`, `7E8`, `8F0`, `EB80`, `ED04`, `EE0C`,
 * `EFE0`, and this one calls `func_0019EA2C`, which is not adjacent to
 * any of them in that run.  So this class is outside the sequence the
 * other ten form, and it is the only one whose callee can see the object
 * rather than only a field of it.
 *
 * Everything else is unchanged: ask `func_000A7AC8` whether to proceed,
 * return 0 early if not, otherwise store the count into `0x24($s1)` and
 * return it cast to a boolean.
 *
 * The eleventh member, `func_000BFC2C`, is 652 bytes with twenty labels
 * and is not this template either.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDEC4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s1, $a0, $zero\n\t"
        "bnez  $v0, .Leboot_000BDEF0\n\t"
        "nop\n\t"
        "b     .Leboot_000BDF04\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BDEF0:\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "jal   func_0019EA2C\n\t"
        "or    $a1, $s0, $zero\n\t"
        "sw    $v0, 0x24($s1)\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".Leboot_000BDF04:\n\t"
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
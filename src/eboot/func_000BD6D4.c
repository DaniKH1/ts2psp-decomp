/**
 * The Sims 2 PSP - func_000BD6D4 (0x000BD6D4, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a1, $zero
 *     sw    $s1, 0x14($sp)
 *     sw    $ra, 0x18($sp)
 *     jal   func_000A7AC8
 *       or    $s1, $a0, $zero
 *     bnez  $v0, .Leboot_000BD700
 *       nop
 *     b     .Leboot_000BD710
 *       or    $v0, $zero, $zero
 *   .Leboot_000BD700:
 *     jal   func_0019E4F8
 *       lw    $a0, 0x18($s0)
 *     sw    $v0, 0x24($s1)
 *     sltu  $v0, $zero, $v0
 *   .Leboot_000BD710:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * The third member of the five-function group that starts at
 * func_000BD5F4.
 *
 * **`bnez $v0` tests the *callee's* return value, not an argument.**
 * `$a0` was already moved to `$s1` in the previous delay slot, so the
 * `jal` leaves `$v0` as the only thing the branch can look at - and it
 * branches when func_000A7AC8 said "no".  So this is the *failure* path
 * that runs first, and 0 is the early return.
 *
 * The `or $v0, $zero, $zero` sits in the delay slot of the `b` that
 * jumps to the epilogue, and `b` is not a nullifying branch, so **that
 * store-free zeroing does execute** - which is what makes the early
 * return return 0 rather than whatever the callee left behind.
 *
 * On the taken path, `func_0019E4F8` is called with `0x18($s0)` and its
 * result goes to `0x24($s1)` - **the field that func_000BD5F4
 * initialised to zero**, which is the link between the two members of
 * the group.
 *
 * **`sltu $v0, $zero, $v0` is a boolean cast of the count**, the same
 * normalisation as `func_000804B8` and `func_001A9ABC`: 0x18($s0) may
 * have been a raw count and the caller wants "did anything happen".
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD6D4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s1, $a0, $zero\n\t"
        "bnez  $v0, .Leboot_000BD700\n\t"
        "nop\n\t"
        "b     .Leboot_000BD710\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BD700:\n\t"
        "jal   func_0019E4F8\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "sw    $v0, 0x24($s1)\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".Leboot_000BD710:\n\t"
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
/**
 * The Sims 2 PSP - func_000BFEB8 (0x000BFEB8, 0x7C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x24($a0)
 *     sw    $s1, 0x14($sp)
 *     ori   $s1, $zero, 0x1
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a1, .Leboot_000BFEF8
 *       or    $s0, $a0, $zero
 *     lw    $a2, 0x14($s0)
 *     addiu $a2, $a2, 0x20
 *     lw    $a0, 0x0($a2)
 *     ori   $a3, $zero, 0x1
 *     bne   $a0, $a3, .Leboot_000BFF00
 *       ori   $a1, $zero, 0x0
 *     b     .Leboot_000BFF04
 *       nop
 *   .Leboot_000BFEF8:
 *     b     .Leboot_000BFF20
 *       or    $v0, $s1, $zero
 *   .Leboot_000BFF00:
 *     addu  $a1, $a2, $a0
 *   .Leboot_000BFF04:
 *     lw    $a0, 0x0($s0)
 *     jal   func_000D8278
 *       addiu $a0, $a0, 0x4C
 *     lw    $a0, 0x24($s0)
 *     jal   func_000CB324
 *       or    $a1, $v0, $zero
 *     or    $v0, $s1, $zero
 *   .Leboot_000BFF20:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Walks to the object at `0x24($a0)`, notifies it through two functions,
 * and returns 1.
 *
 * **`$s1` is set to 1 before anything else happens and is never
 * reassigned, and every path ends writing it to `$v0`.  So this function
 * returns 1 unconditionally** - including the early-out at
 * `.Leboot_000BFEF8` when `0x24($a0)` is null, which branches straight to
 * the epilogue having called nothing.  The two `or $v0, $s1, $zero`
 * instructions, one in the early-out delay slot and one after the second
 * `jal`, are the same statement of the same constant.
 *
 * **That is the second half of the +0x01C story.**  Three of the four
 * overrides call `func_000B9D34` and branch on its result, which is dead
 * because that function returns 1; this one does not call it at all, has
 * a real test in it (`beqz $a1`, `bne $a0, $a3`), and **still returns
 * nothing but 1.**  So the four overrides are not four behaviours of one
 * question - three are `return 1` with extra work, and this is `return
 * 1` with different extra work.  Only `func_000BE138` can answer 0.
 *
 * **Six instructions in the middle compute nothing.**  `lw $a2, 0x14`,
 * `addiu $a2, 0x20`, `lw $a0, 0($a2)`, `ori $a3, 0x1`, the `bne` and the
 * `addu $a1, $a2, $a0` - they produce an index and an offset, and at
 * `.Leboot_000BFF04` the code reloads `$a0` from `$s0` and the delay slot
 * of the next `jal` overwrites `$a1` with `$v0`.  **Neither register is
 * read again.**  The `bne` does branch, and both of its destinations
 * arrive at `.Leboot_000BFF04` by different routes, but what it
 * distinguishes makes no difference to the result.
 *
 * What survives is: if `0x24($a0)` is null, do nothing; otherwise call
 * `func_000D8278(0x4C + this[0])` and then `func_000CB324(0x24($a0),
 * <its return>)`.  The `0x4C` is the second argument's field offset being
 * passed as an address, which is a common shape for "take a pointer to
 * one member of this object and hand it over".
 */
#include "types.h"

__attribute__((noreturn)) void func_000BFEB8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x24($a0)\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "ori   $s1, $zero, 0x1\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a1, .Leboot_000BFEF8\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a2, 0x14($s0)\n\t"
        "addiu $a2, $a2, 0x20\n\t"
        "lw    $a0, 0x0($a2)\n\t"
        "ori   $a3, $zero, 0x1\n\t"
        "bne   $a0, $a3, .Leboot_000BFF00\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "b     .Leboot_000BFF04\n\t"
        "nop\n\t"
        ".Leboot_000BFEF8:\n\t"
        "b     .Leboot_000BFF20\n\t"
        "or    $v0, $s1, $zero\n\t"
        ".Leboot_000BFF00:\n\t"
        "addu  $a1, $a2, $a0\n\t"
        ".Leboot_000BFF04:\n\t"
        "lw    $a0, 0x0($s0)\n\t"
        "jal   func_000D8278\n\t"
        "addiu $a0, $a0, 0x4C\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "jal   func_000CB324\n\t"
        "or    $a1, $v0, $zero\n\t"
        "or    $v0, $s1, $zero\n\t"
        ".Leboot_000BFF20:\n\t"
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
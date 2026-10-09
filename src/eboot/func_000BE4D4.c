/**
 * The Sims 2 PSP - func_000BE4D4 (0x000BE4D4, 0x6C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000B9D34
 *       or    $s0, $a0, $zero
 *     beqz  $v0, .Leboot_000BE50C
 *       nop
 *     or    $a0, $s0, $zero
 *     jal   func_000BA26C
 *       ori   $a1, $zero, 0x7
 *     bnez  $v0, .Leboot_000BE524
 *       nop
 *     b     .Leboot_000BE514
 *       or    $a0, $s0, $zero
 *   .Leboot_000BE50C:
 *     b     .Leboot_000BE530
 *       or    $v0, $zero, $zero
 *   .Leboot_000BE514:
 *     jal   func_000BA26C
 *       ori   $a1, $zero, 0x8
 *     beqz  $v0, .Leboot_000BE52C
 *       nop
 *   .Leboot_000BE524:
 *     lui   $a0, 0x8000
 *     sw    $a0, 0x28($s0)
 *   .Leboot_000BE52C:
 *     ori   $v0, $zero, 0x1
 *   .Leboot_000BE530:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Returns 1 after trying `func_000BA26C` twice with arguments 7 then 8.
 *
 * **The first call is dead and decidably so.**  `func_000B9D34` is the
 * shared +0x01C default of the eleven vtables; it is `ori $v0, $zero,
 * 0x1` and nothing else, so the `beqz $v0` right after the `jal` is
 * never taken and `.Leboot_000BE50C` - which is what returns 0 - is
 * unreachable.  **Every call path in this function ends up returning 1
 * or falling through to it.**
 *
 * That leaves the live body: call `func_000BA26C(this, 7)`, and if that
 * returns non-zero store `0x80000000` into `0x28($s0)`.  Otherwise call
 * it again with 8 and, *if that one returns zero*, fall into the same
 * store.  So the two calls are complementary and 0x28 is written when
 * exactly one of them answered non-zero - **an XOR of two predicates
 * landing in a flag field.**
 *
 * `0x80000000` is the constant, not an address: `lui $a0, 0x8000` loads
 * it.  Whether it is a sentinel, a limit, or a sign bit depends on who
 * reads 0x28, which these bytes do not say.
 *
 * **`func_000BE75C` is this function with 25 of its 27 words identical** -
 * the only differences are the two immediates, `0x7, 0x8` here against
 * `0x8, 0x9` there.  Two adjacent argument pairs through one body is
 * what a table indexed by class looks like after the compiler has
 * inlined it; it is also the reason the pair is worth keeping in one
 * file's worth of comment.
 *
 * Slot +0x01C of `sym_001EB060`; the same slot is left as
 * `func_000B9D34` in seven of the eleven vtables.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE4D4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000B9D34\n\t"
        "or    $s0, $a0, $zero\n\t"
        "beqz  $v0, .Leboot_000BE50C\n\t"
        "nop\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_000BA26C\n\t"
        "ori   $a1, $zero, 0x7\n\t"
        "bnez  $v0, .Leboot_000BE524\n\t"
        "nop\n\t"
        "b     .Leboot_000BE514\n\t"
        "or    $a0, $s0, $zero\n\t"
        ".Leboot_000BE50C:\n\t"
        "b     .Leboot_000BE530\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BE514:\n\t"
        "jal   func_000BA26C\n\t"
        "ori   $a1, $zero, 0x8\n\t"
        "beqz  $v0, .Leboot_000BE52C\n\t"
        "nop\n\t"
        ".Leboot_000BE524:\n\t"
        "lui   $a0, 0x8000\n\t"
        "sw    $a0, 0x28($s0)\n\t"
        ".Leboot_000BE52C:\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_000BE530:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
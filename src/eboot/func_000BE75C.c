/**
 * The Sims 2 PSP - func_000BE75C (0x000BE75C, 0x6C bytes)
 *
 * Identical to `func_000BE4D4` except that the two constants passed to
 * `func_000BA26C` are `0x8` and `0x9` instead of `0x7` and `0x8` -
 * **25 of the 27 words are the same**, and the two that differ are both
 * `ori $a1, $zero, <imm>`.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000B9D34
 *       or    $s0, $a0, $zero
 *     beqz  $v0, .Leboot_000BE794
 *       nop
 *     or    $a0, $s0, $zero
 *     jal   func_000BA26C
 *       ori   $a1, $zero, 0x8
 *     bnez  $v0, .Leboot_000BE7AC
 *       nop
 *     b     .Leboot_000BE79C
 *       or    $a0, $s0, $zero
 *   .Leboot_000BE794:
 *     b     .Leboot_000BE7B8
 *       or    $v0, $zero, $zero
 *   .Leboot_000BE79C:
 *     jal   func_000BA26C
 *       ori   $a1, $zero, 0x9
 *     beqz  $v0, .Leboot_000BE7A4
 *       nop
 *   .Leboot_000BE7AC:
 *     lui   $a0, 0x8000
 *     sw    $a0, 0x28($s0)
 *   .Leboot_000BE7A4:
 *     ori   $v0, $zero, 0x1
 *   .Leboot_000BE7B8:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * **Two functions, one body, two consecutive argument pairs.**  Taken
 * with `func_000BE4D4` the set is `(7,8)` and `(8,9)`, so the pairs
 * overlap and the pairs as a whole cover a contiguous run.  A third
 * such function would very likely be `(9,10)`; whether one exists is
 * not something these bytes can say, and guessing at it is not useful
 * unless something actually calls the run.
 *
 * **The `beqz $v0` after `func_000B9D34` is dead here too**, for the
 * same reason and by the same argument: that function returns 1
 * unconditionally, so `.Leboot_000BE794` - the path that returns 0 -
 * is unreachable.  **Two independent copies of the same dead branch**,
 * 0x280 bytes apart, is what makes this a property of the source rather
 * than an accident of one function.
 *
 * Slot +0x01C of `sym_001EB138`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE75C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000B9D34\n\t"
        "or    $s0, $a0, $zero\n\t"
        "beqz  $v0, .Leboot_000BE794\n\t"
        "nop\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_000BA26C\n\t"
        "ori   $a1, $zero, 0x8\n\t"
        "bnez  $v0, .Leboot_000BE7AC\n\t"
        "nop\n\t"
        "b     .Leboot_000BE79C\n\t"
        "or    $a0, $s0, $zero\n\t"
        ".Leboot_000BE794:\n\t"
        "b     .Leboot_000BE7B8\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BE79C:\n\t"
        "jal   func_000BA26C\n\t"
        "ori   $a1, $zero, 0x9\n\t"
        "beqz  $v0, .Leboot_000BE7A4\n\t"
        "nop\n\t"
        ".Leboot_000BE7AC:\n\t"
        "lui   $a0, 0x8000\n\t"
        "sw    $a0, 0x28($s0)\n\t"
        ".Leboot_000BE7A4:\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_000BE7B8:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
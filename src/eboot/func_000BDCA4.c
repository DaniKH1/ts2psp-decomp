/**
 * The Sims 2 PSP - func_000BDCA4 (0x000BDCA4, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a1, $zero
 *     sw    $s1, 0x14($sp)
 *     sw    $ra, 0x18($sp)
 *     jal   func_000A7AC8
 *       or    $s1, $a0, $zero
 *     bnez  $v0, .Leboot_000BDCD0
 *       nop
 *     b     .Leboot_000BDCE0
 *       or    $v0, $zero, $zero
 *   .Leboot_000BDCD0:
 *     jal   func_0019E8F0
 *       lw    $a0, 0x18($s0)
 *     sw    $v0, 0x24($s1)
 *     sltu  $v0, $zero, $v0
 *   .Leboot_000BDCE0:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x014 of `sym_001EADD8`.  **19 of its 20 words are identical to
 * `func_000BD6D4`**, which fills the same slot in the first vtable;
 * the single word that differs is the `jal` at word 11, here
 * `func_0019E8F0` against `func_0019E4F8` there.
 *
 * **+0x014 is a one-word template across the family** - nine of the ten
 * siblings differ from the first in exactly one instruction, where the
 * constructors of slot +0x0C needed two.  So this slot varies less than
 * the constructors do, and what varies is narrower: there is no
 * per-class vtable pointer here, only the callee.
 *
 * The template asks `func_000A7AC8` whether to proceed and returns 0
 * early if it says no; otherwise it calls the per-class function on
 * `0x18($s0)`, stores the count into `0x24($s1)`, and returns it cast
 * to a boolean with `sltu $v0, $zero, $v0`.  **The caller gets "did
 * anything happen", not the count.**
 *
 * **The eleventh member of this slot, `func_000BFC2C`, is 652 bytes**
 * and has twenty labels, so it is not this template at all; the tenth,
 * `func_000BDEC4`, is 84 bytes - one word longer.  Those two are the
 * exceptions this file is not.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDCA4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s1, $a0, $zero\n\t"
        "bnez  $v0, .Leboot_000BDCD0\n\t"
        "nop\n\t"
        "b     .Leboot_000BDCE0\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BDCD0:\n\t"
        "jal   func_0019E8F0\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "sw    $v0, 0x24($s1)\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".Leboot_000BDCE0:\n\t"
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
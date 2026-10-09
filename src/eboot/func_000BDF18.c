/**
 * The Sims 2 PSP - func_000BDF18 (0x000BDF18, 0xF0 bytes)
 *
 * Slot +0x034 of `sym_001EAEB0`.  **This is `func_000BDCF4` with
 * every destination offset raised by exactly 4** - 0xF0 to 0xF4, 0xF8
 * to 0xFC, 0x104 to 0x108, 0x110 to 0x114, 0x11C to 0x120, 0x128 to
 * 0x12C - and **nothing else changed**: same three 12-byte blocks, same
 * two-word copy, same float, same instruction for instruction
 * everywhere else.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000BD3A4
 *       or    $s0, $a0, $zero
 *     lw    $a0, 0x8($s0)
 *     lwc1  $f13, 0x14($a0)
 *     mtc1  $zero, $f12
 *     lw    $a0, 0x24($s0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1tl .Leboot_000BDF4C
 *       mov.s $f13, $f12
 *   .Leboot_000BDF4C:
 *     swc1  $f13, 0xF4($a0)
 *     ... the same 46 words as func_000BDCF4, at the shifted offsets
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x44($a0)
 *     swc1  $f12, 0x12C($a1)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * **Six immediates differ between the two and all six move the same
 * way, which is what makes this decidable rather than a guess:** the two
 * classes store an identical record layout, one word apart in the
 * destination object.  Either the destination has a header that differs
 * by a word between the classes, or the two classes' destinations are
 * consecutive elements of an array.  **Nothing here says which** - the
 * bytes only give the offsets - but either way the gap between the two
 * is exactly one word, not an arbitrary amount.
 *
 * **The clamp is the same `bc1tl` as the sibling's and means the same
 * thing.**  `$f12` is 0.0f, `c.le.s $f13, $f12` asks whether the loaded
 * value is at most zero, and the branch is nullifying - so the stored
 * value is the original when it is at most zero and 0.0f otherwise.
 * **That is `min(x, 0.0f)`, a clamp from above**, and the `l` in
 * `bc1tl` is the whole difference between this and its opposite.
 *
 * **The destination object is at least 0x130 bytes here**, against
 * 0x12C for the sibling - **0x10C more than the 0x24 offset of the
 * getter that returns it.**  Across this slot the members have now
 * pinned that object at 0xB8, 0x12C and 0x130 bytes, from the
 * destination offsets alone.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDF18(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000BD3A4\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lwc1  $f13, 0x14($a0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl .Leboot_000BDF4C\n\t"
        "mov.s $f13, $f12\n\t"
        ".Leboot_000BDF4C:\n\t"
        "swc1  $f13, 0xF4($a0)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x18\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0xFC\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x24\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x108\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x30\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x114\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x3C\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0x120\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x44($a0)\n\t"
        "swc1  $f12, 0x12C($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
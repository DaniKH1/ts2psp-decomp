/**
 * The Sims 2 PSP - func_000BE7C8 (0x000BE7C8, 0xF0 bytes)
 *
 * Slot +0x034 of `sym_001EB138`.  **52 of its 60 words are identical to
 * `func_000BE540`**, the same slot in the previous record; the tail is
 * the same bitfield code, the same flag clear, and the same two dead
 * unsigned-to-float conversions.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000BD3A4
 *       or    $s0, $a0, $zero
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     addiu $a0, $a0, 0x14
 *     lw    $a2, 0x0($a0)
 *     lw    $a3, 0x4($a0)
 *     addiu $a1, $a1, 0xB0
 *     lw    $a0, 0x8($a0)
 *     sw    $a2, 0x0($a1)
 *     sw    $a3, 0x4($a1)
 *     sw    $a0, 0x8($a1)
 *     lw    $a0, 0x24($s0)
 *     lw    $a0, 0xBC($a0)
 *     mtc1  $a0, $f12
 *     bgez  $a0, .Leboot_000BE824
 *       cvt.s.w $f12, $f12
 *     lui   $a0, 0x4F80
 *     mtc1  $a0, $f13
 *     add.s $f12, $f12, $f13
 *   .Leboot_000BE824:
 *     or    $a0, $s0, $zero
 *     jal   updateNodeGraph_08C4
 *       ori   $a1, $zero, 0x8
 *     lw    $a0, 0x24($s0)
 *     lw    $a0, 0xC0($a0)
 *     mtc1  $a0, $f12
 *     bgez  $a0, .Leboot_000BE850
 *       cvt.s.w $f12, $f12
 *     lui   $a0, 0x4F80
 *     mtc1  $a0, $f13
 *     add.s $f12, $f12, $f13
 *   .Leboot_000BE850:
 *     or    $a0, $s0, $zero
 *     jal   updateNodeGraph_08C4
 *       ori   $a1, $zero, 0x9
 *     lw    $a0, 0x28($s0)
 *     beqz  $a0, .Leboot_000BE8A8
 *       nop
 *     ... the same 18-word bitfield-and-flag tail
 *   .Leboot_000BE8A8:
 *     lw    $s0, 0x10($sp)
 *     ...
 *
 * **Three differences from `func_000BE540`, all of them small.**
 *
 * 1. **The opening copy is a 12-byte block, not two floats.**  Where the
 *    sibling copies `0x14` and `0x18` as separate floats, this one
 *    advances a pointer by 0x14 and copies three consecutive words to
 *    `0xB0` - the same 12-byte-block shape `func_000BDCF4` uses, so
 *    **this class copies a whole struct where its sibling copies two
 *    fields of it.**  Since `func_000BDCF4` copied 12-byte blocks from
 *    `0x18`/`0x24`/`0x30`, a block starting at `0x14` is the same
 *    element type one slot earlier.
 *
 * 2. **The dead conversions read `0xBC` and `0xC0` instead of `0xB8`
 *    and `0xBC`** - the same two-word window shifted by one word, which
 *    is the same +4 relationship `func_000BDF18` has to `func_000BDCF4`.
 *
 * 3. **The two calls pass 8 then 9**, against the sibling's 7 then 8.
 *
 * **So the 7/8 pair and the 8/9 pair both exist, in two different
 * slots, calling two different functions.**  `func_000BE4D4` and
 * `func_000BE75C` at slot +0x01C pass `(7,8)` and `(8,9)` to
 * `func_000BA26C`; `func_000BE540` and `func_000BE7C8` here pass `(7,8)`
 * and `(8,9)` to `updateNodeGraph_08C4` at 0x1B6144.  **Four
 * functions, two of each, all passing one adjacent pair - and the
 * pattern crosses slots, so it is a convention about how these classes
 * address adjacent channels rather than a quirk of one method.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE7C8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000BD3A4\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x14\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0xB0\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "lw    $a0, 0xBC($a0)\n\t"
        "mtc1  $a0, $f12\n\t"
        "bgez  $a0, .Leboot_000BE824\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "lui   $a0, 0x4F80\n\t"
        "mtc1  $a0, $f13\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".Leboot_000BE824:\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   updateNodeGraph_08C4\n\t"
        "ori   $a1, $zero, 0x8\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "lw    $a0, 0xC0($a0)\n\t"
        "mtc1  $a0, $f12\n\t"
        "bgez  $a0, .Leboot_000BE850\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "lui   $a0, 0x4F80\n\t"
        "mtc1  $a0, $f13\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".Leboot_000BE850:\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   updateNodeGraph_08C4\n\t"
        "ori   $a1, $zero, 0x9\n\t"
        "lw    $a0, 0x28($s0)\n\t"
        "beqz  $a0, .Leboot_000BE8A8\n\t"
        "nop\n\t"
        "lw    $a1, 0xC($s0)\n\t"
        "lw    $a2, 0x0($s0)\n\t"
        "or    $a0, $a1, $a0\n\t"
        "sw    $a0, 0xC($s0)\n\t"
        "lw    $a0, 0x4($s0)\n\t"
        "lw    $a1, 0x38($a2)\n\t"
        "sra   $a3, $a0, 5\n\t"
        "sll   $a3, $a3, 0x2\n\t"
        "addu  $a1, $a1, $a3\n\t"
        "lw    $a3, 0x0($a1)\n\t"
        "andi  $a0, $a0, 0x1F\n\t"
        "ori   $t0, $zero, 0x1\n\t"
        "sllv  $a0, $t0, $a0\n\t"
        "or    $a0, $a3, $a0\n\t"
        "sw    $a0, 0x0($a1)\n\t"
        "sb    $zero, 0x34($a2)\n\t"
        ".Leboot_000BE8A8:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
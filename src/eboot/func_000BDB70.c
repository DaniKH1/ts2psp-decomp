/**
 * The Sims 2 PSP - func_000BDB70 (0x000BDB70, 0x54 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000BD3A4
 *       or    $s0, $a0, $zero
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x1C($a0)
 *     swc1  $f12, 0xB0($a1)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x14($a0)
 *     swc1  $f12, 0xB4($a1)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x18($a0)
 *     swc1  $f12, 0xB8($a1)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x034 of `sym_001EAD00` - three fields, 21 words, and **the
 * member that fixes the destination's lower bound at 0xB8.**
 *
 * **The source offsets are not in order.**  The loads are `0x1C`,
 * `0x14`, `0x18` while the stores are `0xB0`, `0xB4`, `0xB8` in
 * sequence.  So the first field written is the one at source `0x1C`,
 * not the one at `0x14`, and the source and destination field orders
 * disagree.  That is decidable and it is not an accident of the
 * listing: it means the source-level statement order was
 * `0x1C`, `0x14`, `0x18` rather than ascending, **so this member was
 * written by hand in a different order than the two-field members and
 * the compiler preserved that order.**
 *
 * This is the only member of the slot with an out-of-order source, and
 * it is the reason the slot cannot be described as "a copy loop": three
 * of these eleven functions are hand-unrolled copies and one of them
 * does not even agree with itself about which field comes first.
 *
 * `func_000BDB70` is 21 words, which is `9 + 4 * 3` - the same formula
 * the other members follow, so the whole slot scales by four words per
 * field with no per-field branch or setup.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDB70(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000BD3A4\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x1C($a0)\n\t"
        "swc1  $f12, 0xB0($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x14($a0)\n\t"
        "swc1  $f12, 0xB4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x18($a0)\n\t"
        "swc1  $f12, 0xB8($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
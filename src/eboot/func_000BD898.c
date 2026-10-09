/**
 * The Sims 2 PSP - func_000BD898 (0x000BD898, 0x34 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000BD3A4
 *       or    $s0, $a0, $zero
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x14($a0)
 *     swc1  $f12, 0xB0($a1)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x034 of `sym_001EAB50`.  **The whole of this slot is a
 * "gather some fields, push them somewhere" template**, and this is its
 * shortest member: one field copied, 13 words.
 *
 * The common shape is four words per field -
 *
 *   lw    $a0, 0x8($s0)      the source
 *   lw    $a1, 0x24($s0)     the destination
 *   lwc1  $f12, <src>($a0)
 *   swc1  $f12, <dst>($a1)
 *
 * - so the members of this slot are distinguished by **how many fields
 * they copy and which ones**, and the slot length is
 * `9 + 4 * (number of fields)`.  `func_000BD724` copies two floats,
 * this one one, `func_000BDB70` three.
 *
 * **The destination is always at least 0xB8 bytes**, since 0xB0 and
 * 0xB4 are written by the two-field members and 0xB8 by the three-field
 * one.  That is the same bound `func_000BD724` already documented, and
 * it is 0x94 past the 0x24 offset of the getter that hands out the
 * destination - **another instance of a 0x24 accessor describing nothing
 * about the size of what it returns.**
 *
 * The first member of this slot, `func_000BD724`, and this one share
 * their first thirteen words except for the field offsets: the
 * `lwc1`/`swc1` pair here is the *first* of `func_000BD724`'s two.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD898(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000BD3A4\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x14($a0)\n\t"
        "swc1  $f12, 0xB0($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
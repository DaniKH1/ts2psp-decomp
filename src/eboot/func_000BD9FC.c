/**
 * The Sims 2 PSP - func_000BD9FC (0x000BD9FC, 0x44 bytes)
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
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x18($a0)
 *     swc1  $f12, 0xB4($a1)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x034 of `sym_001EAC28`.  Copies two floats from `0x14` and
 * `0x18` of the source at `0x8($s0)` into `0xB0` and `0xB4` of the
 * destination at `0x24($s0)`.
 *
 * **All 17 words are identical to `func_000BD724`**, which fills the
 * same slot in the first vtable - same offsets, same order, no
 * difference at all.  So this slot's third member duplicates its first,
 * and the field it copies is the same pair.
 *
 * **The destination is at least 0xB8 bytes** - 0xB0 and 0xB4 are
 * written, and `func_000BDB70` writes 0xB8 as well - which is 0x94 past
 * the 0x24 offset of the getter that hands out the destination.  **A
 * 0x24 accessor describing nothing about the size of what it returns.**
 *
 * Every field costs four words: the two `lw`s reload the source and
 * destination rather than keeping them in registers, which is why the
 * members of this slot scale linearly in length rather than paying a
 * single setup.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD9FC(void) {
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
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x18($a0)\n\t"
        "swc1  $f12, 0xB4($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
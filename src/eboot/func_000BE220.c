/**
 * The Sims 2 PSP - func_000BE220 (0x000BE220, 0x180 bytes)
 *
 * Slot +0x034 of `sym_001EAF88` - **the largest member of the slot**,
 * and the one that copies the most fields: eight blocks plus two floats
 * plus one converted integer.
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
 *     bc1tl .Leboot_000BE254
 *       mov.s $f13, $f12
 *   .Leboot_000BE254:
 *     swc1  $f13, 0xB0($a0)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x18($a0)
 *     swc1  $f12, 0xB8($a1)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x1C($a0)
 *     trunc.w.s $f12, $f12
 *     mfc1  $a0, $f12
 *     sw    $a0, 0xBC($a1)
 *     ... eight block copies, source 0x20/0x2C/0x38/0x40/0x48/0x50/
 *         0x58/0x60 to destination 0xC0/0xCC/0xD8/0xE0/0xE8/0xF0/
 *         0xF8/0x100
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * **The first three fields show this class stores four representations of
 * related data.**  `0x14` goes through the same `bc1tl` clamp the other
 * members use - `min(x, 0.0f)`, because `$f12` is 0.0f and the branch
 * nullifies its delay slot.  `0x18` goes across unchanged as a float.
 * **And `0x1C` is converted the other way** - `trunc.w.s` then `mfc1` -
 * **so a float in the source becomes a word at `0xBC` in the
 * destination.**  The field pair is a float copied, a float clamped,
 * and a float truncated to an integer, which is what a record holding
 * one logical quantity in three encodings looks like.
 *
 * **The eight blocks are two 12-byte structs followed by six 8-byte
 * pairs, and the strides match exactly on both sides** - source 0x20,
 * 0x2C, 0x38 then 0x40 through 0x60 by 8; destination 0xC0, 0xCC,
 * 0xD8 then 0xE0 through 0x100 by 8.  **So the source and destination
 * records have identical field layout**, which is why the copies are
 * three-word and two-word blocks rather than a single flat memcpy: the
 * compiler unrolled member assignments, and each member's size is
 * visible as its block size.
 *
 * Note that `0x38` and `0x40` are 8 apart while the two 12-byte structs
 * before them are 12 apart - **so the record is not uniformly packed
 * and the elements are not an array of one type.**
 *
 * The destination object is **at least 0x108 bytes** - `0x104` is the
 * last byte written - against the 0x130 that `func_000BDF18` implied.
 * **The four members of this slot that write past 0xB0 disagree about
 * the object's size, so 0x24($s0) is a pointer into something whose
 * extent none of them agrees on.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE220(void) {
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
        "bc1tl .Leboot_000BE254\n\t"
        "mov.s $f13, $f12\n\t"
        ".Leboot_000BE254:\n\t"
        "swc1  $f13, 0xB0($a0)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x18($a0)\n\t"
        "swc1  $f12, 0xB8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x1C($a0)\n\t"
        "trunc.w.s $f12, $f12\n\t"
        "mfc1  $a0, $f12\n\t"
        "sw    $a0, 0xBC($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x20\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0xC0\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x2C\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0xCC\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x38\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0xD8\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x40\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0xE0\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x48\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0xE8\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x50\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0xF0\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x58\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0xF8\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x60\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0x100\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
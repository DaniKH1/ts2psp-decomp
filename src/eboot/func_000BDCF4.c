/**
 * The Sims 2 PSP - func_000BDCF4 (0x000BDCF4, 0xF0 bytes)
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
 *     bc1tl .Leboot_000BDD28
 *       mov.s $f13, $f12
 *   .Leboot_000BDD28:
 *     swc1  $f13, 0xF0($a0)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     addiu $a0, $a0, 0x18
 *     lw    $a2, 0x0($a0)
 *     lw    $a3, 0x4($a0)
 *     addiu $a1, $a1, 0xF8
 *     lw    $a0, 0x8($a0)
 *     sw    $a2, 0x0($a1)
 *     sw    $a3, 0x4($a1)
 *     sw    $a0, 0x8($a1)
 *     ... and the same ten words again for 0x24 -> 0x104
 *     ... and again for 0x30 -> 0x110
 *     ... and a two-word copy for 0x3C -> 0x11C
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x44($a0)
 *     swc1  $f12, 0x128($a1)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x034 of `sym_001EADD8` - the first member of this slot that
 * is not a plain field-by-field float copy.
 *
 * **The `bc1tl` at the top is a clamp, and it is decidable.**  `$f12` is
 * loaded with `mtc1 $zero`, so it is 0.0f, and `c.le.s $f13, $f12` asks
 * whether the loaded value is at most zero.  The branch is the
 * **nullifying** form `bc1tl`, so its delay slot does not execute when
 * the branch is taken:
 *
 *   f13 <= 0.0  ->  branch taken,  `mov.s` nullified, $f13 unchanged
 *   f13 >  0.0  ->  branch clear,  `mov.s` runs,     $f13 = 0.0
 *
 * and the value stored is the one that survives.  **So this stores
 * `min(source->0x14, 0.0f)`** - it clamps from above at zero, not from
 * below.  Had the branch been the plain `bc1t` the same source would
 * have produced the opposite clamp, and the `l` in `bc1tl` is the only
 * thing in the instruction that decides which.
 *
 * **Three 12-byte blocks, then two words, then a float.**  The blocks at
 * `0x18`, `0x24` and `0x30` are each three consecutive words copied to
 * `0xF8`, `0x104` and `0x110` - **0xC apart on both sides, matching
 * stride** - so they are three elements of one 12-byte struct.  The
 * copy is emitted with `$a0` advanced and both source words reloaded
 * after the pointer bump, because `lw $a0, 0x8($a0)` reads the third
 * word *through* the advanced pointer.
 *
 * **The destination object is at least 0x12C bytes.**  `0x128` is the
 * highest offset written and it is a float, so the object must be 0x12C
 * bytes at least.  **That is 0x108 more than the 0x24 offset of the
 * getter that returns it** - the largest gap found for that field, and
 * a much stronger version of the 0xB8 bound the smaller members of this
 * slot implied.  Whatever is at `0x24($s0)` is not a small object.
 *
 * `func_000BDF18` is this function with **every destination offset
 * raised by exactly 4**, and nothing else changed - same block order,
 * same sizes, same clamp.  So the two classes store an identical layout
 * at two different offsets inside their destination, one word apart.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BDCF4(void) {
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
        "bc1tl .Leboot_000BDD28\n\t"
        "mov.s $f13, $f12\n\t"
        ".Leboot_000BDD28:\n\t"
        "swc1  $f13, 0xF0($a0)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x18\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0xF8\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x24\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x104\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x30\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x110\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x3C\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, 0x11C\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a0, 0x4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x44($a0)\n\t"
        "swc1  $f12, 0x128($a1)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
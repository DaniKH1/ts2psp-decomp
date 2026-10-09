/**
 * The Sims 2 PSP - func_000BD724 (0x000BD724, 0x44 bytes)
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
 *     addiu $sp, $sp, 0x20
 *
 * The last member of the group that starts at func_000BD5F4: it copies
 * two floats out of one sub-object and into another.
 *
 * **Both halves reload `0x8($s0)` and `0x24($s0)`.**  Nothing in
 * `lwc1`/`swc1` clobbers either register, so the second pair of loads
 * is redundant in any ordinary reading - the same compiler habit as the
 * three reloads in `func_000BBDC8`.  **Recorded as redundant rather than
 * explained away.**
 *
 * The floats come from 0x14 and 0x18 of the object at `0x8($s0)` and go
 * to **0xB0 and 0xB4 of the object at `0x24($s0)`** - two different
 * sub-objects, source and destination.  `$a1` is loaded purely to be the
 * store base, which is why it is reloaded rather than kept.
 *
 * So the object reached through `0x24($s0)` is **at least 0xB8 bytes**.
 * The getter at the head of the group reads offset 0x24 of `$s0` and
 * says nothing about any of this: **a single-field getter and the size
 * of the object it hands out are independent facts.**
 *
 * Note the epilogue reloads `$s0` as well as `$ra`, even though `$s0`
 * has been dead since the second `swc1`.  **The reload is not
 * redundant on the same grounds as the data loads** - it is the ordinary
 * callee-saved-reg restore, and dropping it makes the function 4 bytes
 * short.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD724(void) {
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
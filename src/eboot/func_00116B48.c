/**
 * The Sims 2 PSP - func_00116B48 (0x00116B48, 0x34 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a2, 0xC($a0)
 *     sw    $ra, 0x10($sp)
 *     bne   $a2, $a1, .Leboot_00116B6C
 *       nop
 *     jal   func_00116990
 *       nop
 *     b     .Leboot_00116B70
 *       ori   $v0, $zero, 0x1
 *   .Leboot_00116B6C:
 *     or    $v0, $zero, $zero
 *   .Leboot_00116B70:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Returns 1 if the cached version number at 0xC($a0) equals the one
 * asked for in $a1, and 0 if not - **except that on the equal path it
 * first calls func_00116990.**
 *
 * **So the name is backwards from what it looks like.**  A cache
 * compare-and-refresh: the hit path is the expensive one.  `func_00116990`
 * is called *before* the 1 is produced, and its own return value is
 * discarded - the `ori $v0, $zero, 0x1` is unconditional on that path,
 * so a refresh that failed still reports a hit.
 *
 * Both the `ori` and the `nop` sit in delay slots of branches whose
 * targets are both after it, which is what makes the two paths join at
 * a single epilogue.
 */
#include "types.h"

__attribute__((noreturn)) void func_00116B48(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a2, 0xC($a0)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "bne   $a2, $a1, .Leboot_00116B6C\n\t"
        "nop\n\t"
        "jal   func_00116990\n\t"
        "nop\n\t"
        "b     .Leboot_00116B70\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_00116B6C:\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_00116B70:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00002890 (0x00002890, 0x3C bytes)
 *
 * Checks a flag at offset 0xB9, then conditionally checks a pointer
 * at offset 0xE4, and calls func_00000FD4 if either check fails.
 *
 *     addiu $sp, $sp, -0x20
 *     lbu   $a1, 0xB9($a0)
 *     sw    $ra, 0x10($sp)
 *     beqz  $a1, .Leboot_000028B8
 *     nop
 *     lw    $a1, 0xE4($a0)
 *     bnez  $a1, .Leboot_000028B8
 *     nop
 *     b     .Leboot_000028C0
 *       or  $v0, $zero, $zero
 *   .Leboot_000028B8:
 *     jal   func_00000FD4
 *     nop
 *   .Leboot_000028C0:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Returns 0 in $v0 if flag at 0xB9 is set AND pointer at 0xE4 is non-zero.
 * Otherwise calls func_00000FD4 and returns its result.
 */
#include "types.h"

__attribute__((noreturn)) void func_00002890(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lbu   $a1, 0xB9($a0)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "beqz  $a1, .Leboot_000028B8\n\t"
        "nop\n\t"
        "lw    $a1, 0xE4($a0)\n\t"
        "bnez  $a1, .Leboot_000028B8\n\t"
        "nop\n\t"
        "b     .Leboot_000028C0\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000028B8:\n\t"
        "jal   func_00000FD4\n\t"
        "nop\n\t"
        ".Leboot_000028C0:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
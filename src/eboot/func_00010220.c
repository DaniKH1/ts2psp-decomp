/**
 * The Sims 2 PSP - func_00010220 (0x00010220, 0x3C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     bnez  $a0, .Leboot_0001023C
 *     or    $s0, $a0, $zero
 *     b     .Leboot_0001024C
 *       nop
 *   .Leboot_0001023C:
 *     jal   func_00102230
 *     nop
 *     jal   func_0000D8BFC
 *       or  $a0, $s0, $zero
 *   .Leboot_0001024C:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 */
#include "types.h"

__attribute__((noreturn)) void func_00010220(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "bnez  $a0, .Leboot_0001023C\n\t"
        "or    $s0, $a0, $zero\n\t"
        "b     .Leboot_0001024C\n\t"
        "nop\n\t"
        ".Leboot_0001023C:\n\t"
        "jal   func_00102230\n\t"
        "nop\n\t"
        "jal   func_000D8BFC\n\t"
        "or    $a0, $s0, $zero\n\t"
        ".Leboot_0001024C:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
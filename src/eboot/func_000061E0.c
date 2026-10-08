/**
 * The Sims 2 PSP - func_000061E0 (0x000061E0, 0x68 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_00006234
 *       or    $s0, $a1, $zero
 *     lui   $a0, %hi(sym_001E33A4)
 *     addiu $a0, $a0, %lo(sym_001E33A4)
 *     sw    $a0, 0x38($s1)
 *     lui   $a0, %hi(sym_001E3414)
 *     addiu $a0, $a0, %lo(sym_001E3414)
 *     sw    $a0, 0xC($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_00055120
 *       or  $a1, $zero, $zero
 *     andi  $a0, $s0, 0x1
 *     beqz  $a0, .Leboot_00006234
 *     nop
 *     jal   func_0012771C
 *       or  $a0, $s1, $zero
 *   .Leboot_00006234:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function initializes an object with two global symbol pointers
 * (sym_001E33A4 and sym_001E3414), then conditionally calls
 * func_00055120 and func_0012771C based on a flag.
 */
#include "types.h"

__attribute__((noreturn)) void func_000061E0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_00006234\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, %%hi(sym_001E33A4)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E33A4)\n\t"
        "sw    $a0, 0x38($s1)\n\t"
        "lui   $a0, %%hi(sym_001E3414)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E3414)\n\t"
        "sw    $a0, 0xC($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_00055120\n\t"
        "or    $a1, $zero, $zero\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "beqz  $a0, .Leboot_00006234\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_00006234:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
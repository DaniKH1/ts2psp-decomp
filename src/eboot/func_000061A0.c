/**
 * The Sims 2 PSP - func_000061A0 (0x000061A0, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000550E0
 *       or  $s0, $a0, $zero
 *     lui   $a0, %hi(sym_001E33A4)
 *     addiu $a0, $a0, %lo(sym_001E33A4)
 *     sw    $a0, 0x38($s0)
 *     lui   $a0, %hi(sym_001E3414)
 *     addiu $a0, $a0, %lo(sym_001E3414)
 *     sw    $a0, 0xC($s0)
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function calls func_000550E0 with the object pointer, then
 * writes two global symbol pointers (sym_001E33A4 and sym_001E3414)
 * to offsets 0x38 and 0xC of the object, and returns the object pointer.
 */
#include "types.h"

__attribute__((noreturn)) void func_000061A0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000550E0\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a0, %%hi(sym_001E33A4)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E33A4)\n\t"
        "sw    $a0, 0x38($s0)\n\t"
        "lui   $a0, %%hi(sym_001E3414)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E3414)\n\t"
        "sw    $a0, 0xC($s0)\n\t"
        "or    $v0, $s0, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
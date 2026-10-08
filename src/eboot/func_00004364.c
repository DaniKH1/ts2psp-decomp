/**
 * The Sims 2 PSP - func_00004364 (0x00004364, 0x40 bytes)
 *
 * Sister function to func_000042B0. Calls func_0008B0B8 on the object,
 * then sets two pointer fields on the object from different global symbols.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_0008B0B8
 *       or  $s0, $a0, $zero
 *     lui   $a0, %hi(sym_001E30DC)
 *     addiu $a0, $a0, %lo(sym_001E30DC)
 *     sw    $a0, 0x38($s0)
 *     lui   $a0, %hi(sym_001E314C)
 *     addiu $a0, $a0, %lo(sym_001E314C)
 *     sw    $a0, 0xC($s0)
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Identical structure to func_000042B0 but with different target symbols:
 *   sym_001E30DC and sym_001E314C instead of sym_001E3034 and sym_001E30A4.
 */
#include "types.h"

__attribute__((noreturn)) void func_00004364(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_0008B0B8\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a0, %%hi(sym_001E30DC)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E30DC)\n\t"
        "sw    $a0, 0x38($s0)\n\t"
        "lui   $a0, %%hi(sym_001E314C)\n\t"
        "addiu $a0, $a0, %%lo(sym_001E314C)\n\t"
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
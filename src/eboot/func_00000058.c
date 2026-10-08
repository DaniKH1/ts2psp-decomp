/**
 * The Sims 2 PSP - func_00000058 (0x00000058, 0x58 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_00001DF4)
 *     lw    $a1, %lo(sym_00001DF4)($a0)
 *     sw    $s0, 0x10($sp)
 *     lui   $s0, %hi(sym_00001DF8)
 *     sw    $ra, 0x14($sp)
 *     bnez  $a1, .Leboot_0000009C
 *       addiu $s0, $s0, %lo(sym_00001DF8)
 *     ori   $a1, $zero, 0x1
 *     sw    $a1, %lo(sym_00001DF4)($a0)
 *     lui   $a1, %hi(str_collisionTweaks)
 *     or    $a0, $s0, $zero
 *     jal   func_000A4080
 *       addiu $a1, $a1, %lo(str_collisionTweaks)
 *     lui   $a0, %hi(sym_001D1B2C)
 *     jal   func_0014C5D8
 *       addiu $a0, $a0, %lo(sym_001D1B2C)
 *   .Leboot_0000009C:
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function initializes collision tweaks. If a flag at sym_00001DF4 is set,
 * it registers a collision tweak string. Then it calls func_0014C5D8 with
 * a symbol address. Returns the value of sym_00001DF8 in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_00000058(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_00001DF4)\n\t"
        "lw    $a1, %%lo(sym_00001DF4)($a0)\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "lui   $s0, %%hi(sym_00001DF8)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "bnez  $a1, .Leboot_0000009C\n\t"
        "addiu $s0, $s0, %%lo(sym_00001DF8)\n\t"
        "ori   $a1, $zero, 0x1\n\t"
        "sw    $a1, %%lo(sym_00001DF4)($a0)\n\t"
        "lui   $a1, %%hi(str_collisionTweaks)\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_000A4080\n\t"
        "addiu $a1, $a1, %%lo(str_collisionTweaks)\n\t"
        "lui   $a0, %%hi(sym_001D1B2C)\n\t"
        "jal   func_0014C5D8\n\t"
        "addiu $a0, $a0, %%lo(sym_001D1B2C)\n\t"
        ".Leboot_0000009C:\n\t"
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
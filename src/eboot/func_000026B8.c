/**
 * The Sims 2 PSP - func_000026B8 (0x000026B8, 0x60 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x14($sp)
 *     or    $s0, $a0, $zero
 *     swc1  $f20, 0x10($sp)
 *     mov.s $f20, $f12
 *     lui   $a1, %hi(str_NavigateAndWendToSeat_onUpdate)
 *     ori   $a0, $zero, 0x1C
 *     sw    $ra, 0x18($sp)
 *     jal   func_00056B9C
 *       addiu $a1, $a1, %lo(str_NavigateAndWendToSeat_onUpdate)
 *     or    $a0, $s0, $zero
 *     jal   func_000036F4
 *       mov.s $f12, $f20
 *     lbu   $a0, 0xB9($s0)
 *     bnez  $a0, .Leboot_000026FC
 *       nop
 *     sw    $zero, 0xE4($s0)
 *   .Leboot_000026FC:
 *     jal   func_00056BA4
 *       ori   $a0, $zero, 0x1C
 *     lwc1  $f20, 0x10($sp)
 *     lw    $s0, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function appears to be a navigation-related function that
 * registers an update callback, then checks a flag at offset 0xB9.
 * If the flag is set, it calls func_00056BA4. Otherwise it clears
 * offset 0xE4 and calls func_00056BA4 with a different parameter.
 */
#include "types.h"

__attribute__((noreturn)) void func_000026B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x14($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "swc1  $f20, 0x10($sp)\n\t"
        "mov.s $f20, $f12\n\t"
        "lui   $a1, %%hi(str_NavigateAndWendToSeat_onUpdate)\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_00056B9C\n\t"
        "addiu $a1, $a1, %%lo(str_NavigateAndWendToSeat_onUpdate)\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_000036F4\n\t"
        "mov.s $f12, $f20\n\t"
        "lbu   $a0, 0xB9($s0)\n\t"
        "bnez  $a0, .Leboot_000026FC\n\t"
        "nop\n\t"
        "sw    $zero, 0xE4($s0)\n\t"
        ".Leboot_000026FC:\n\t"
        "jal   func_00056BA4\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "lwc1  $f20, 0x10($sp)\n\t"
        "lw    $s0, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
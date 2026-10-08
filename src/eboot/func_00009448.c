/**
 * The Sims 2 PSP - func_00009448 (0x00009448, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a1, %hi(func_00009378)
 *     ori   $a0, $zero, 0x1F
 *     sw    $ra, 0x10($sp)
 *     jal   func_00086574
 *       addiu $a1, $a1, %lo(func_00009378)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Calls func_00086574 with a function pointer (func_00009378) and
 * a constant 0x1F (31) as arguments.
 */
#include "types.h"

__attribute__((noreturn)) void func_00009448(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a1, %%hi(func_00009378)\n\t"
        "ori   $a0, $zero, 0x1F\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00086574\n\t"
        "addiu $a1, $a1, %%lo(func_00009378)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
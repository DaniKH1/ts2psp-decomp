/**
 * The Sims 2 PSP - func_0000946C (0x0000946C, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a1, %hi(func_000093E0)
 *     ori   $a0, $zero, 0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_00086574
 *       addiu $a1, $a1, %lo(func_000093E0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Sister function to func_00009448. Calls func_00086574 with
 * a different function pointer (func_000093E0) and constant 0x20 (32).
 */
#include "types.h"

__attribute__((noreturn)) void func_0000946C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a1, %%hi(func_000093E0)\n\t"
        "ori   $a0, $zero, 0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00086574\n\t"
        "addiu $a1, $a1, %%lo(func_000093E0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
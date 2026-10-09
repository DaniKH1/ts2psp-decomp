/**
 * The Sims 2 PSP - func_00110ADC (0x00110ADC, 0x20 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a2, 0x4($a1)
 *     sw    $ra, 0x10($sp)
 *     jal   func_00113458
 *       lw    $a1, 0x0($a1)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Treats $a1 as a two-word pair and unpacks it into the first two
 * arguments of func_00113458: 0x0($a1) becomes $a1 and 4($a1) becomes
 * $a2, so it forwards the pair {lo, hi} as {arg1, arg2}.
 *
 * **The second `lw` is in the delay slot.**  That is load-bearing: it
 * reuses $a1, which is still live, and the `lw $a2` before the call is
 * what keeps the pair from being clobbered.
 */
#include "types.h"

__attribute__((noreturn)) void func_00110ADC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a2, 0x4($a1)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00113458\n\t"
        "lw    $a1, 0x0($a1)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
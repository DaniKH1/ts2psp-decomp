/**
 * The Sims 2 PSP - func_00000B28 (0x00000B28, 0x30 bytes)
 *
 * Computes two products and stores the results as integers to memory.
 *
 *     swc1  $f12, 0x54($a0)
 *     addiu $a1, $a0, 0x60
 *     lwc1  $f13, 0x0($a1)
 *   sym_00000B34:
 *     lwc1  $f14, 0x4($a1)
 *     mul.s $f13, $f13, $f12
 *     addiu $a0, $a0, 0x58
 *     mul.s $f12, $f14, $f12
 *     mfc1  $a1, $f13
 *     sw    $a1, 0x0($a0)
 *     mfc1  $a2, $f12
 *     jr    $ra
 *       sw    $a2, 0x4($a0)
 *
 * This function appears to compute:
 *   *a0 = f12 * f13 (as int)
 *   *(a0+4) = f12 * f14 (as int)
 * Where f12 is pre-loaded in 0x54($a0), and f13/f14 are loaded from a0+0x60.
 *
 * The label `sym_00000B34` marks the second load but is not branched to.
 * The delay slot of `jr $ra` stores the second result.
 */
#include "types.h"

__attribute__((noreturn)) void func_00000B28(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "swc1  $f12, 0x54($a0)\n\t"
        "addiu $a1, $a0, 0x60\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "addiu $a0, $a0, 0x58\n\t"
        "mul.s $f12, $f14, $f12\n\t"
        "mfc1  $a1, $f13\n\t"
        "sw    $a1, 0x0($a0)\n\t"
        "mfc1  $a2, $f12\n\t"
        "jr    $ra\n\t"
        "sw    $a2, 0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
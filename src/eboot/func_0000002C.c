/**
 * The Sims 2 PSP - func_0000002C (0x0000002C, 0x2C bytes)
 *
 * Computes two reciprocal values from a floating-point constant and stores
 * them to global symbols.
 *
 *     lui   $a0, %hi(sym_001D1B10)
 *     lwc1  $f12, %lo(sym_001D1B10)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a0, %hi(sym_001D1B14)
 *     lui   $a1, %hi(sym_001D1B18)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001D1B14)($a0)
 *     jr    $ra
 *     swc1  $f12, %lo(sym_001D1B18)($a1)
 *
 * This is functionally identical to the module entry function `func_00000000`
 * but operates on different symbols (sym_001D1B10/14/18 instead of
 * sym_001D1B00/04/08). The constant 0x43340000 = 180.0f is the same
 * degrees-to-radians conversion factor.
 *
 * The delay slot of `jr $ra` contains the second `swc1`, which stores the
 * reciprocal value. This is the standard CodeWarrior pattern for filling
 * the return delay slot with useful work.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000002C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, %%hi(sym_001D1B10)\n\t"
        "lwc1  $f12, %%lo(sym_001D1B10)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a0, %%hi(sym_001D1B14)\n\t"
        "lui   $a1, %%hi(sym_001D1B18)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001D1B14)($a0)\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, %%lo(sym_001D1B18)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
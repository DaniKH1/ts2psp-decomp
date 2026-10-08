/**
 * The Sims 2 PSP - func_00004F64 (0x00004F64, 0x2C bytes)
 *
 * Computes two reciprocal values from a floating-point constant and stores
 * them to global symbols.
 *
 *     lui   $a0, %hi(sym_001D1BC8)
 *     lwc1  $f12, %lo(sym_001D1BC8)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a0, %hi(sym_001D1BCC)
 *     lui   $a1, %hi(sym_001D1BD0)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001D1BCC)($a0)
 *     jr    $ra
 *       swc1 $f12, %lo(sym_001D1BD0)($a1)
 *
 * Another FP reciprocal cache against 180.0f (0x43340000), identical pattern
 * to func_00000000 and func_0000002C but with different symbols:
 *   sym_001D1BC8 = value
 *   sym_001D1BCC = value / 180.0f
 *   sym_001D1BD0 = 180.0f / value
 */
#include "types.h"

__attribute__((noreturn)) void func_00004F64(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, %%hi(sym_001D1BC8)\n\t"
        "lwc1  $f12, %%lo(sym_001D1BC8)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a0, %%hi(sym_001D1BCC)\n\t"
        "lui   $a1, %%hi(sym_001D1BD0)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001D1BCC)($a0)\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, %%lo(sym_001D1BD0)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
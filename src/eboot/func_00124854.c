/**
 * The Sims 2 PSP - func_00124854 (0x00124854, 0x2C bytes)
 *
 *     lui   $a0, %hi(sym_001DE7A0)
 *     lwc1  $f12, %lo(sym_001DE7A0)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a0, %hi(sym_001DE7A4)
 *     lui   $a1, %hi(sym_001DE7A8)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001DE7A4)($a0)
 *     jr    $ra
 *       swc1 $f12, %lo(sym_001DE7A8)($a1)
 *
 * The same reciprocal-cache body as func_0012323C, on a different set
 * of globals: divisor `sym_001DE7A0`, results `sym_001DE7A4` and
 * `sym_001DE7A8`.
 *
 * **Byte-for-byte the same instruction sequence, differing only in the
 * three symbol addresses.**  With func_0012323C that makes two copies
 * of the 180.0f helper and, counting `func_00000000` and
 * `func_0010260C`, four in the module.  **The repetition is the
 * finding: this is a template the engine instantiates per class**, and
 * the fact that the compiler emitted identical code for each is why the
 * shape survives rather than being shared through a call.
 */
#include "types.h"

__attribute__((noreturn)) void func_00124854(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, %%hi(sym_001DE7A0)\n\t"
        "lwc1  $f12, %%lo(sym_001DE7A0)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a0, %%hi(sym_001DE7A4)\n\t"
        "lui   $a1, %%hi(sym_001DE7A8)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001DE7A4)($a0)\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, %%lo(sym_001DE7A8)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
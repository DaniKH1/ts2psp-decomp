/**
 * The Sims 2 PSP - func_0012323C (0x0012323C, 0x2C bytes)
 *
 *     lui   $a0, %hi(sym_001DE690)
 *     lwc1  $f12, %lo(sym_001DE690)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a0, %hi(sym_001DE694)
 *     lui   $a1, %hi(sym_001DE698)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001DE694)($a0)
 *     jr    $ra
 *       swc1 $f12, %lo(sym_001DE698)($a1)
 *
 * Fills a reciprocal cache: reads a divisor from `sym_001DE690`,
 * stores `divisor / 180.0f` into `sym_001DE694` and
 * `180.0f / divisor` into `sym_001DE698`.
 *
 * **`0x43340000` is exactly 180.0f** - the constant appears as a plain
 * float in the `lui`, and `mtc1` moves the low half into `$f13` after
 * the `ori` that fills its move-delay slot (hence the spurious-looking
 * `nop` the disassembly shows between them).
 *
 * **Both divisions happen before either store.**  `$f12` is still live
 * across the first `div.s`, so the second division reuses it; that is
 * why the two `lui`s sit between the two `div.s` instructions rather
 * than after them.  The second store goes in the delay slot of the
 * `jr`, and the first needs its own instruction because it has no spare
 * slot to fall into.
 *
 * This is the **third** independent copy of the 180.0f idiom after
 * `func_00000000` and `func_0010260C`, and the third set of three
 * consecutive globals - a house helper for a per-class conversion rate,
 * not a coincidence.
 */
#include "types.h"

__attribute__((noreturn)) void func_0012323C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, %%hi(sym_001DE690)\n\t"
        "lwc1  $f12, %%lo(sym_001DE690)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a0, %%hi(sym_001DE694)\n\t"
        "lui   $a1, %%hi(sym_001DE698)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001DE694)($a0)\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, %%lo(sym_001DE698)($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
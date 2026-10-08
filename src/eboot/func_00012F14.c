/**
 * The Sims 2 PSP - func_00012F14 (0x00012F14, 0x28 bytes)
 *
 * Writes a boolean into bit 31 of the word at offset 0x64.
 *
 *     andi  $a1, $a1, 0xFF
 *     lw    $a2, 0x64($a0)
 *     lui   $a3, 0x8000
 *     addiu $a3, $a3, -0x1
 *     andi  $a1, $a1, 0x1
 *     and   $a2, $a2, $a3
 *     sll   $a1, $a1, 31
 *     or    $a1, $a2, $a1
 *     jr    $ra
 *     sw    $a1, 0x64($a0)
 *
 * **`self->word_64 = (self->word_64 & 0x7FFFFFFF) | ((arg & 1) << 31);`**
 *
 * **This is clear-then-set in one function, and neither half is a member of
 * `tools/flag_accessors.py`'s three shapes.**  The tool models `get`, `set` and
 * `clear`, each a single operation; this is two, in the order clear then set, and the
 * census's `combined` path looks only for an accessor that also writes a float.
 * `tools/boolean_shapes.py` counts **10 functions** of this shape - the search is
 * bounded to eight instructions and to before any branch, because "some `or` later in
 * the function" is not a claim in a 372-byte body.
 *
 * **Forty bytes away, `func_00012F3C` reads the same bit of the same word**, and the
 * census finds that one: 0x64, mask 0x80000000, `get`.  **So the module has a getter
 * for this bit and this setter, and the census reports only half of it** - which is
 * exactly the situation its docstring warns about when it says a getter with no
 * setter is evidence the other half was inlined away.  **Here the setter exists and
 * the census cannot see it**, so the pairing that census reads has been half-missing
 * rather than genuinely absent.
 *
 * **The clear mask is 0x7FFFFFFF and is built by negation.**  `lui 0x8000` then
 * `addiu -0x1` gives 0x7FFFFFFF; the alternative, `lui 0x8000` then `ori 0xFFFF`,
 * gives 0xFFFFFFFF, which would be the *set* mask, not the clear one.  So the choice
 * of `addiu -1` over `ori 0xFFFF` is the whole difference between clearing bit 31 and
 * clearing everything - **and the compiler did not mix them up**, which is worth
 * noting because `renderMeshInstances_1060` builds 0xFFFFFFFF the other way.
 *
 * **`andi $a1, $a1, 0xFF` at the top is dead.**  Four instructions later
 * `andi $a1, $a1, 0x1` masks to bit 0, which subsumes it.  **This is a sixth dead
 * mask in this tree and a third kind**: not the `sltiu`-then-`andi` pair of
 * `func_000FBFD4`, and not the all-ones mask of `func_0014EAAC`, but a narrowing to a
 * byte immediately followed by a narrowing to a bit of that byte.  **The source
 * narrowed to a `bool` and the compiler kept the byte narrowing that got there** - or
 * the source narrowed to a `u8` and then to a bool, and the first is redundant
 * because the second already is.  The bytes do not say which, and either way the first
 * instruction cannot affect the result.
 *
 * **`sll 31` is how a boolean reaches the top bit**, and it is a shift rather than a
 * mask because the value is already 0 or 1.  `and $a2, $a2, $a3` writes `$a2` and
 * `or $a1, $a2, $a1` writes a *different* register - the result is assembled in `$a1`
 * because `$a2` holds the cleared word and `$a1` already holds the shifted bit.
 */
#include "types.h"

/** Write the low bit of `$a1` into bit 31 of the word at offset 0x64.
 *  @param self In $a0: the object; +0x64 is read and written.
 *  @param arg  In $a1: any value; only its bit 0 is used. */
__attribute__((noreturn)) void func_00012F14(void *self, long arg) {
    (void)self;
    (void)arg;
    __asm__ __volatile__(
        "andi  $a1, $a1, 0xFF\n\t"
        "lw    $a2, 0x64($a0)\n\t"
        "lui   $a3, 0x8000\n\t"
        "addiu $a3, $a3, -0x1\n\t"
        "andi  $a1, $a1, 0x1\n\t"
        "and   $a2, $a2, $a3\n\t"
        "sll   $a1, $a1, 31\n\t"
        "or    $a1, $a2, $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x64($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
/**
 * The Sims 2 PSP - func_00102280 (0x00102280, 0x30 bytes)
 *
 *     lui   $a0, %hi(sym_001DB12C)
 *     lw    $v0, %lo(sym_001DB12C)($a0)
 *     beqz  $v0, 1f
 *     nop
 *     lui   $a1, %hi(sym_001DB014)
 *     addiu $a1, $a1, %lo(sym_001DB014)
 *     lw    $a1, 0x18($a1)
 *     sra   $a1, $a1, 1
 *     sh    $a1, 0xE($v0)
 *     lw    $v0, %lo(sym_001DB12C)($a0)
 *   1:
 *     jr    $ra
 *     nop
 *
 * **If the global render object exists, write half of one of its fields into
 * offset `0xE` of it, and hand the caller that object.**
 *
 *     if (g_render) g_render->half_0xE = (s16)(g_shared->field_0x18 >> 1);
 *     return g_render;
 *
 * ## The `>> 1` is the whole point, and the type is a guess that matters
 *
 * `sra $a1, $a1, 1` is an arithmetic shift, so this is a **signed** halving, and
 * it is then truncated to sixteen bits by `sh`.  Three consequences, all of
 * which the instruction sequence fixes and none of which the names do:
 *
 *  - the value is halved as a signed 32-bit quantity and *then* narrowed, so a
 *    field of `0xFFFF` would store `0x7FFF` as `s16` and not `-1`;
 *  - `>> 1` is not `/ 2` for negative odd inputs - `-3 >> 1` is `-2`, not `-1`;
 *  - odd values round toward negative infinity, not toward zero.
 *
 * **A half-width or half-resolution field is the obvious reading of a value
 * halved on the way into a `sh` at offset `0xE`, and it is not established
 * here.**  What is established is that the halving is arithmetic and that the
 * result is a signed 16-bit truncation, so any name chosen for this has to
 * preserve both.
 *
 * ## Why `$v0` is loaded twice
 *
 * The second `lw $v0, %lo(sym_001DB12C)($a0)` after the store is not redundant
 * work - `$v0` is the object and the `sh` does not change it, so the value is
 * still correct.  **It is there so that the returned pointer comes from the
 * global rather than from the value tested**, which matters only if something
 * in between could have changed the global.  Nothing here can: the body between
 * the two loads is three integer instructions with no call.  So the reload is
 * either defensive, or generated from source that re-read the pointer, and
 * **the bytes do not distinguish those.**
 *
 * `func_001022F0` is the setter for this same global from the other side: it
 * writes `$v0` from a call to `func_0010215C` rather than from a shift.
 */
#include "types.h"

/** Store half of `sym_001DB014`'s field `0x18` at offset `0xE` of the object at
 *  `sym_001DB12C`, if that object exists, and return the object. */
__attribute__((noreturn)) void func_00102280(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, %%hi(sym_001DB12C)\n\t"
        "lw    $v0, %%lo(sym_001DB12C)($a0)\n\t"
        "beqz  $v0, 1f\n\t"
        "nop\n\t"
        "lui   $a1, %%hi(sym_001DB014)\n\t"
        "addiu $a1, $a1, %%lo(sym_001DB014)\n\t"
        "lw    $a1, 0x18($a1)\n\t"
        "sra   $a1, $a1, 1\n\t"
        "sh    $a1, 0xE($v0)\n\t"
        "lw    $v0, %%lo(sym_001DB12C)($a0)\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_0012DE70 (0x0012DE70, 0x34 bytes)
 *
 * `func_0012DE0C` with the fourth lane loaded rather than set to one.
 *
 *     lv.s   S100, 0x0($a1)
 *     lv.s   S110, 0x4($a1)
 *     lv.s   S120, 0x8($a1)
 *     lv.s   S130, 0xC($a1)
 *     lv.q   R200, 0x0($a0)
 *     lv.q   R201, 0x10($a0)
 *     lv.q   R202, 0x20($a0)
 *     lv.q   R203, 0x30($a0)
 *     vtfm4.q R000, E200, R100
 *     svr.q  R000, 0x0($a2)
 *     svl.q  R000, 0xC($a2)
 *     jr     $ra
 *     nop
 *
 * **`out = matrix * (v.x, v.y, v.z, v.w)`**  - a vector transform, against a matrix read
 * from `$a0`.
 *
 * **This and `func_0012DE0C` are the same thirteen instructions apart**, and the two
 * differences are the whole point of the pair:
 *
 *     func_0012DE0C   viim.s S130, 1            vtfm4.q R000, M200, R100
 *     func_0012DE70   lv.s   S130, 0xC($a1)     vtfm4.q R000, E200, R100
 *
 * One supplies the fourth lane as the constant 1 and the other as the source's fourth
 * float, **which is the difference between transforming a point and transforming a
 * direction** - the usual reason a homogeneous matrix API needs two entry points.  And
 * the matrix operand is spelled `M200` here and `M200` in the other but `E200` here, so
 * **the two spellings of the operand change together with the fourth lane.**
 *
 * **Neither name's `M` or `E` is explained by this project, and the pairing is the
 * evidence rather than the explanation**: the module uses both on the same instruction,
 * differing exactly where the source differs, so the spelling follows the *kind* of
 * transform rather than the operand.  What `M` and `E` mean to the assembler - a matrix
 * operand versus an explicit column list, say - is not established here, and saying so
 * is more useful than picking one of the two that sounds plausible.
 *
 * **This is also the function that settles a pattern.**  In the module's shortest vector
 * functions the number of `lv.s` matches the element format of the operation consuming
 * them: three for `.t` forms and four for `.q`.  `func_0012DE0C` and `func_0012DDD8`
 * were the two that broke it with three loads and a `.q` operation, **and here is the
 * fourth load that makes the rule work for `func_0012DE70` - plus the `viim.s` that
 * accounts for the sibling.**
 *
 * The store is the staggered `svr.q`/`svl.q` pair at encoded immediates 3 and 13, the
 * same layout as five other functions in the module.
 * `tools/vfpu_split_store.py` finds all of them at those two offsets, so which half
 * each store writes is not established from the module and this file does not guess.
 */
#include "types.h"

/** Write `matrix * v` to `$a2`, all four components read from `$a1`.
 *  @param m   In $a0: a 4x4 matrix, sixteen bytes per row.
 *  @param v   In $a1: four floats.
 *  @param out In $a2: receives the transformed vector, through a staggered store pair. */
__attribute__((noreturn)) void func_0012DE70(void *m, void *v, void *out) {
    (void)m;
    (void)v;
    (void)out;
    __asm__ __volatile__(
        "lv.s   S100, 0x0($a1)\n\t"
        "lv.s   S110, 0x4($a1)\n\t"
        "lv.s   S120, 0x8($a1)\n\t"
        "lv.s   S130, 0xC($a1)\n\t"
        "lv.q   R200, 0x0($a0)\n\t"
        "lv.q   R201, 0x10($a0)\n\t"
        "lv.q   R202, 0x20($a0)\n\t"
        "lv.q   R203, 0x30($a0)\n\t"
        "vtfm4.q R000, E200, R100\n\t"
        "svr.q  R000, 0x0($a2)\n\t"
        "svl.q  R000, 0xC($a2)\n\t"
        ".set noreorder\n\t"
        "jr     $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
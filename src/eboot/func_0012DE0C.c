/**
 * The Sims 2 PSP - func_0012DE0C (0x0012DE0C, 0x34 bytes)
 *
 * Transforms a three-float vector by a 4x4 matrix, with w set to one.
 *
 *     lv.s   S100, 0x0($a1)
 *     lv.s   S110, 0x4($a1)
 *     lv.s   S120, 0x8($a1)
 *     viim.s S130, 1
 *     lv.q   R200, 0x0($a0)
 *     lv.q   R201, 0x10($a0)
 *     lv.q   R202, 0x20($a0)
 *     lv.q   R203, 0x30($a0)
 *     vtfm4.q R000, M200, R100
 *     svr.q  R000, 0x0($a2)
 *     svl.q  R000, 0xC($a2)
 *     jr     $ra
 *     nop
 *
 * **`out = matrix * (v.x, v.y, v.z, 1.0)`**  - a point transform, against a matrix read
 * from `$a0`.
 *
 * **`viim.s S130, 1` is the homogeneous coordinate**, and it is the fourth of the four
 * lanes: the three `lv.s` fill lanes 0, 1 and 2 of C100 and the fourth comes from an
 * immediate.  **So the odd thing about three `lv.s` feeding a `.q` instruction is not an
 * omission - the fourth lane is supplied by a different instruction.**
 *
 * That makes this and `func_0012DE70` the pair that settles the question, and they are
 * **the same thirteen instructions apart**:
 *
 *     func_0012DE0C   viim.s S130, 1            vtfm4.q R000, M200, R100
 *     func_0012DE70   lv.s   S130, 0xC($a1)     vtfm4.q R000, E200, R100
 *
 * One takes the fourth lane from a constant and the other from the source's fourth
 * float - **a point transform against a direction transform**, which is exactly the
 * distinction between multiplying by a position and multiplying by a vector.  **And the
 * matrix operand is spelled differently too, `M200` against `E200`.**  Same length, same
 * stores, same everything else.
 *
 * **Across the module's shortest vector functions the number of `lv.s` matches the
 * element format of the operation that consumes them**: three for the `.t` forms
 * (`func_0012DD94`'s `vmul.t`, `func_0012DE40`'s `vtfm3.t`, `syncSkeleton_27D0`'s
 * `vscl.t`, `syncSkeleton_2808`'s `vmul.t`) and four for the `.q` forms
 * (`syncSkeleton_22B8`, `func_0012A9D0`, `syncSkeleton_21A0`, `func_0012DE70`).  **This
 * function and `func_0012DDD8` are the only short ones that break the pattern, and both
 * supply the fourth lane separately - here from `viim.s`, and `func_0012DDD8` from the
 * register it had already loaded.**  The pattern holds where it was checked and the
 * exceptions are accounted for rather than waved at.
 *
 * The store is the staggered `svr.q`/`svl.q` pair at encoded immediates 3 and 13, the
 * same layout as `syncSkeleton_22B8` and `func_0012A9D0`.  **`tools/vfpu_split_store.py`
 * finds five instances of that pair in the module and every one uses those two offsets,
 * so which half each store writes is not established here either** - this pair would add
 * two more instances at the same offsets, not new information.
 *
 * **`viim.s` appears four times in the whole module**: here and in
 * `renderMeshInstances_1034` / `renderMeshInstances_1060`, where the immediate is 32
 * rather than 1.  So the instruction is used both for a homogeneous coordinate and for a
 * fixed-point scale factor, and **the two uses share nothing but the instruction**.
 */
#include "types.h"

/** Write `matrix * (v, 1.0)` to `$a2`.
 *  @param m   In $a0: a 4x4 matrix, sixteen bytes per row.
 *  @param v   In $a1: three floats.
 *  @param out In $a2: receives the transformed vector, through a staggered store pair. */
__attribute__((noreturn)) void func_0012DE0C(void *m, void *v, void *out) {
    (void)m;
    (void)v;
    (void)out;
    __asm__ __volatile__(
        "lv.s   S100, 0x0($a1)\n\t"
        "lv.s   S110, 0x4($a1)\n\t"
        "lv.s   S120, 0x8($a1)\n\t"
        "viim.s S130, 1\n\t"
        "lv.q   R200, 0x0($a0)\n\t"
        "lv.q   R201, 0x10($a0)\n\t"
        "lv.q   R202, 0x20($a0)\n\t"
        "lv.q   R203, 0x30($a0)\n\t"
        "vtfm4.q R000, M200, R100\n\t"
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
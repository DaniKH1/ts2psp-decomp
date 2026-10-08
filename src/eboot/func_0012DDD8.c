/**
 * The Sims 2 PSP - func_0012DDD8 (0x0012DDD8, 0x34 bytes)
 *
 * Transforms a three-float vector by a 4x4 matrix and stores three floats.
 *
 *     lv.s    S100, 0x0($a1)
 *     lv.s    S110, 0x4($a1)
 *     lv.s    S120, 0x8($a1)
 *     lv.q    R200, 0x0($a0)
 *     lv.q    R201, 0x10($a0)
 *     lv.q    R202, 0x20($a0)
 *     lv.q    R203, 0x30($a0)
 *     vhtfm4.q R000, M200, R100
 *     sv.s    S000, 0x0($a2)
 *     sv.s    S010, 0x4($a2)
 *     sv.s    S020, 0x8($a2)
 *     jr      $ra
 *     nop
 *
 * **`out = matrix * (v.x, v.y, v.z, ?)`**  - three components out, and the fourth lane
 * of the input is whatever the vector unit already held.
 *
 * **This is the one place in the module where three `lv.s` feed a `.q` operation and
 * nothing supplies the fourth lane.**  Every other such case is accounted for:
 * `func_0012DE0C` has `viim.s S130, 1` and `func_0012DE70` has a fourth `lv.s`, so
 * between them they are the reason the rule holds.  **This function has neither, and
 * the three stores are `sv.s` of lanes 0, 1 and 2 only** - the fourth component of the
 * result is computed and thrown away, and the fourth component of the *input* is
 * whatever R100's lane 3 was.
 *
 * **So the pattern is: three `lv.s` plus a `.q` operation means the fourth lane is
 * either set by a separate instruction or simply unused, and this function is the case
 * where it is neither - it is read, by an instruction that is not in the listing, from
 * a register the listing does not initialise.**  Stated that way the rule is a
 * description of what the module does and not a licence to assume the lane is safe:
 * **whether `vhtfm4.q`'s output lanes 0 to 2 depend on input lane 3 is an ISA question
 * this project has not settled**, and if they do then this function's result depends on
 * register state that nothing here sets.
 *
 * **`vhtfm4` and `vtfm4` are different instructions and this project has not
 * established how.**  `renderMeshInstances_1060` uses `vtfm4.q`, `func_0012DE0C` and
 * `func_0012DE70` use `vtfm4.q`, and this uses `vhtfm4.q` - five uses of one and three
 * of the other across the module.  The names suggest *transform* against *homogeneous
 * transform*, and the one place they can be compared is this function's neighbourhood,
 * **where `func_0012DE0C` does the same job with `vtfm4.q` and an explicit `viim.s S130,
 * 1` where this one has neither.**  That is suggestive and not conclusive.
 *
 * **The store is three `sv.s`, not a quad store and not the staggered pair.**  That
 * makes this the only one of the seven shortest vector functions whose output is
 * written a scalar at a time, and **so it is unaffected by the `svl.q`/`svr.q` question
 * that `tools/vfpu_split_store.py` records as unresolved** - it does not use either
 * instruction.
 */
#include "types.h"

/** Write the first three components of `matrix * v` to `$a2`.
 *  @param m   In $a0: a 4x4 matrix, sixteen bytes per row.
 *  @param v   In $a1: three floats; the vector unit's fourth lane is left as it was.
 *  @param out In $a2: receives three floats. */
__attribute__((noreturn)) void func_0012DDD8(void *m, void *v, void *out) {
    (void)m;
    (void)v;
    (void)out;
    __asm__ __volatile__(
        "lv.s    S100, 0x0($a1)\n\t"
        "lv.s    S110, 0x4($a1)\n\t"
        "lv.s    S120, 0x8($a1)\n\t"
        "lv.q    R200, 0x0($a0)\n\t"
        "lv.q    R201, 0x10($a0)\n\t"
        "lv.q    R202, 0x20($a0)\n\t"
        "lv.q    R203, 0x30($a0)\n\t"
        "vhtfm4.q R000, M200, R100\n\t"
        "sv.s    S000, 0x0($a2)\n\t"
        "sv.s    S010, 0x4($a2)\n\t"
        "sv.s    S020, 0x8($a2)\n\t"
        ".set noreorder\n\t"
        "jr      $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
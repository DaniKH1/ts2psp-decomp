/**
 * The Sims 2 PSP - func_0012DE40 (0x0012DE40, 0x30 bytes)
 *
 * Transforms a three-float vector by a 3x3 matrix and stores three floats.
 *
 *     lv.s    S100, 0x0($a1)
 *     lv.s    S110, 0x4($a1)
 *     lv.s    S120, 0x8($a1)
 *     lv.q    R200, 0x0($a0)
 *     lv.q    R201, 0x10($a0)
 *     lv.q    R202, 0x20($a0)
 *     vtfm3.t R000, M200, R100
 *     sv.s    S000, 0x0($a2)
 *     sv.s    S010, 0x4($a2)
 *     sv.s    S020, 0x8($a2)
 *     jr      $ra
 *     nop
 *
 * **`out = matrix3x3 * v`**  - a plain vector-by-matrix product, no fourth lane
 * anywhere.
 *
 * **This is `func_0012DDD8` with one fewer matrix row and a `.t` operation instead of a
 * `.q` one.**  Both load three scalars, both store three scalars, both transform; the
 * differences are the size of the matrix and the element format, **and the two together
 * are what make the module's rule checkable**:
 *
 *     func_0012DE40   3 x lv.s, 3 x lv.q, vtfm3.t    three floats, three floats
 *     func_0012DDD8   3 x lv.s, 4 x lv.q, vhtfm4.q    three floats, three floats
 *
 * **Three `lv.s` for the `.t` operation is exactly right, because `vtfm3.t` reads three
 * components per quad** - so unlike `func_0012DDD8`, this function's fourth lane is
 * never read at all and leaving it stale is correct rather than a hazard.  That is the
 * difference the two functions make between "the pattern holds" and "the pattern holds
 * by luck".
 *
 * **A 3x3 matrix is twelve floats, which is three quads** - and the last quad is one
 * float short of full, since twelve is not a multiple of four.  **So R202's lane 3 holds
 * whatever was there, and `sv.s` stores only lanes 0, 1 and 2 of the result**, so the
 * short quad is never written to memory.  Loading it with a full `lv.q` is therefore one
 * instruction more than strictly needed, and the module does it anyway - the same
 * "loads a quad where three would do" habit as `func_0012D9F8`'s sixteen `lv.s`.
 *
 * **`vtfm3.t` is the only triple-transform in the module**, against five `vtfm4.q` and
 * three `vhtfm4.q`, so this is the one place the triple form can be compared with the
 * quad forms at all.  **The comparison says the triple form is used exactly when the
 * matrix is 3x3, and this project has not established why** - a 3x3 in three quads is
 * not a whole number of quads, so the argument would be that the triple format saves
 * having to deal with the remainder.  **That argument is plausible and unsupported, and
 * the honest position is that one instance does not make a rule.**
 *
 * **The output is written a scalar at a time**, like `func_0012DDD8`, so this function is
 * likewise unaffected by the `svl.q`/`svr.q` question that
 * `tools/vfpu_split_store.py` records as unresolved.
 */
#include "types.h"

/** Write `matrix3x3 * v` to `$a2`.
 *  @param m   In $a0: a 3x3 matrix, sixteen bytes per row, three rows.
 *  @param v   In $a1: three floats.
 *  @param out In $a2: receives three floats. */
__attribute__((noreturn)) void func_0012DE40(void *m, void *v, void *out) {
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
        "vtfm3.t R000, M200, R100\n\t"
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
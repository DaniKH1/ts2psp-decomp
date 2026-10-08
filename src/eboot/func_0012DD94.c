/**
 * The Sims 2 PSP - func_0012DD94 (0x0012DD94, 0x44 bytes)
 *
 * Scales a 4x4 matrix in place by a three-float vector.
 *
 *     lv.s   S100, 0x0($a1)
 *     lv.s   S110, 0x4($a1)
 *     lv.s   S120, 0x8($a1)
 *     lv.q   R200, 0x0($a0)
 *     lv.q   R201, 0x10($a0)
 *     lv.q   R202, 0x20($a0)
 *     lv.q   R203, 0x30($a0)
 *     vmul.t R200, R200, R100
 *     vmul.t R201, R201, R100
 *     vmul.t R202, R202, R100
 *     vmul.t R203, R203, R100
 *     sv.q   R200, 0x0($a0)
 *     sv.q   R201, 0x10($a0)
 *     sv.q   R202, 0x20($a0)
 *     sv.q   R203, 0x30($a0)
 *     jr     $ra
 *     nop
 *
 * **`m[i] *= v` for every row - each row's first three components scaled, the fourth
 * untouched - and written back over the original.**
 *
 * **`.t` is the triple format, and it is why three `lv.s` is the right number here.**
 * `vmul.t` reads three components per quad, so filling lanes 0, 1 and 2 of C100 is
 * exactly enough; **the fourth lane is never read, so leaving it whatever it held is
 * correct rather than sloppy.**  That is the rule this function exemplifies, and the
 * exceptions to it - `func_0012DDD8` and `func_0012DE0C`, three `lv.s` feeding a `.q`
 * operation - are accounted for by a separate `viim.s S130, 1` supplying the fourth
 * lane.
 *
 * **So this function scales three columns, not four.**  With a triple multiply on a
 * 4x4 laid out as four quads, each row's x, y and z are scaled and its fourth component -
 * the one that would be `w` in a homogeneous matrix, or a translation in an affine one -
 * is passed through unchanged.  **Whether `vmul.t` leaves lane 3 alone or produces
 * something unspecified in it is an ISA question this project has not settled**, and it
 * matters here: the four `sv.q` stores write all four components back.  **What the bytes
 * do establish is that the store is a full quad store, so whatever is in lane 3 after
 * the multiply is written to memory**, which is the same class of question as
 * `svl.q`'s halves and is recorded here rather than assumed away.
 *
 * **The load and the store use the same four addresses, so the whole function is
 * in-place**, and there is no reload between them - the quads stay in the vector unit's
 * registers across all four multiplies.  **A 4x4 matrix is exactly sixteen floats, which
 * is exactly four quads, so the entire matrix fits in the vector unit at once** - and
 * that is why the function needs no scratch memory and no frame, unlike the scalar
 * version of the same idea, which is `func_000C3470` and its 0xC0 bytes of stack.
 *
 * **The two spellings of the same copy appear here and in `func_0012DE70`.**  This
 * function loads the matrix with `lv.q` and the vector with `lv.s`; that one loads the
 * matrix with `lv.q` and the vector with four `lv.s`.  **The matrix is always `lv.q` and
 * the vector is `lv.s`, in all six of the shortest functions** - a 4x4 is naturally a
 * quad and a 3-vector is naturally three scalars, and the module does not try to load a
 * triple with `lv.q` and ignore a lane.
 */
#include "types.h"

/** Scale each row of the 4x4 at `$a0` by the three floats at `$a1`, in place.
 *  @param m In $a0: a 4x4 matrix, sixteen bytes per row, read and written.
 *  @param v In $a1: three floats. */
__attribute__((noreturn)) void func_0012DD94(void *m, void *v) {
    (void)m;
    (void)v;
    __asm__ __volatile__(
        "lv.s   S100, 0x0($a1)\n\t"
        "lv.s   S110, 0x4($a1)\n\t"
        "lv.s   S120, 0x8($a1)\n\t"
        "lv.q   R200, 0x0($a0)\n\t"
        "lv.q   R201, 0x10($a0)\n\t"
        "lv.q   R202, 0x20($a0)\n\t"
        "lv.q   R203, 0x30($a0)\n\t"
        "vmul.t R200, R200, R100\n\t"
        "vmul.t R201, R201, R100\n\t"
        "vmul.t R202, R202, R100\n\t"
        "vmul.t R203, R203, R100\n\t"
        "sv.q   R200, 0x0($a0)\n\t"
        "sv.q   R201, 0x10($a0)\n\t"
        "sv.q   R202, 0x20($a0)\n\t"
        "sv.q   R203, 0x30($a0)\n\t"
        ".set noreorder\n\t"
        "jr     $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
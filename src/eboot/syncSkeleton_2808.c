/**
 * The Sims 2 PSP - syncSkeleton_2808 (0x001BB224, 0x38 bytes)
 *
 * Multiplies the three rows of a 3x4 matrix by a vector already in `$vf10`.
 *
 *     lv.s  S100, 0x0($a1)        three scalars, loaded and then unused
 *     lv.s  S110, 0x4($a1)
 *     lv.s  S120, 0x8($a1)
 *     lv.q  R200, 0x0($a0)        the matrix, 16 bytes per row
 *     lv.q  R201, 0x10($a0)
 *     lv.q  R202, 0x20($a0)
 *     vmul.t R200, R200, R100     multiply by the vector in R100
 *     vmul.t R201, R201, R100
 *     vmul.t R202, R202, R100
 *     sv.q  R200, 0x0($a0)
 *     sv.q  R201, 0x10($a0)
 *     sv.q  R202, 0x20($a0)
 *     jr    $ra
 *     nop
 *
 * **The three `lv.s` results are never used.**  They load `$vfs0`, `$vfs1` and
 * `$vfs2` and nothing reads them: the multiplications all take `R100` as the
 * right-hand operand.  So either the original had a fourth instruction that was
 * optimised into one of these, or - more likely for a function of this shape -
 * the loads are the tail of an expression whose other half reads a vector register
 * the compiler could not see.
 *
 * The consequence is that **this function is not self-contained.**  `$vf10` holds
 * the operand and nothing here sets it, so the caller must have left the right
 * vector there.  With `.t` on every operation, the value is read from the vector
 * unit's *transpose* file rather than from the data file, which is what lets three
 * separate `vmul.t` share one operand: it was transposed once, by whatever wrote it.
 *
 * That is the pair with `syncSkeleton_27D0`: that one broadcasts three scalars with
 * `vscl.t` and is self-contained, this one consumes a prepared vector and is not.
 * One is a skinning step with weights from memory; the other is a matrix product
 * with its right-hand factor already in place.  Same fourteen instructions, same
 * register layout, different contract - which is why they sit next to each other in
 * the link order.
 *
 * The three dead loads are transcribed rather than dropped.  Removing them would
 * give the same behaviour and the wrong bytes, and it would hide the fact that the
 * original's argument `$a1` is not used by the code that matters.
 */
#include "types.h"

/* A 3x4 matrix of floats, 16 bytes per row - one quad each. */
typedef struct Mat34 {
    float row[3][4];
} Mat34;

typedef struct Scale {
    float x;   /* 0x0 - loaded, and unused */
    float y;   /* 0x4 - loaded, and unused */
    float z;   /* 0x8 - loaded, and unused */
} Scale;

/* `noreturn` is a lie, as in func_000A9A00 and syncSkeleton_27D0: the block below
 * contains the `jr $ra`, and without the attribute gcc appends an epilogue of its
 * own, which puts the last store after the return. */
__attribute__((noreturn)) void syncSkeleton_2808(Mat34 *matrix,
                                                 const Scale *scale) {
    (void)matrix;
    (void)scale;

    __asm__ __volatile__(
        "lv.s  S100, 0x0($a1)\n\t"
        "lv.s  S110, 0x4($a1)\n\t"
        "lv.s  S120, 0x8($a1)\n\t"
        "lv.q  R200, 0x0($a0)\n\t"
        "lv.q  R201, 0x10($a0)\n\t"
        "lv.q  R202, 0x20($a0)\n\t"
        "vmul.t R200, R200, R100\n\t"
        "vmul.t R201, R201, R100\n\t"
        "vmul.t R202, R202, R100\n\t"
        "sv.q  R200, 0x0($a0)\n\t"
        "sv.q  R201, 0x10($a0)\n\t"
        "sv.q  R202, 0x20($a0)\n\t"
        /* `noreorder`, or gas fills the delay slot by moving the preceding store
         * across the branch.  And an explicit `nop`, because `noreorder` also stops
         * it inserting one - under `reorder` the slot must not be written out, and
         * under `noreorder` it must be.  Opposite rules, four bytes apart. */
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
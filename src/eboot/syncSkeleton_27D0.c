/**
 * The Sims 2 PSP - syncSkeleton_27D0 (0x001BB1EC, 0x38 bytes)
 *
 * Scales the three rows of a 3x4 matrix by three per-axis factors.
 *
 *     lv.s  $vfs0,  0x0($a1)        three scalars, one per axis
 *     lv.s  $vfs1,  0x4($a1)
 *     lv.s  $vfs2,  0x8($a1)
 *     lv.q  $vf20,  0x0($a0)        three quads: the matrix, 16 bytes per row
 *     lv.q  $vf21,  0x10($a0)
 *     lv.q  $vf22,  0x20($a0)
 *     vscl.t $vf20, $vf20, $vfs0    scale row 0 by the x factor
 *     vscl.t $vf21, $vf21, $vfs1
 *     vscl.t $vf22, $vf22, $vfs2
 *     sv.q  $vf20,  0x0($a0)
 *     sv.q  $vf21,  0x10($a0)
 *     sv.q  $vf22,  0x20($a0)
 *     jr    $ra
 *     nop
 *
 * **This is the first code in the project that uses the PSP's vector unit**, and it
 * is the clearest statement yet of what the vector unit was put in the console for:
 * skinning.  Three quads of four floats is exactly a 3x4 matrix, the three scalars are
 * bone weights, and `vscl.t` multiplies a quad by a scalar broadcast from a vector
 * register.  One instruction does four multiplies, so a whole matrix is scaled in
 * three.
 *
 * The `.t` suffix is a transpose, not a scale - the value is broadcast from the
 * single-precision view into all four lanes of the quad.  That is what makes
 * `vscl` "scale" rather than "multiply": `$vfs0` holds one float and the operation
 * multiplies every lane of `$vf20` by it.
 *
 * `syncSkeleton_2808`, immediately after, is the same shape with `vmul.t` against
 * `$vf10` instead of `vscl.t` against `$vfs0` - a matrix-matrix product where the
 * right-hand factor lives in the vector registers rather than in memory.  So this
 * pair is a scaling and a concatenation, and the name is accurate: skeleton
 * transforms are being composed on the vector unit, not on the scalar one.
 *
 * **Why this is asm and not C.**  psp-gcc has no vector types on Allegrex - there is
 * no `float4`, and no operator that lowers to `lv.q` or `vscl.t`.  There is no C
 * spelling of this function at all, so unlike the other files in this directory the
 * body here *is* the machine code, and the comment above is the decompilation.  That
 * is a deliberate departure from the rule the rest of the set follows, and it is
 * recorded rather than disguised: the alternative would be to leave a function that
 * is now understood untranscribed, which would be worse.
 */
#include "types.h"

/* A 3x4 matrix of floats, 16 bytes per row - one quad each. */
typedef struct Mat34 {
    float row[3][4];
} Mat34;

/* The three per-axis factors, one float each - loaded as scalars, not as a quad,
 * because `vscl` broadcasts them. */
typedef struct Scale {
    float x;   /* 0x0 */
    float y;   /* 0x4 */
    float z;   /* 0x8 */
} Scale;

/* `noreturn` is a lie and is here for the same reason as in func_000A9A00: the
 * block below contains the `jr $ra`, and without the attribute gcc appends an
 * epilogue of its own, which pushes the last `sv.q` past the return and makes the
 * function four bytes too long.  Telling the compiler not to generate a return,
 * when the return is already written, is the only way to keep that store in the
 * delay slot where it belongs. */
__attribute__((noreturn)) void syncSkeleton_27D0(Mat34 *matrix,
                                                 const Scale *scale) {
    (void)matrix;
    (void)scale;

    /* Everything is in the block, including the return: the vector loads and stores
     * are the whole function and there is nothing left for the compiler to do.  The
     * delay slot is left to the assembler - writing `nop` there as well would make
     * the symbol one instruction too long, the same result as func_000A9A00.
     *
     * The register names are spimdisasm's, not gcc's.  `$vfs0`, `$vf20`, `vfs0`
     * and `$vfs00` are all rejected as `invalid operands`; `S100` and `R200` parse,
     * with errors only when the *class* is wrong - `lv.s` needs a single register and
     * `lv.q`/`sv.q`/`vscl.t` need a quad one.  So the rule is: use the disassembler's
     * spelling and match the class to the mnemonic.
     *
     * No clobbers at all, vector registers included: gcc rejects `"$vfs0"` as a
     * clobber name too, and there is no way to name the vector file to it.  That is
     * safe here only because the block is the entire function, so nothing is live
     * anywhere. */
    __asm__ __volatile__(
        "lv.s  S100, 0x0($a1)\n\t"
        "lv.s  S110, 0x4($a1)\n\t"
        "lv.s  S120, 0x8($a1)\n\t"
        "lv.q  R200, 0x0($a0)\n\t"
        "lv.q  R201, 0x10($a0)\n\t"
        "lv.q  R202, 0x20($a0)\n\t"
        "vscl.t R200, R200, S100\n\t"
        "vscl.t R201, R201, S110\n\t"
        "vscl.t R202, R202, S120\n\t"
        "sv.q  R200, 0x0($a0)\n\t"
        "sv.q  R201, 0x10($a0)\n\t"
        "sv.q  R202, 0x20($a0)\n\t"
        /* Without this the assembler fills the return's delay slot by moving the
         * preceding `sv.q` across the branch, which is what a delay-slot scheduler
         * exists to do and which is exactly wrong here: the original's slot is a
         * `nop`.  Left on, gas emits `jr $ra` and then `sv.q`, and the function comes
         * out 52 bytes instead of 56. */
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        /* Explicit, because `noreorder` also stops gas *inserting* one: with the
         * slot left empty and nothing after the block to be attributed to it - the
         * `noreturn` again - the symbol comes out 52 bytes and the whole function
         * shifts the next one along.  So under `noreorder` the slot must be written
         * out, and under `reorder` it must not be; the two cases are opposites and
         * getting them the wrong way round costs four bytes either time. */
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
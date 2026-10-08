/**
 * The Sims 2 PSP - func_00000000 (0x00000000, 0x2C bytes)
 *
 * The very first function of the module.  It bootstraps the two reciprocals
 * the rest of the engine works with, against a scale factor of 180.0f.
 *
 *   [0x001D1B00]  the value
 *   [0x001D1B04]  value / 180.0
 *   [0x001D1B08]  180.0 / value
 *
 * Both divisions are kept - rather than one of them becoming a multiply by the
 * exact reciprocal - and the scale is materialised with `lui` + `mtc1`
 * into `$f13` before either division runs.  That idiom is CodeWarrior's, and
 * getting psp-gcc to emit it is what this function establishes: see the float
 * section of progress.md.
 *
 * ## The scale is 180.0f, and this file used to say 2^28
 *
 * An earlier version of this comment called `0x43340000` "the 2^28 scale of
 * the engine's fixed point world".  **That was wrong.**  The float bit pattern
 * `0x43340000` is exactly 180.0f - exponent 0x86, mantissa 0.40625 - and 2^28
 * as a float is `0x4D800000`, a different constant entirely.
 *
 * **The instructions were never in question, only the reading of them**: this
 * file has always compiled to the original 0x2C bytes, and it still does.  What
 * was wrong was the interpretation, and it was wrong in a way that would have
 * propagated - a "2^28 fixed point world" is a plausible-sounding story about
 * this engine and nothing here supports it.
 *
 * 180 is the degrees in a half turn, so **the two reciprocals are most likely
 * an angle conversion** - degrees to radians and back - cached at startup so the
 * rest of the engine multiplies instead of dividing.  **That is a reading, not
 * a finding**: 180.0 is also a perfectly ordinary scale factor in other units,
 * and this file does not have the consumers of `sym_001D1B04`/`sym_001D1B08`
 * in front of it.  `func_0010260C` divides by the same 180.0f, which is one
 * data point and not a proof.
 *
 * psp-gcc loads float constants with `lwc1` out of `.rodata` and uses `$f0`-`$f2`
 * for temporaries, where CodeWarrior only ever starts temporaries at `$f12`.
 * Both are pinned here, and the two global addresses need two separate integer
 * registers because the original interleaves their `lui`s with the divisions.
 */
#include "types.h"

extern f32 sym_001D1B00;
extern f32 sym_001D1B04;
extern f32 sym_001D1B08;

/* 180.0f - degrees in a half turn, and the factor both reciprocals are built
 * against.  Its float bit pattern is 0x43340000, so `lui` needs only the high
 * half.  Not 2^28: that is 0x4D800000, and this constant was once misnamed as
 * such. */
#define SCALE_180_HI 0x4334

void func_00000000(void) {
    /* The last `swc1` belongs in `jr $ra`'s delay slot, which GCC will not
     * generate for a function whose result is void - it emits `nop` there and
     * then its own return.  Returning a value makes GCC schedule the asm into
     * the delay slot instead, because it then has a reason to put the last
     * write on the return path.  The value is discarded by the caller, so this
     * does not change the interface. */
    register f32 unused;
    register f32 scale asm("$f13");
    register f32 value asm("$f12");
    register f32 quotient asm("$f14");
    register u32 addr asm("$a0");
    register u32 addr2 asm("$a1");

    /* `lui` + `mtc1` rather than a `lwc1` from .rodata: the constant never
     * reaches memory, which is the CodeWarrior idiom for float literals.
     * `%%hi`/`%%lo` rather than `%hi`/`%lo`: in GCC inline asm a literal
     * percent is written `%%`.
     *
     * `.set noreorder` is what makes this work, and it is scoped to just the
     * one hazard.  The `mtc1` at 0x08 writes an FPU register and the `div.s` at
     * 0x10 reads it one instruction later, so the assembler believes a barrier
     * is needed and inserts a `nop`.  CodeWarrior does not, and the original has
     * no gap - the Allegrex `mtc1` result is available immediately, so the two
     * really are back to back.  Leaving `.set noreorder` in place for the rest
     * would cost GCC the delay slot of the return, which is where the final
     * `swc1` has to land, so it is turned back on as early as possible. */
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui    %[a0], %%hi(sym_001D1B00)\n\t"
        "lwc1   %[f12], %%lo(sym_001D1B00)(%[a0])\n\t"
        "lui    %[a0], %[scale_hi]\n\t"
        "mtc1   %[a0], %[f13]\n\t"
        "div.s  %[f14], %[f12], %[f13]\n\t"
        ".set reorder\n\t"
        "lui    %[a0], %%hi(sym_001D1B04)\n\t"
        "lui    %[a1], %%hi(sym_001D1B08)\n\t"
        "div.s  %[f12], %[f13], %[f12]\n\t"
        "swc1   %[f14], %%lo(sym_001D1B04)(%[a0])\n\t"
        "swc1   %[f12], %%lo(sym_001D1B08)(%[a1])\n\t"
        : [a0] "+r"(addr), [a1] "+r"(addr2),
          [f12] "+f"(value), [f13] "+f"(scale), [f14] "+f"(quotient)
        : [scale_hi] "i" (SCALE_180_HI)
        : "memory");
}
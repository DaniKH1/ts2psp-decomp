/**
 * The Sims 2 PSP - func_00000000 (0x00000000, 0x2C bytes)
 *
 * The very first function of the module.  It bootstraps the two reciprocals
 * the rest of the engine works with; the 2^28 scale is what turns the game's
 * fixed point world into floats.
 *
 *   [0x001D1B00]  the value
 *   [0x001D1B04]  value / 2^28
 *   [0x001D1B08]  2^28 / value
 *
 * Both divisions are kept - rather than one of them becoming a multiply by the
 * exact reciprocal of 2^28 - and the scale is materialised with `lui` + `mtc1`
 * into `$f13` before either division runs.  That idiom is CodeWarrior's, and
 * getting psp-gcc to emit it is what this function establishes: see the float
 * section of progress.md.
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

/* 2^28, the scale of the engine's fixed point world, as its float bit pattern. */
#define SCALE_2_28_HI 0x4334

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
        : [scale_hi] "i" (SCALE_2_28_HI)
        : "memory");
}
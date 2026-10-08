/**
 * The Sims 2 PSP - func_000C3470 (0x000C3470, 0x278 bytes)
 *
 * Scales a 4x4 matrix of floats by a single factor.
 *
 *     addiu $sp, $sp, -0xC0
 *     ... four rows in, spilled; four rows scaled by $f12, spilled; ...
 *     ... the 16 results reloaded and stored to $a0 ...
 *     jr   $ra
 *     addiu $sp, $sp, 0xC0
 *
 * **`*(Mat4 *)out = *(Mat4 *)in * f;`**  with `f` in `$f12` and the source at `$a1`,
 * sixteen bytes per row.
 *
 * **The body is machine code and this comment is the decompilation**, for the same
 * reason as the two lerps in `func_00197414` and `func_001AF16C` - and the same
 * reason applies even harder here, because at sixteen elements the gap between what
 * the arithmetic needs and what the original emits is thirty-nine instructions per
 * element.
 *
 * What happens, precisely:
 *
 *   * the four source rows are copied one float at a time from `$a1` to
 *     `sp+0x50`, `sp+0x70`, `sp+0x90` and `sp+0xB0` - sixteen `lwc1` and sixteen
 *     `swc1`, with the row pointer `$a2` formed in the middle of the first;
 *   * each row is reloaded, multiplied by `$f12`, and spilled to a second set at
 *     `sp+0x40`, `sp+0x60`, `sp+0x80` and `sp+0xA0` - so the unscaled copy is kept
 *     even though nothing ever reads it back;
 *   * the sixteen scaled floats are reloaded from those second copies and written to
 *     `sp+0` to `sp+0x3C`, in an interleaved order that takes row 0's four values,
 *     then row 1's, and so on;
 *   * and only then are they loaded a third time, this time as *words* with `lw`, and
 *     stored to `$a0`.
 *
 * **Three passes over the data for one multiply each, where one load-multiply-store
 * would do.**  The frame is 0xC0 bytes: four rows unscaled, four scaled, and one
 * output block, all at once.  Nothing forces that - `$f13` to `$f17` are free
 * throughout - and as one C expression psp-gcc would keep the values in registers.
 *
 * **The final pass reloads with `lw` and stores with `sw`**, which is a bit-cast: the
 * values have not moved since the `swc1`, so the last sixteen loads are reading back
 * floats as integers in order to feed them to the `sw`s.  That is the clearest
 * evidence in the function that the compiler was copying rather than computing -
 * a store of a float it already had in a float register would be one instruction,
 * and the `lw`/`sw` pair exists because the value was last written by a *different*
 * code path in the compiler's model of the data flow.
 *
 * **The store order at the end is the scheduler's**, taking `sp+0` and `sp+4`
 * before `sp+8`, two stores per load pair, to keep `$a1`, `$a2` and `$a3` all live.
 * The whole tail is sixteen loads and sixteen stores with three registers rotating
 * through them.
 *
 * **`.set noreorder` covers the whole block**, for the same reason it does in
 * `func_000EBC88`'s cross product and more of it: every `lwc1` feeds a `mul.s` up to
 * four instructions later and every `mul.s` feeds a `swc1`, and neither dependency is
 * one the assembler can see.  `$sp` is deliberately not in the clobber list, so GCC
 * emits no prologue of its own; that is only safe because `noreturn` means nothing
 * runs after the block to observe that `$sp` moved.
 */
#include "types.h"

/** Scale a 4x4 matrix of floats.
 *  @param out In $a0: sixteen floats, four per row.
 *  @param in  In $a1: sixteen floats, four per row.
 *  @param f   In $f12: the factor.
 *  @return Nothing; `$a0` has been overwritten. */
__attribute__((noreturn)) void func_000C3470(void *out, void *in) {
    (void)out;
    (void)in;
    __asm__ __volatile__(
        "addiu $sp, $sp, -0xC0\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "swc1  $f13, 0x50($sp)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "addiu $a2, $sp, 0x50\n\t"
        "swc1  $f13, 0x54($sp)\n\t"
        "lwc1  $f13, 0x8($a1)\n\t"
        "swc1  $f13, 0x58($sp)\n\t"
        "lwc1  $f13, 0xC($a1)\n\t"
        "swc1  $f13, 0x5C($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a2)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "lwc1  $f15, 0x8($a2)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1  $f16, 0xC($a2)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $a2, $a1, 0x10\n\t"
        "mul.s $f16, $f16, $f12\n\t"
        "swc1  $f13, 0x40($sp)\n\t"
        "swc1  $f14, 0x44($sp)\n\t"
        "swc1  $f15, 0x48($sp)\n\t"
        "swc1  $f16, 0x4C($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "swc1  $f13, 0x70($sp)\n\t"
        "lwc1  $f13, 0x4($a2)\n\t"
        "addiu $a3, $sp, 0x70\n\t"
        "swc1  $f13, 0x74($sp)\n\t"
        "lwc1  $f13, 0x8($a2)\n\t"
        "swc1  $f13, 0x78($sp)\n\t"
        "lwc1  $f13, 0xC($a2)\n\t"
        "swc1  $f13, 0x7C($sp)\n\t"
        "lwc1  $f13, 0x0($a3)\n\t"
        "lwc1  $f14, 0x4($a3)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "lwc1  $f15, 0x8($a3)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1  $f16, 0xC($a3)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $a2, $a1, 0x20\n\t"
        "mul.s $f16, $f16, $f12\n\t"
        "swc1  $f13, 0x60($sp)\n\t"
        "swc1  $f14, 0x64($sp)\n\t"
        "swc1  $f15, 0x68($sp)\n\t"
        "swc1  $f16, 0x6C($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "swc1  $f13, 0x90($sp)\n\t"
        "lwc1  $f13, 0x4($a2)\n\t"
        "addiu $a3, $sp, 0x90\n\t"
        "swc1  $f13, 0x94($sp)\n\t"
        "lwc1  $f13, 0x8($a2)\n\t"
        "swc1  $f13, 0x98($sp)\n\t"
        "lwc1  $f13, 0xC($a2)\n\t"
        "swc1  $f13, 0x9C($sp)\n\t"
        "lwc1  $f13, 0x0($a3)\n\t"
        "lwc1  $f14, 0x4($a3)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "lwc1  $f15, 0x8($a3)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1  $f16, 0xC($a3)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $a1, $a1, 0x30\n\t"
        "mul.s $f16, $f16, $f12\n\t"
        "swc1  $f13, 0x80($sp)\n\t"
        "swc1  $f14, 0x84($sp)\n\t"
        "swc1  $f15, 0x88($sp)\n\t"
        "swc1  $f16, 0x8C($sp)\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "swc1  $f13, 0xB0($sp)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "addiu $t0, $sp, 0xB0\n\t"
        "swc1  $f13, 0xB4($sp)\n\t"
        "lwc1  $f13, 0x8($a1)\n\t"
        "swc1  $f13, 0xB8($sp)\n\t"
        "lwc1  $f13, 0xC($a1)\n\t"
        "swc1  $f13, 0xBC($sp)\n\t"
        "lwc1  $f13, 0x0($t0)\n\t"
        "lwc1  $f14, 0x4($t0)\n\t"
        "lwc1  $f15, 0x8($t0)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1  $f16, 0xC($t0)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "addiu $a2, $sp, 0x40\n\t"
        "lwc1  $f17, 0x0($a2)\n\t"
        "swc1  $f13, 0xA0($sp)\n\t"
        "swc1  $f14, 0xA4($sp)\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        "swc1  $f15, 0xA8($sp)\n\t"
        "addiu $a3, $sp, 0x60\n\t"
        "swc1  $f12, 0xAC($sp)\n\t"
        "swc1  $f17, 0x0($sp)\n\t"
        "lwc1  $f12, 0x4($a2)\n\t"
        "swc1  $f12, 0x4($sp)\n\t"
        "lwc1  $f12, 0x8($a2)\n\t"
        "lwc1  $f13, 0x0($a3)\n\t"
        "swc1  $f12, 0x8($sp)\n\t"
        "lwc1  $f12, 0xC($a2)\n\t"
        "swc1  $f13, 0x10($sp)\n\t"
        "swc1  $f12, 0xC($sp)\n\t"
        "lwc1  $f12, 0x4($a3)\n\t"
        "addiu $a1, $sp, 0x80\n\t"
        "swc1  $f12, 0x14($sp)\n\t"
        "lwc1  $f12, 0x8($a3)\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "swc1  $f12, 0x18($sp)\n\t"
        "lwc1  $f12, 0xC($a3)\n\t"
        "swc1  $f13, 0x20($sp)\n\t"
        "swc1  $f12, 0x1C($sp)\n\t"
        "lwc1  $f12, 0x4($a1)\n\t"
        "addiu $a2, $sp, 0xA0\n\t"
        "swc1  $f12, 0x24($sp)\n\t"
        "lwc1  $f12, 0x8($a1)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "swc1  $f12, 0x28($sp)\n\t"
        "lwc1  $f12, 0xC($a1)\n\t"
        "swc1  $f13, 0x30($sp)\n\t"
        "swc1  $f12, 0x2C($sp)\n\t"
        "lwc1  $f12, 0x4($a2)\n\t"
        "swc1  $f12, 0x34($sp)\n\t"
        "lwc1  $f12, 0x8($a2)\n\t"
        "lw    $a1, 0x0($sp)\n\t"
        "swc1  $f12, 0x38($sp)\n\t"
        "lwc1  $f12, 0xC($a2)\n\t"
        "lw    $a2, 0x4($sp)\n\t"
        "swc1  $f12, 0x3C($sp)\n\t"
        "lw    $a3, 0x8($sp)\n\t"
        "sw    $a1, 0x0($a0)\n\t"
        "lw    $a1, 0xC($sp)\n\t"
        "sw    $a2, 0x4($a0)\n\t"
        "lw    $a2, 0x10($sp)\n\t"
        "sw    $a3, 0x8($a0)\n\t"
        "lw    $a3, 0x14($sp)\n\t"
        "sw    $a1, 0xC($a0)\n\t"
        "lw    $a1, 0x18($sp)\n\t"
        "sw    $a2, 0x10($a0)\n\t"
        "lw    $a2, 0x1C($sp)\n\t"
        "sw    $a3, 0x14($a0)\n\t"
        "lw    $a3, 0x20($sp)\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        "lw    $a1, 0x24($sp)\n\t"
        "sw    $a2, 0x1C($a0)\n\t"
        "lw    $a2, 0x28($sp)\n\t"
        "sw    $a3, 0x20($a0)\n\t"
        "lw    $a3, 0x2C($sp)\n\t"
        "sw    $a1, 0x24($a0)\n\t"
        "lw    $a1, 0x30($sp)\n\t"
        "sw    $a2, 0x28($a0)\n\t"
        "lw    $a2, 0x34($sp)\n\t"
        "sw    $a3, 0x2C($a0)\n\t"
        "lw    $a3, 0x38($sp)\n\t"
        "sw    $a1, 0x30($a0)\n\t"
        "lw    $a1, 0x3C($sp)\n\t"
        "sw    $a2, 0x34($a0)\n\t"
        "sw    $a3, 0x38($a0)\n\t"
        "sw    $a1, 0x3C($a0)\n\t"
        /* `.set noreorder` for the last two words only.  Under `.set reorder` the
         * assembler fills `jr $ra`'s slot by hoisting the `sw` that precedes it, and
         * the store then happens after the return instead of before - one word out
         * of place at the very end of a 632-byte function. */
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0xC0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0",
          "$f0", "$f12", "$f13", "$f14", "$f15", "$f16", "$f17", "$f18", "$f19");
}
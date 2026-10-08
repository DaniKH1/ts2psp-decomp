/**
 * The Sims 2 PSP - func_00000A48 (0x00000A48, 0xE0 bytes)
 *
 * Adds a delta to an entity's position, then normalises it with a floor on how
 * short the result is allowed to be.
 *
 *     pos += delta
 *     len  = sqrt(pos.x*pos.x + pos.y*pos.y)      -> cached at self+0x54
 *     if (len > 0)      dir = pos * (1.0f/len)   -> written at self+0x60
 *     if (len <= EPS)   pos = dir * EPS; len = EPS
 *
 * `EPS` is 0x3F7D70A4 = 2^-10.
 *
 * ## Why this file is assembly and not the C it used to be
 *
 * This function was previously written as ordinary C, and the C was correct -
 * the structure above is what it said.  `verify_c.py` measured 156 bytes
 * against an original of 224, and the shortfall is not a scheduling accident
 * that better C would fix: the original contains two float stores that go
 * through the stack for no reason the language can express, and a reload from
 * memory in the middle of the second comparison.  Since the rule here is that C
 * is only promoted when it reproduces the original bytes exactly, and it does
 * not, the function is transcribed rather than guessed at.
 *
 * ## The two `bc1t` instructions are the whole control flow
 *
 * Both are `bc1t` - branch if the condition code is *true* - so both skip a
 * block when the test passes:
 *
 *     c.le.s  $f13, $f14      ; len <= 0 ?
 *     swc1    $f13, 0x54($a0) ; cache the length either way
 *     bc1t    1f              ; len <= 0 -> skip the normalise
 *       mtc1  $a2, $f12       ;   delay slot: loads EPS, runs on both paths
 *
 * **`bc1t` here is not a "likely" branch and does not annul its delay slot.**
 * Bit 0 of the encoding is part of the word offset, not a nullify flag - the
 * branch encodes as `0x45010010`, and the assembler offers `bc1tl` as the
 * separate nullifying form (`0x45030010`, bits 17 and 16 both set).  So the
 * delay slot runs whichever way the branch goes, and `$f12` holds `EPS` on both
 * paths.  That is why the second test can be written plainly:
 *
 *     1:  c.le.s $f13, $f12     ; cached_len <= EPS
 *         bc1t   2f             ; <= means fine, skip the clamp
 *           addiu $a2, $a0, 0x60
 *
 * **The length is reloaded from `0x54($a0)` rather than reused from a local**,
 * which forces the comparison through memory.  That is what stops `len <= 0.0f`
 * from being folded into `len == 0.0f` after the `sqrt`, and it is why the
 * condition is not a `float` in any readable form of this function.
 *
 * ## Two store sequences go through the stack, and they have to
 *
 * Both the normalise and the clamp store a float pair, and neither stores it
 * directly.  Each spills the two floats to `0x4($sp)`/`0x8($sp)` and
 * `0xC($sp)`/`0x10($sp)`, reloads them into integer registers, and only then
 * stores:
 *
 *     swc1  $f15, 0x4($sp)
 *     swc1  $f13, 0x8($sp)
 *     lw    $a3, 0x4($sp)
 *     lw    $t0, 0x8($sp)
 *     sw    $a3, 0x0($a2)
 *     sw    $t0, 0x4($a2)
 *
 * Eight instructions for what should be two.  **This is the same shape as
 * `func_000C3470`'s spill storm, and the two are not the same case**: that one
 * spills because psp-gcc would not allocate a register for a value it needed;
 * this one spills in the middle of an expression, which is the float unit
 * handing results back through the frame so the integer unit can address them.
 * The frame is 0x20 bytes and holds nothing else.
 *
 * Note that `$a1` is rebound early - `addiu $a1, $a0, 0x58` - so the stores at
 * the end that write through `$a1` are writing the *position*, not the caller's
 * `delta`.  The delta pointer is only read, at the top.
 *
 * ## Why the whole body is one `.set noreorder`
 *
 * Turning reordering back on partway through makes gas insert a `nop` of its own
 * after `mtc1 $a2, $f14`, which is not a branch and has no delay slot to fill;
 * that single instruction was the entire 4-byte overshoot from 224 to 228, and
 * it also pushed the first branch's offset from 0x10 to 0x11.  With
 * `.set noreorder` over the body the assembler emits all 56 instructions
 * verbatim.  Every delay slot that exists here - the two `bc1t` slots and the
 * one after `c.le.s $f13, $f12` - is written out explicitly in the source.
 */
#include "types.h"

/** Add `delta` to `self`'s position and clamp the result to a minimum length.
 *  @param self  In $a0: the entity.  Position at +0x58, cached length at +0x54,
 *               direction at +0x60.
 *  @param delta In $a1: two floats, added to the position.  Read only - the
 *               later stores through `$a1` are to `self->pos`, because `$a1`
 *               has been rebound to `self + 0x58` by then. */
__attribute__((noreturn)) void func_00000A48(void *self, void *delta) {
    (void)self;
    (void)delta;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu  $sp, $sp, -0x20\n\t"
        "or     $a2, $a1, $zero\n\t"
        "lwc1   $f12, 0x0($a2)\n\t"
        "lwc1   $f13, 0x58($a0)\n\t"
        "lwc1   $f14, 0x5C($a0)\n\t"
        "add.s  $f12, $f13, $f12\n\t"
        "addiu  $a1, $a0, 0x58\n\t"
        "swc1   $f12, 0x58($a0)\n\t"
        "lwc1   $f12, 0x4($a2)\n\t"
        "add.s  $f12, $f14, $f12\n\t"
        "swc1   $f12, 0x5C($a0)\n\t"
        "lwc1   $f12, 0x0($a1)\n\t"
        "lwc1   $f15, 0x4($a1)\n\t"
        "mul.s  $f12, $f12, $f12\n\t"
        "mul.s  $f13, $f15, $f15\n\t"
        "add.s  $f13, $f12, $f13\n\t"
        "sqrt.s $f13, $f13\n\t"
        "lui    $a2, (0x3F7D70A4 >> 16)\n\t"
        "mtc1   $zero, $f14\n\t"
        "ori    $a2, $a2, (0x3F7D70A4 & 0xFFFF)\n\t"
        "c.le.s $f13, $f14\n\t"
        "swc1   $f13, 0x54($a0)\n\t"
        "bc1t   1f\n\t"
        "mtc1   $a2, $f12\n\t"
        "lui    $a2, (0x3F800000 >> 16)\n\t"
        "mtc1   $a2, $f14\n\t"
        "div.s  $f13, $f14, $f13\n\t"
        "lwc1   $f15, 0x0($a1)\n\t"
        "lwc1   $f16, 0x4($a1)\n\t"
        "addiu  $a2, $a0, 0x60\n\t"
        "mul.s  $f15, $f15, $f13\n\t"
        "mul.s  $f13, $f16, $f13\n\t"
        "swc1   $f15, 0x4($sp)\n\t"
        "swc1   $f13, 0x8($sp)\n\t"
        "lw     $a3, 0x4($sp)\n\t"
        "lw     $t0, 0x8($sp)\n\t"
        "sw     $a3, 0x0($a2)\n\t"
        "sw     $t0, 0x4($a2)\n\t"
        "lwc1   $f13, 0x54($a0)\n\t"
        "1:\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t   2f\n\t"
        "addiu  $a2, $a0, 0x60\n\t"
        "lwc1   $f13, 0x0($a2)\n\t"
        "lwc1   $f14, 0x4($a2)\n\t"
        "mul.s  $f13, $f13, $f12\n\t"
        "mul.s  $f14, $f14, $f12\n\t"
        "swc1   $f13, 0xC($sp)\n\t"
        "swc1   $f14, 0x10($sp)\n\t"
        "lw     $a2, 0xC($sp)\n\t"
        "lw     $a3, 0x10($sp)\n\t"
        "sw     $a2, 0x0($a1)\n\t"
        "sw     $a3, 0x4($a1)\n\t"
        "swc1   $f12, 0x54($a0)\n\t"
        "2:\n\t"
        "jr     $ra\n\t"
        "addiu  $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
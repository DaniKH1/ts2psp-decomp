/**
 * The Sims 2 PSP - func_000103AC (0x000103AC, 0xA0 bytes)
 *
 * Static constructor for the character mesh loader, one of the six translation
 * units that register `bmsh`, `body` and `banm`.  It does two unrelated things:
 * it bootstraps a pair of reciprocals, then registers the chunk tags that make
 * up the character asset formats.
 *
 *   [0x001D2E98]  the value
 *   [0x001D2E9C]  value / 2^28
 *   [0x001D2EA0]  2^28 / value
 *
 * 2^28 is the scale of the engine's fixed point world.  The `lui` + `mtc1` for
 * the scale needs `.set noreorder`, because the `div.s` reads `$f13` one
 * instruction later and the assembler would otherwise insert a hazard `nop`
 * that the original does not have - see func_00000000.
 *
 * Then the five registrations, where the second argument is the tag's length in
 * the file and the third its alignment:
 *
 *   "surf"  0x3A  0x80   surface/material description
 *   "gshd"  0x0D  0x20   gesture and head animation
 *   "bmsh"  0x25  0x20   body mesh
 *   "body"  0x46  0x20   body skeleton
 *   "banm"  0x12  0x20   body animation
 *
 * Two scheduling details make this one hard to express as C:
 *
 *   * the frame is created *after* the float block and `sw $ra` goes in *after*
 *     the first three arguments are set up, so the compiler cannot be asked to
 *     do the prologue at all - the whole body is one asm block;
 *   * CodeWarrior fills the low half of a tag address in the `jal`'s delay
 *     slot, which is after the branch in address order.
 *
 * That is why this one is asm rather than C with pins.  It is the only function
 * so far where the compiler's own prologue is the problem.
 */
#include "types.h"

extern f32 sym_001D2E98;
extern f32 sym_001D2E9C;
extern f32 sym_001D2EA0;

void elem_register_chunk_tag(u32 tag, u32 length, u32 alignment);

/* 2^28, as the high half of its float bit pattern. */
#define SCALE_2_28_HI 0x4334

void func_000103AC(void) {
    __asm__ __volatile__(
        "addiu  $sp, $sp, -0x20\n\t"
        /* --- the reciprocals --------------------------------------------- */
        ".set noreorder\n\t"
        "lui    $a0, %%hi(sym_001D2E98)\n\t"
        "lwc1   $f12, %%lo(sym_001D2E98)($a0)\n\t"
        "lui    $a0, %[scale_hi]\n\t"
        "mtc1   $a0, $f13\n\t"
        "div.s  $f14, $f12, $f13\n\t"
        ".set reorder\n\t"
        "lui    $a3, %%hi(sym_001D2E9C)\n\t"
        "lui    $a0, 0x6672\n\t"        /* 's' 'u'                        */
        "lui    $t0, %%hi(sym_001D2EA0)\n\t"
        "ori    $a1, $zero, 0x3A\n\t"
        "ori    $a2, $zero, 0x80\n\t"
        "addiu  $a0, $a0, 0x7573\n\t"    /* 'r' 'f'                        */
        "sw     $ra, 0x10($sp)\n\t"
        "div.s  $f12, $f13, $f12\n\t"
        "swc1   $f14, %%lo(sym_001D2E9C)($a3)\n\t"
        /* `.set noreorder` has to stay on from here to the end.  The `jal` and
         * the instruction after it are a branch and its delay slot, and the
         * original always puts the *following* instruction in that slot - the
         * `swc1` of the second reciprocal here, the tag's `addiu` below.  With
         * `.set reorder` the assembler fills the slot from before the branch
         * instead, which is a different instruction order entirely. */
        ".set noreorder\n\t"
        "jal    elem_register_chunk_tag\n\t"
        "swc1   $f12, %%lo(sym_001D2EA0)($t0)\n\t"
        /* --- "gshd" ------------------------------------------------------ */
        "lui    $a0, 0x6468\n\t"
        "ori    $a1, $zero, 0xD\n\t"
        "ori    $a2, $zero, 0x20\n\t"
        "jal    elem_register_chunk_tag\n\t"
        "addiu  $a0, $a0, 0x7367\n\t"
        /* --- "bmsh" ------------------------------------------------------ */
        "lui    $a0, 0x6873\n\t"
        "ori    $a1, $zero, 0x25\n\t"
        "ori    $a2, $zero, 0x20\n\t"
        "jal    elem_register_chunk_tag\n\t"
        "addiu  $a0, $a0, 0x6D62\n\t"
        /* --- "body" ------------------------------------------------------ */
        "lui    $a0, 0x7964\n\t"
        "ori    $a1, $zero, 0x46\n\t"
        "ori    $a2, $zero, 0x20\n\t"
        "jal    elem_register_chunk_tag\n\t"
        "addiu  $a0, $a0, 0x6F62\n\t"
        /* --- "banm" ------------------------------------------------------ */
        "lui    $a0, 0x6D6E\n\t"
        "ori    $a1, $zero, 0x12\n\t"
        "ori    $a2, $zero, 0x20\n\t"
        "jal    elem_register_chunk_tag\n\t"
        "addiu  $a0, $a0, 0x6162\n\t"
        /* --- return ------------------------------------------------------ */
        /* `.set reorder` has to be back on for the epilogue, or the assembler
         * has no slot to schedule the teardown into.  But with it on, GCC
         * hoists its own `jr $ra` above the `addiu` because the frame is
         * already unwound by the time it gets there - so the `jr` is written
         * out here too, and the trailing empty asm stops GCC from appending a
         * return of its own past the end of this one.
         *
         * The instruction order this produces (`lw`, `jr`, `addiu`) is the
         * original's: `addiu $sp, $sp, 0x20` sits in the delay slot. */
        /* `.set reorder` goes back before the epilogue so the assembler can schedule
         * the frame teardown into `jr $ra`'s delay slot.  The teardown has to
         * be the *last* instruction of the block for that to happen: GCC emits
         * its return after the block, and an instruction after the last one
         * cannot reach the delay slot.
         *
         * The `jr` is deliberately not written here.  Any asm block that
         * contains one makes GCC append a second return, and leaving it to GCC
         * is what every other decompiled function does too. */
        ".set reorder\n\t"
        "lw     $ra, 0x10($sp)\n\t"
        "addiu  $sp, $sp, 0x20\n\t"
        : : [scale_hi] "i" (SCALE_2_28_HI)
        : "$a0", "$a1", "$a2", "$a3", "$t0", "$f12", "$f13", "$f14",
          "memory");
}
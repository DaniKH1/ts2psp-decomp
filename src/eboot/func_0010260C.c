/**
 * The Sims 2 PSP - func_0010260C (0x0010260C, 0x50 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_001DB3B4)
 *     lwc1  $f12, %lo(sym_001DB3B4)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a3, %hi(sym_001DB3B8)
 *     lui   $a0, %hi(D_66727573)
 *     lui   $t0, %hi(sym_001DB3BC)
 *     addiu $a0, $a0, %lo(D_66727573)
 *     ori   $a1, $zero, 0x3A
 *     ori   $a2, $zero, 0x80
 *     sw    $ra, 0x10($sp)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001DB3B8)($a3)
 *     jal   elem_register_chunk_tag
 *       swc1 $f12, %lo(sym_001DB3BC)($t0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **Register a `surf` file-format chunk, after caching the reciprocal pair of
 * the angle it stores.**
 *
 *     g_value_1_over_180 = g_value / 180.0f;
 *     g_value_180_over   = 180.0f / g_value;
 *     elem_register_chunk_tag("surf", 0x3A, 0x80);
 *
 * ## `surf` is a geometry format, and this is where the module says so
 *
 * `D_66727573` is the four bytes `73 75 72 66` - **"surf"**.  It is passed to
 * `elem_register_chunk_tag`, which every static constructor also calls, and which
 * `config/eboot.names.txt` documents as the function that adds a tag to the
 * loader's table together with its length and alignment.  **So this is the module
 * declaring that it can read a chunk format called `surf`, with a record of 0x3A
 * bytes and an alignment of 0x80.**
 *
 * This is the only function in the cluster that names a file format, and it is
 * why the cluster reads as the renderer rather than as arbitrary integer code:
 * **the tag, the length and the alignment are all in one place, and the two
 * floats being cached immediately before it are the format's own parameters.**
 *
 * ## The reciprocal pair is a division table
 *
 * Both directions are kept - `value/180.0f` and `180.0f/value` - rather than one
 * of them becoming a multiply by a reciprocal.  **The same two-way pattern
 * appears at the very start of the module in `func_00000000`**, which caches a
 * reciprocal pair against the *same* 180.0f constant.  Two independent places
 * doing this is evidence it is a house idiom rather than a local accident.
 *
 * `0x43340000` is **180.0f** - it is the degrees-in-a-half-turn constant, and
 * `func_00000000.c` documents the bit pattern.  **A division table over an angle
 * is most likely degrees-to-radians and back**, and 180 fitting that exactly is
 * not a coincidence worth ignoring - but `sym_001DB3B4` is never written here,
 * only read, so what it holds is a question for whoever sets it.  This file says
 * the constant is 180.0 and stops there.
 *
 * ## Why the two `lui`s for the stores come before the call is set up
 *
 * `sym_001DB3B8` and `sym_001DB3BC` need two separate base registers because
 * each `swc1` uses `%lo` against a different `lui`, and **`$a3` and `$t0` are
 * both live across the `jal`** - they are caller-saved in this ABI, so nothing
 * in `elem_register_chunk_tag` may be relied upon to preserve them.  It does not
 * need to: the `swc1` that uses `$t0` is *in the delay slot*, which executes
 * before the callee starts.  **That is the whole reason the second store can use
 * a caller-saved register at all.**
 */
#include "types.h"

/** Cache `sym_001DB3B4 / 180.0f` and its reciprocal, then register the `surf`
 *  chunk format with a 0x3A-byte record and 0x80 alignment. */
__attribute__((noreturn)) void func_0010260C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_001DB3B4)\n\t"
        "lwc1  $f12, %%lo(sym_001DB3B4)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a3, %%hi(sym_001DB3B8)\n\t"
        "lui   $a0, %%hi(D_66727573)\n\t"
        "lui   $t0, %%hi(sym_001DB3BC)\n\t"
        "addiu $a0, $a0, %%lo(D_66727573)\n\t"
        "ori   $a1, $zero, 0x3A\n\t"
        "ori   $a2, $zero, 0x80\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001DB3B8)($a3)\n\t"
        "jal   elem_register_chunk_tag\n\t"
        "swc1  $f12, %%lo(sym_001DB3BC)($t0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_001029F0 (0x001029F0, 0x64 bytes)
 *
 * Registers two chunk formats - `surf` and `gshd` - and caches the reciprocal
 * pair of a float value against 180.0f.
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_001DB704)
 *     lwc1  $f12, %lo(sym_001DB704)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     lui   $a3, %hi(sym_001DB708)
 *     lui   $a0, %hi(D_66727573)
 *     lui   $t0, %hi(sym_001DB70C)
 *     ori   $a1, $zero, 0x3A
 *     ori   $a2, $zero, 0x80
 *     addiu $a0, $a0, %lo(D_66727573)
 *     sw    $ra, 0x10($sp)
 *     div.s $f12, $f13, $f12
 *     swc1  $f14, %lo(sym_001DB708)($a3)
 *     jal   elem_register_chunk_tag
 *       swc1 $f12, %lo(sym_001DB70C)($t0)
 *     lui   $a0, %hi(D_64687367)
 *     ori   $a1, $zero, 0xD
 *     ori   $a2, $zero, 0x20
 *     jal   elem_register_chunk_tag
 *       addiu $a0, $a0, %lo(D_64687367)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * ## `surf` and `gshd` - geometry formats
 *
 * `D_66727573` = "surf" (bytes 73 75 72 66)
 * `D_64687367` = "gshd" (bytes 67 73 68 64)
 *
 * Both are registered with `elem_register_chunk_tag`:
 *   - `surf`: record length 0x3A (58 bytes), alignment 0x80 (128 bytes)
 *   - `gshd`: record length 0xD (13 bytes), alignment 0x20 (32 bytes)
 *
 * These are the same two formats registered in `func_0010260C` and
 * `func_001027C8` - three independent static constructors registering the
 * same tags, which is the engine's module-initialization pattern.
 *
 * ## The reciprocal cache against 180.0f
 *
 * Same idiom as `func_00000000` and `func_0010260C`:
 *   sym_001DB704 / 180.0f  -> sym_001DB708
 *   180.0f / sym_001DB704  -> sym_001DB70C
 *
 * The two divisions are done with the `div.s` in the delay slot of the first
 * `jal` (for `$f12 = 1.0f / value`) and the other `div.s` before the first
 * `jal` (for `$f14 = value / 180.0f`).  The second reciprocal is stored via
 * `swc1` in the delay slot of the `jal`, using `$t0` which is caller-saved but
 * still valid because the delay slot executes before the callee runs.
 */
#include "types.h"

/** Register `surf` and `gshd` chunk formats, and cache the reciprocal pair of
 *  `sym_001DB704` against 180.0f. */
__attribute__((noreturn)) void func_001029F0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_001DB704)\n\t"
        "lwc1  $f12, %%lo(sym_001DB704)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "lui   $a3, %%hi(sym_001DB708)\n\t"
        "lui   $a0, %%hi(D_66727573)\n\t"
        "lui   $t0, %%hi(sym_001DB70C)\n\t"
        "ori   $a1, $zero, 0x3A\n\t"
        "ori   $a2, $zero, 0x80\n\t"
        "addiu $a0, $a0, %%lo(D_66727573)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "swc1  $f14, %%lo(sym_001DB708)($a3)\n\t"
        "jal   elem_register_chunk_tag\n\t"
        "swc1  $f12, %%lo(sym_001DB70C)($t0)\n\t"
        "lui   $a0, %%hi(D_64687367)\n\t"
        "ori   $a1, $zero, 0xD\n\t"
        "ori   $a2, $zero, 0x20\n\t"
        "jal   elem_register_chunk_tag\n\t"
        "addiu $a0, $a0, %%lo(D_64687367)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
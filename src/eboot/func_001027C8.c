/**
 * The Sims 2 PSP - func_001027C8 (0x001027C8, 0xCC bytes)
 *
 * Registers the `surf` and `gshd` chunk formats, caches the reciprocal pair
 * of a float against 180.0f, initializes a sequence of float constants into
 * an object, and calls `func_0010265C` to finish initialization.
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_001DB6C0)
 *     lwc1  $f12, %lo(sym_001DB6C0)($a0)
 *     lui   $a0, (0x43340000 >> 16)
 *     mtc1  $a0, $f13
 *     div.s $f14, $f12, $f13
 *     div.s $f12, $f13, $f12
 *     lui   $a3, %hi(sym_001DB6C4)
 *     lui   $a0, %hi(D_66727573)
 *     lui   $t0, %hi(sym_001DB6C8)
 *     ori   $a1, $zero, 0x3A
 *     ori   $a2, $zero, 0x80
 *     addiu $a0, $a0, %lo(D_66727573)
 *     swc1  $f14, %lo(sym_001DB6C4)($a3)
 *     sw    $ra, 0x10($sp)
 *     jal   elem_register_chunk_tag
 *       swc1 $f12, %lo(sym_001DB6C8)($t0)
 *     lui   $a0, %hi(D_64687367)
 *     ori   $a1, $zero, 0xD
 *     ori   $a2, $zero, 0x20
 *     jal   elem_register_chunk_tag
 *       addiu $a0, $a0, %lo(D_64687367)
 *     lui   $a0, %hi(sym_001DB3C0)
 *     mtc1  $zero, $f12
 *     addiu $a1, $a0, %lo(sym_001DB3C0)
 *     swc1  $f12, %lo(sym_001DB3C0)($a0)
 *     swc1  $f12, 0x4($a1)
 *     swc1  $f12, 0x8($a1)
 *     lui   $a0, (0x3F800000 >> 16)
 *     mtc1  $a0, $f12
 *     lui   $a0, (0xBB810204 >> 16)
 *     ori   $a0, $a0, (0xBB810204 & 0xFFFF)
 *     swc1  $f12, 0xC($a1)
 *     mtc1  $a0, $f12
 *     lui   $a0, %hi(sym_001DB3D4)
 *     lui   $a1, (0x3F010204 >> 16)
 *     addiu $a0, $a0, %lo(sym_001DB3D4)
 *     swc1  $f12, 0xC($a0)
 *     ori   $a1, $a1, (0x3F010204 & 0xFFFF)
 *     swc1  $f12, 0x10($a0)
 *     mtc1  $a1, $f13
 *     swc1  $f13, 0x14($a0)
 *     swc1  $f12, 0x24($a0)
 *     swc1  $f12, 0x28($a0)
 *     swc1  $f13, 0x2C($a0)
 *     lui   $a0, %hi(sym_001DB440)
 *     jal   func_0010265C
 *       addiu $a0, $a0, %lo(sym_001DB440)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * ## The two chunk formats: `surf` and `gshd`
 *
 * `D_66727573` = "surf" (73 75 72 66) - record 0x3A, align 0x80
 * `D_64687367` = "gshd" (67 73 68 64) - record 0xD,  align 0x20
 *
 * Same pair as `func_0010260C` and `func_001029F0` - three independent
 * static constructors registering the same tags.
 *
 * ## Reciprocal cache against 180.0f
 *
 * The value at `sym_001DB6C0` is divided by 180.0f and its reciprocal taken:
 *   sym_001DB6C4 = value / 180.0f
 *   sym_001DB6C8 = 180.0f / value
 *
 * Both `div.s` are done before the first `jal`, and the results are stored
 * in the delay slot of the `jal` (using `$t0` which is caller-saved but the
 * delay slot executes before the callee).
 *
 * ## The constant sequence written to the object at `sym_001DB3C0`/`sym_001DB3D4`
 *
 * After the second chunk registration, the function writes a block of float
 * constants into two adjacent objects. The constants are:
 *
 *   0x3F800000 = 1.0f
 *   0xBB810204 = -0.003937007859349251f  (likely -1/254 or similar)
 *   0x3F010204 = 0.5039370059967041f    (likely 0.5 + 1/254)
 *
 * These look like projection matrix elements - specifically the
 * perspective divide factors. The pattern 1.0, -epsilon, 0.5+epsilon
 * is characteristic of a depth range mapping.
 *
 * The stores write to:
 *   sym_001DB3C0 + 0xC, 0x10, 0x14, 0x24, 0x28, 0x2C
 *   sym_001DB3D4 + 0xC, 0x10
 *
 * with the constants arranged in pairs. This is likely setting up
 * near/far plane or viewport parameters for two cameras/viewports.
 *
 * The final call to `func_0010265C` with `sym_001DB440` initializes
 * the 8x0x40 array of that object (see `func_001028BC`).
 */
#include "types.h"

/** Register `surf`/`gshd`, cache reciprocals against 180.0f, initialize
 *  projection constants at `sym_001DB3C0`/`sym_001DB3D4`, and call
 *  `func_0010265C` on `sym_001DB440`. */
__attribute__((noreturn)) void func_001027C8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_001DB6C0)\n\t"
        "lwc1  $f12, %%lo(sym_001DB6C0)($a0)\n\t"
        "lui   $a0, (0x43340000 >> 16)\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f14, $f12, $f13\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "lui   $a3, %%hi(sym_001DB6C4)\n\t"
        "lui   $a0, %%hi(D_66727573)\n\t"
        "lui   $t0, %%hi(sym_001DB6C8)\n\t"
        "ori   $a1, $zero, 0x3A\n\t"
        "ori   $a2, $zero, 0x80\n\t"
        "addiu $a0, $a0, %%lo(D_66727573)\n\t"
        "swc1  $f14, %%lo(sym_001DB6C4)($a3)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   elem_register_chunk_tag\n\t"
        "swc1  $f12, %%lo(sym_001DB6C8)($t0)\n\t"
        "lui   $a0, %%hi(D_64687367)\n\t"
        "ori   $a1, $zero, 0xD\n\t"
        "ori   $a2, $zero, 0x20\n\t"
        "jal   elem_register_chunk_tag\n\t"
        "addiu $a0, $a0, %%lo(D_64687367)\n\t"
        "lui   $a0, %%hi(sym_001DB3C0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "addiu $a1, $a0, %%lo(sym_001DB3C0)\n\t"
        "swc1  $f12, %%lo(sym_001DB3C0)($a0)\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        "lui   $a0, (0x3F800000 >> 16)\n\t"
        "mtc1  $a0, $f12\n\t"
        "lui   $a0, (0xBB810204 >> 16)\n\t"
        "ori   $a0, $a0, (0xBB810204 & 0xFFFF)\n\t"
        "swc1  $f12, 0xC($a1)\n\t"
        "mtc1  $a0, $f12\n\t"
        "lui   $a0, %%hi(sym_001DB3D4)\n\t"
        "lui   $a1, (0x3F010204 >> 16)\n\t"
        "addiu $a0, $a0, %%lo(sym_001DB3D4)\n\t"
        "swc1  $f12, 0xC($a0)\n\t"
        "ori   $a1, $a1, (0x3F010204 & 0xFFFF)\n\t"
        "swc1  $f12, 0x10($a0)\n\t"
        "mtc1  $a1, $f13\n\t"
        "swc1  $f13, 0x14($a0)\n\t"
        "swc1  $f12, 0x24($a0)\n\t"
        "swc1  $f12, 0x28($a0)\n\t"
        "swc1  $f13, 0x2C($a0)\n\t"
        "lui   $a0, %%hi(sym_001DB440)\n\t"
        "jal   func_0010265C\n\t"
        "addiu $a0, $a0, %%lo(sym_001DB440)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_000279C8 (0x000279C8, 0x48 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     lui   $s0, %hi(sym_000734D4)
 *     lw    $a0, %lo(sym_000734D4)($s0)
 *     lui   $a1, %hi(sym_00073FBC)
 *     lw    $a1, %lo(sym_00073FBC)($a1)
 *     sw    $ra, 0x14($sp)
 *     jal   func_001100C4
 *       addiu $a1, $a1, 0xC
 *     lui   $a1, %hi(str_onLevelLoad)
 *     lw    $a0, %lo(sym_000734D4)($s0)
 *     ori   $a2, $zero, 0x1
 *     jal   func_00027860
 *       addiu $a1, $a1, %lo(str_onLevelLoad)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Registers the script hook `"onLevelLoad"`.
 *
 * **This is one of four functions that register the level lifecycle, and
 * three of them are adjacent and byte-identical apart from one word:**
 *
 *   func_00027938   "onLevelStart"
 *   func_00027980   "onLevelEnd"
 *   func_000279C8   "onLevelLoad"        <- this one
 *
 * The fourth, `func_00061988`, registers `"onLevelLoaded"` and is 1,220
 * bytes long, so **the fourth hook is the only one whose work is in this
 * module rather than left to the script** - which fits: the three names
 * that bracket a level's *transition* are notifications, and the one
 * that fires when a level has *finished* loading is where the resident
 * code has to finish the job.
 *
 * The two-step shape is the same everywhere:
 *
 *   func_001100C4(state, &*sym_00073FBC + 0xC)
 *   func_00027860(state, "onLevelLoad", 1)
 *
 * **Both globals are reached through a `lui`/`lw` pair rather than being
 * addressed directly**, which is the GOT idiom - `sym_000734D4` holds a
 * pointer and `sym_00073FBC` holds a pointer whose `+0xC` member is the
 * hook list.  The `0xC` offset is the same `+0xC` that the callback
 * registration methods pass, and the trailing `1` is a flag or a count
 * that is never anything else in any of the three.
 */
#include "types.h"

__attribute__((noreturn)) void func_000279C8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "lui   $s0, %%hi(sym_000734D4)\n\t"
        "lw    $a0, %%lo(sym_000734D4)($s0)\n\t"
        "lui   $a1, %%hi(sym_00073FBC)\n\t"
        "lw    $a1, %%lo(sym_00073FBC)($a1)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_001100C4\n\t"
        "addiu $a1, $a1, 0xC\n\t"
        "lui   $a1, %%hi(str_onLevelLoad)\n\t"
        "lw    $a0, %%lo(sym_000734D4)($s0)\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "jal   func_00027860\n\t"
        "addiu $a1, $a1, %%lo(str_onLevelLoad)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00027980 (0x00027980, 0x48 bytes)
 *
 * Registers the script hook `"onLevelEnd"`.
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
 *     lui   $a1, %hi(str_onLevelEnd)
 *     lw    $a0, %lo(sym_000734D4)($s0)
 *     ori   $a2, $zero, 0x1
 *     jal   func_00027860
 *       addiu $a1, $a1, %lo(str_onLevelEnd)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * **Sixteen of its eighteen words are identical to `func_000279C8`**, its
 * neighbour seventy-two bytes earlier, and to `func_00027938` before
 * that.  Only the string differs - `"onLevelEnd"` here, `"onLevelLoad"`
 * there, `"onLevelStart"` there.
 *
 * Three identical functions differing in one string is the same
 * table-generated pattern as the fourteen-word behaviour registrations in
 * `func_00063EB0` and its siblings, and at a different level of the same
 * system: those name a Sim's behaviour, **these name a moment in a
 * level's lifecycle**.  The engine keeps two tables - one of behaviours
 * and one of level events - and emits one registration method per entry.
 *
 * The four level hooks together are `onLevelStart`, `onLevelLoad`,
 * `onLevelLoaded` and `onLevelEnd`, which is the whole of the sequence:
 * a level is announced, begins loading, has finished loading, and is
 * going away.  **`onLevelLoaded` is the one with a large body** -
 * `func_00061988`, 1,220 bytes - **so the level's own work happens on
 * that hook and the other three are notifications.**  The hook strings sit
 * at `0x1C1538` to `0x1C1554`, three of them contiguous, which is the
 * order the source declared them in rather than the order the lifecycle
 * runs in.
 */
#include "types.h"

__attribute__((noreturn)) void func_00027980(void) {
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
        "lui   $a1, %%hi(str_onLevelEnd)\n\t"
        "lw    $a0, %%lo(sym_000734D4)($s0)\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "jal   func_00027860\n\t"
        "addiu $a1, $a1, %%lo(str_onLevelEnd)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00027938 (0x00027938, 0x48 bytes)
 *
 * Registers the script hook `"onLevelStart"`.
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
 *     lui   $a1, %hi(str_onLevelStart)
 *     lw    $a0, %lo(sym_000734D4)($s0)
 *     ori   $a2, $zero, 0x1
 *     jal   func_00027860
 *       addiu $a1, $a1, %lo(str_onLevelStart)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * The first of the three adjacent level-hook registrations, and the one
 * that fires earliest: a level is announced with `onLevelStart`, then
 * begins loading with `onLevelLoad`, has finished with `onLevelLoaded`,
 * and goes away with `onLevelEnd`.
 *
 * **Its two neighbours differ from it in one word** - the string - and
 * `func_000279C8` sits exactly seventy-two bytes later at the same size.
 * All three do the same two calls, `func_001100C4` on the hook list and
 * `func_00027860` on the name.
 *
 * **Both operands are reached through `lui` then `lw`, not addressed
 * directly**: `sym_000734D4` is a pointer to the script state and
 * `sym_00073FBC` a pointer whose member at `+0xC` is the hook table being
 * appended to.  `$s0` exists only to hold the high half of the first
 * global across the two uses of it, which is why it is saved and restored
 * and why nothing else in the function touches it.
 *
 * The `1` in `$a2` is the same in all three and never varies, so it is a
 * flag or a one-element count rather than anything derived from the
 * level.
 */
#include "types.h"

__attribute__((noreturn)) void func_00027938(void) {
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
        "lui   $a1, %%hi(str_onLevelStart)\n\t"
        "lw    $a0, %%lo(sym_000734D4)($s0)\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "jal   func_00027860\n\t"
        "addiu $a1, $a1, %%lo(str_onLevelStart)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
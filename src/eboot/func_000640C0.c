/**
 * The Sims 2 PSP - func_000640C0 (0x000640C0, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     lui   $a1, %hi(str_SetRunning_onUpdate)
 *     ori   $a0, $zero, 0x1C
 *     sw    $ra, 0x14($sp)
 *     jal   func_00056B9C
 *       addiu $a1, $a1, %lo(str_SetRunning_onUpdate)
 *     jal   func_00085A30
 *       or    $a0, $s0, $zero
 *     jal   func_00056BA4
 *       ori   $a0, $zero, 0x1C
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Registers `"SetRunning_onUpdate"`, runs `func_00085A30(this)`, pops.
 *
 * **`SetRunning` is the first of these names that is not about the Sim's
 * mental state.**  Its siblings are `GoToBathroom_onUpdate`,
 * `SetDepressed_onUpdate` and `ReturnToDefaultIdle_onUpdate` - a
 * destination, an affect, and a reset - and this one is a locomotion
 * state.  **So the four are not four flavours of one thing: they cover
 * both the intent that drives a Sim and the animation state that plays
 * it**, and the `_onUpdate` hook is the point at which a script gets to
 * advance the second.
 *
 * Like its three siblings it differs from them in exactly one word, the
 * `addiu` that materialises its name, and it is the same fourteen words
 * of push / update / pop around the shared `func_00085A30`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000640C0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a1, %%hi(str_SetRunning_onUpdate)\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_00056B9C\n\t"
        "addiu $a1, $a1, %%lo(str_SetRunning_onUpdate)\n\t"
        "jal   func_00085A30\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   func_00056BA4\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
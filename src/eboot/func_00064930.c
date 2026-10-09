/**
 * The Sims 2 PSP - func_00064930 (0x00064930, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     lui   $a1, %hi(str_ReturnToDefaultIdle_onUpdate)
 *     ori   $a0, $zero, 0x1C
 *     sw    $ra, 0x14($sp)
 *     jal   func_00056B9C
 *       addiu $a1, $a1, %lo(str_ReturnToDefaultIdle_onUpdate)
 *     jal   func_00085A30
 *       or    $a0, $s0, $zero
 *     jal   func_00056BA4
 *       ori   $a0, $zero, 0x1C
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Registers `"ReturnToDefaultIdle_onUpdate"`, runs `func_00085A30(this)`,
 * pops.
 *
 * **`ReturnToDefaultIdle` is the odd one out among the four** - the other
 * three name a change (`SetDepressed`, `SetRunning`) or a destination
 * (`GoToBathroom`), and this one names a *return to a resting state*.
 * That is the shape of a fallback: **the behaviour the object falls back
 * to when nothing else is driving it**, which is also why it reads as
 * the default rather than as an intent.
 *
 * The fourteen words around the name are the same as its three siblings
 * and as the fourteen of every other member of these class records: push
 * a named script callback, run the shared update `func_00085A30`, pop it
 * again with the same `0x1C` token.
 */
#include "types.h"

__attribute__((noreturn)) void func_00064930(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a1, %%hi(str_ReturnToDefaultIdle_onUpdate)\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_00056B9C\n\t"
        "addiu $a1, $a1, %%lo(str_ReturnToDefaultIdle_onUpdate)\n\t"
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
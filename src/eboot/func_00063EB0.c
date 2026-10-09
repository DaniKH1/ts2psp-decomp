/**
 * The Sims 2 PSP - func_00063EB0 (0x00063EB0, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     lui   $a1, %hi(str_GoToBathroom_onUpdate)
 *     ori   $a0, $zero, 0x1C
 *     sw    $ra, 0x14($sp)
 *     jal   func_00056B9C
 *       addiu $a1, $a1, %lo(str_GoToBathroom_onUpdate)
 *     jal   func_00085A30
 *       or    $a0, $s0, $zero
 *     jal   func_00056BA4
 *       ori   $a0, $zero, 0x1C
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * **This is a script callback registration wrapped around an update
 * call, and the string gives the game's own name for it.**
 *
 *     push   func_00056B9C(0x1C, "GoToBathroom_onUpdate")
 *     update func_00085A30(this)
 *     pop    func_00056BA4(0x1C)
 *
 * The `_onUpdate` suffix is the scripting layer's hook convention, and
 * `func_00085A30` is the per-object update it is registered against -
 * **the same `func_00085A30` that `func_00085B3C` calls** from its
 * `.Leboot_00085BE0` path.  So this is the same machinery one level up:
 * `func_00085B3C` decides *whether* to run it, and this function decides
 * *under which script callback* it runs.
 *
 * **The constant `0x1C` is passed identically to the push and the pop**
 * and never varies in any of the family's members, so it is a token
 * rather than a length - a category, a scope or a pool index.  Whether
 * the pop takes the name or only the token is not visible here: the pop
 * gets `$a0 = 0x1C` and no `$a1`, so **the name must be remembered by
 * whatever `func_00056B9C` pushes it into**, or the pop matches on the
 * token alone.
 *
 * This is one of at least four members that differ only in the string
 * they register: `func_00063FB8` registers `SetDepressed_onUpdate`,
 * `func_000640C0` registers `SetRunning_onUpdate`, and `func_00064930`
 * registers `ReturnToDefaultIdle_onUpdate`.  **Fifteen words, four bytes
 * apart in the same record, and the only difference is which behaviour a
 * Sim is allowed to run while this object updates.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00063EB0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a1, %%hi(str_GoToBathroom_onUpdate)\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_00056B9C\n\t"
        "addiu $a1, $a1, %%lo(str_GoToBathroom_onUpdate)\n\t"
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
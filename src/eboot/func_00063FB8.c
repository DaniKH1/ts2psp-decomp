/**
 * The Sims 2 PSP - func_00063FB8 (0x00063FB8, 0x40 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     lui   $a1, %hi(str_SetDepressed_onUpdate)
 *     ori   $a0, $zero, 0x1C
 *     sw    $ra, 0x14($sp)
 *     jal   func_00056B9C
 *       addiu $a1, $a1, %lo(str_SetDepressed_onUpdate)
 *     jal   func_00085A30
 *       or    $a0, $s0, $zero
 *     jal   func_00056BA4
 *       ori   $a0, $zero, 0x1C
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Registers the script callback `"SetDepressed_onUpdate"`, runs
 * `func_00085A30(this)`, and pops the registration again.
 *
 * **Byte for byte this is `func_00063EB0` with one word changed** - the
 * `addiu` that materialises the string - and it sits in the same 0xA8
 * class record's seventh entry.  `func_00063EB0` registers
 * `"GoToBathroom_onUpdate"`, `func_000640C0` registers
 * `"SetRunning_onUpdate"` and `func_00064930` registers
 * `"ReturnToDefaultIdle_onUpdate"`.
 *
 * **Fourteen identical words differing in one name is the module's
 * clearest evidence that these are generated from a table.**  The
 * scripting layer owns a list of behaviours; each one gets a C++ method
 * that registers its own name around the common update.  The names are
 * the *only* per-class information in these methods, which is why the
 * class record's seventh slot exists at all.
 *
 * The name is a plain C string in `.rodata` and is passed by address,
 * so the script layer looks names up by string rather than by an
 * interned id - **and the pop at the end receives only the token
 * `0x1C`, so the matching registration must be tracked by the callee.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00063FB8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a1, %%hi(str_SetDepressed_onUpdate)\n\t"
        "ori   $a0, $zero, 0x1C\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_00056B9C\n\t"
        "addiu $a1, $a1, %%lo(str_SetDepressed_onUpdate)\n\t"
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
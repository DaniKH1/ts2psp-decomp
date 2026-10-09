/**
 * The Sims 2 PSP - func_000BE0E8 (0x000BE0E8, 0x50 bytes)
 *
 * Slot +0x014 of `sym_001EAF88`.  **19 of its 20 words are identical to
 * `func_000BD6D4`**, which fills the same slot in the first vtable;
 * the single word that differs is the `jal` at word 11, here
 * `func_0019EB80` against `func_0019E4F8` there.
 *
 * **+0x014 is a one-word template across the family** - nine of the ten
 * siblings differ from the first in exactly one instruction, where the
 * constructors of slot +0x0C needed two.  So this slot varies less than
 * the constructors do, and what varies is narrower: there is no
 * per-class vtable pointer here, only the callee.
 *
 * **This is the seventh class**, the first of the three that override
 * slot +0x01C with `func_000BE138`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE0E8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s1, $a0, $zero\n\t"
        "bnez  $v0, .Leboot_000BE114\n\t"
        "nop\n\t"
        "b     .Leboot_000BE124\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BE114:\n\t"
        "jal   func_0019EB80\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "sw    $v0, 0x24($s1)\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".Leboot_000BE124:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
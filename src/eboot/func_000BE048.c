/**
 * The Sims 2 PSP - func_000BE048 (0x000BE048, 0xA0 bytes)
 *
 * Slot +0x0C of `sym_001EAF88` - the seventh of the eleven sibling
 * constructors.  **38 of its 40 words are identical to
 * `func_000BD634`**, the first; only two differ:
 *
 *   word 8   the vtable it installs, here `sym_001EAF88`
 *   word 12  the callee, here `func_0019EB18`
 *
 * See `func_000BD7A8` for the template and `func_000BD634` for what the
 * other 38 words are: four globals stored into field 0x18 in sequence
 * with the last three dead, three `beqz $s1` that never branch, the
 * base-constructor call to `func_000B9C88`, and a bit-0 test on the
 * caller's second argument that decides whether to call
 * `func_0012771C`.
 *
 * **This class is one of the three that overrides slot +0x01C** (with
 * `func_000BE138`), so the pair of overrides that makes it not merely
 * re-declare the interface starts here.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE048(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_000BE0D4\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001EAF88)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lw    $a0, 0x0($s1)\n\t"
        "lw    $a1, 0x24($s1)\n\t"
        "jal   func_0019EB18\n\t"
        "lw    $a0, 0x40($a0)\n\t"
        "beqz  $s1, .Leboot_000BE0C4\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9E30)\n\t"
        "beqz  $s1, .Leboot_000BE0C0\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9478)\n\t"
        "beqz  $s1, .Leboot_000BE0C0\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9300)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        ".Leboot_000BE0C0:\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        ".Leboot_000BE0C4:\n\t"
        "beqz  $a0, .Leboot_000BE0D4\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_000BE0D4:\n\t"
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
/**
 * The Sims 2 PSP - func_000BD7A8 (0x000BD7A8, 0xA0 bytes)
 *
 * Slot +0x0C of `sym_001EAB50` - the second of the eleven sibling
 * constructors.  **38 of its 40 words are identical to
 * `func_000BD634`**, the first; only two differ:
 *
 *   word 8   the vtable it installs, here `sym_001EAB50`
 *   word 12  the callee, here `func_0019E590` instead of
 *            `func_0019E490`
 *
 * **The +0x0C slot of the eleven vtables is a template, and the two
 * variable words are the whole of the variation**: each class installs
 * its own vtable and then calls its own function at word 12.  Across
 * all nine siblings measured, those two words are the only difference -
 * same dead stores, same dead branches, same flag test, same layout.
 *
 * The shared material is the part `func_000BD634`'s comment documents:
 * four globals stored into field 0x18 in sequence (the last three of
 * them overwritten and therefore dead), three `beqz $s1` that never
 * branch, a base-constructor call to `func_000B9C88`, and a bit-0 test
 * on the caller's second argument that decides whether to call
 * `func_0012771C`.
 *
 * The three labels follow the pattern found in `func_000BD634`: two of
 * them sit **between two adjacent instructions** around the `andi` and
 * the `beqz` that test bit 0, and the third is the epilogue.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD7A8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_000BD834\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001EAB50)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lw    $a0, 0x0($s1)\n\t"
        "lw    $a1, 0x24($s1)\n\t"
        "jal   func_0019E590\n\t"
        "lw    $a0, 0x40($a0)\n\t"
        "beqz  $s1, .Leboot_000BD824\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9E30)\n\t"
        "beqz  $s1, .Leboot_000BD820\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9478)\n\t"
        "beqz  $s1, .Leboot_000BD820\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9300)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        ".Leboot_000BD820:\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        ".Leboot_000BD824:\n\t"
        "beqz  $a0, .Leboot_000BD834\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_000BD834:\n\t"
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
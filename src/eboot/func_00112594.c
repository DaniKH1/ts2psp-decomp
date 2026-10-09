/**
 * The Sims 2 PSP - func_00112594 (0x00112594, 0x34 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a3, 0x0($a1)
 *     ori   $t0, $zero, 0x4
 *     sw    $ra, 0x10($sp)
 *     bne   $a3, $t0, .Leboot_001125B0
 *       nop
 *     or    $a1, $a2, $zero
 *   .Leboot_001125B0:
 *     lui   $a2, %hi(str_concatenate)
 *     jal   func_001124A4
 *       addiu $a2, $a2, %lo(str_concatenate)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Registers the operation name "concatenate" with func_001124A4, and
 * when the incoming descriptor at 0x0($a1) is not already 4 it first
 * replaces $a1 with $a2.
 *
 * **4 is a slot id, not a length or a type tag.**  It is loaded into
 * `$t0` purely to be compared against, so the branch is a
 * "is this descriptor already ours" test: the register exists only to
 * hold a constant for one `bne`.  $a0 is never read.
 *
 * The `or $a1, $a2, $zero` in the delay slot runs *after* the `bne`
 * decides not to branch, so the substitution happens only on that path.
 */
#include "types.h"

__attribute__((noreturn)) void func_00112594(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a3, 0x0($a1)\n\t"
        "ori   $t0, $zero, 0x4\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "bne   $a3, $t0, .Leboot_001125B0\n\t"
        "nop\n\t"
        "or    $a1, $a2, $zero\n\t"
        ".Leboot_001125B0:\n\t"
        "lui   $a2, %%hi(str_concatenate)\n\t"
        "jal   func_001124A4\n\t"
        "addiu $a2, $a2, %%lo(str_concatenate)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
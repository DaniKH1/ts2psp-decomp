/**
 * The Sims 2 PSP - func_001277FC (0x001277FC, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     beqz  $a0, .Leboot_00127814
 *       nop
 *     jal   func_00127770
 *       lw    $a0, -0x4($a0)
 *   .Leboot_00127814:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Null-safe dispatch: if $a0 is null it returns immediately with $v0
 * untouched, otherwise it loads the word at -4($a0) - the slot just
 * before the object - and passes *that* to func_00127770.
 *
 * **`-4($a0)` is a vtable or a header field.**  The address handed on
 * is not the object but the word preceding it, which is the classic
 * C++ `this - 1` adjustment for a multiple-inheritance base or a
 * virtual dispatch through a leading tag.  The load is in the delay
 * slot, so the null check has already passed before it reads.
 */
#include "types.h"

__attribute__((noreturn)) void func_001277FC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "beqz  $a0, .Leboot_00127814\n\t"
        "nop\n\t"
        "jal   func_00127770\n\t"
        "lw    $a0, -0x4($a0)\n\t"
        ".Leboot_00127814:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_0019B9B8 (0x0019B9B8, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     andi  $a3, $a2, 0xFF
 *     lui   $a2, %hi(D_2161756C)
 *     sw    $ra, 0x10($sp)
 *     jal   func_000D90F4
 *       addiu $a2, $a2, %lo(D_2161756C)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Instruction-for-instruction the same shape as func_0016F674, with
 * one difference: the tag is `D_2161756C`.
 *
 * **The name reads "!vlab", not "lab!".**  The file holds the bytes
 * 21 76 6C 61, and the chunk tags are matched in the order the bytes
 * appear, so a four-character code here starts with `!` (0x21) and
 * ends with `a`.  A label-looking tag is the sort of thing a reader
 * will flip, so the byte order is worth stating.
 *
 * Between them these two functions show the registration call is
 * boilerplate: **same callee, same masking of the third argument into
 * $a3, only the tag differs.**  Any further "surf" or "!vlab" entries
 * elsewhere are the same idiom.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019B9B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "andi  $a3, $a2, 0xFF\n\t"
        "lui   $a2, %%hi(D_2161756C)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_000D90F4\n\t"
        "addiu $a2, $a2, %%lo(D_2161756C)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
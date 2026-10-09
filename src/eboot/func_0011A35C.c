/**
 * The Sims 2 PSP - func_0011A35C (0x0011A35C, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x10($a0)
 *     lw    $a3, 0x8($a1)
 *     lw    $a1, 0x0($a1)
 *     sll   $a2, $a3, 2
 *     sw    $ra, 0x10($sp)
 *     jal   func_00116264
 *       or    $a3, $zero, $zero
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Forwards a struct pointer's two halves plus a count to func_00116264.
 *
 * **`0x10($a0)` is a pointer to a two-word header**, and this function
 * reads both words - data at 0x0 and a length at 0x8 - before the call.
 * **Both loads reuse `$a1`**, so the pointer is consumed: after the
 * sequence `$a1` is the data and `$a3` is the count.
 *
 * **`sll $a2, $a3, 2` scales the count by 4, so the count is a word
 * count and func_00116264 is being asked for a byte length.**  That is
 * the only arithmetic in the function, and it is the whole reason this
 * is a function at all rather than a direct call site.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011A35C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x10($a0)\n\t"
        "lw    $a3, 0x8($a1)\n\t"
        "lw    $a1, 0x0($a1)\n\t"
        "sll   $a2, $a3, 2\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00116264\n\t"
        "or    $a3, $zero, $zero\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
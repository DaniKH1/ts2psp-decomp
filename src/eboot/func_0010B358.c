/**
 * The Sims 2 PSP - func_0010B358 (0x0010B358, 0x1C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_0010B28C
 *       ori   $a1, $zero, 0x1
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Forwards its single argument to func_0010B28C with a constant 1 in
 * $a1, and returns that function's result.  The 1 lands in the delay
 * slot, which is the only reason it is not set up earlier.
 */
#include "types.h"

__attribute__((noreturn)) void func_0010B358(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_0010B28C\n\t"
        "ori   $a1, $zero, 0x1\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
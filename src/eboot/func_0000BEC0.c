/**
 * The Sims 2 PSP - func_0000BEC0 (0x0000BEC0, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_0000D4DC
 *     nop
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Simple wrapper that calls func_0000D4DC and returns.
 */
#include "types.h"

/* Disable GCC's delay slot filling around the jal. */
__attribute__((noreturn)) void func_0000BEC0(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jal   func_0000D4DC\n\t"
        "nop\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        :
        :
        : "memory");
}
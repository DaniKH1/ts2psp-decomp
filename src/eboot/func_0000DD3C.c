/**
 * The Sims 2 PSP - func_0000DD3C (0x0000DD3C, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_0002BB24
 *     nop
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Simple wrapper that calls func_0002BB24 and returns.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000DD3C(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_0002BB24\n\t"
        "nop\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        :
        :
        : "memory");
}
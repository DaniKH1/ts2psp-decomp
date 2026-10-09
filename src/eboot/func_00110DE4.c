/**
 * The Sims 2 PSP - func_00110DE4 (0x00110DE4, 0x20 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_00112754
 *       nop
 *     or    $v0, $zero, $zero
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Calls func_00112754 with all three arguments already in place and
 * then returns 0, discarding the callee's result.
 *
 * **The return value is hard-coded, not the callee's.**  As in
 * func_0010CD40, which returns 1 the same way: these are wrappers that
 * report a fixed outcome and use the call for its side effect.
 */
#include "types.h"

__attribute__((noreturn)) void func_00110DE4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00112754\n\t"
        "nop\n\t"
        "or    $v0, $zero, $zero\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
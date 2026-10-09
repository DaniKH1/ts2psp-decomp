/**
 * The Sims 2 PSP - func_0010CD40 (0x0010CD40, 0x20 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_001085A0
 *       or    $a0, $a3, $zero
 *     ori   $v0, $zero, 0x1
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Passes a *fourth* argument on as func_001085A0's first, discards
 * whatever that call returns, and returns the constant 1.
 *
 * **The 1 is unconditional.**  func_001085A0's result is never read;
 * the `ori` sits after the call and before the epilogue, so this
 * reports success regardless of what the callee did.  Whether the
 * callee signals failure by trapping or by a return value is not
 * something these bytes say.
 */
#include "types.h"

__attribute__((noreturn)) void func_0010CD40(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_001085A0\n\t"
        "or    $a0, $a3, $zero\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
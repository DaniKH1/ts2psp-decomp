/**
 * The Sims 2 PSP - func_0011BD2C (0x0011BD2C, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     addiu $a1, $sp, 0x10
 *     sw    $ra, 0x14($sp)
 *     jal   func_0011BB60
 *       ori   $a2, $zero, 0x4
 *     lw    $v0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * The mirror image of func_001110ADC: instead of unpacking a pair out
 * of a struct, it hands func_0011BB60 a pointer to one word of stack
 * and reads the result straight back out of it.
 *
 * **$a0 is passed through untouched.**  Nothing copies the incoming
 * argument into the stack slot before the call, so the slot is purely
 * an out-parameter and its prior value is irrelevant - the 0x4 is a
 * flag or count telling the callee what to write there.  Reading
 * 0x10($sp) after the call, rather than using $v0, is what makes this
 * an out-parameter call rather than a normal one.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011BD2C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "addiu $a1, $sp, 0x10\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_0011BB60\n\t"
        "ori   $a2, $zero, 0x4\n\t"
        "lw    $v0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
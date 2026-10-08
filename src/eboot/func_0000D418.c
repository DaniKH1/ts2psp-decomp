/**
 * The Sims 2 PSP - func_0000D418 (0x0000D418, 0x30 bytes)
 *
 * Calls a function pointer from a structure, passing the object pointer
 * as an argument.
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x58($a0)
 *     addiu $a1, $a1, 0x58
 *     lh    $a2, 0x0($a1)
 *     lw    $a1, 0x4($a1)
 *     sw    $ra, 0x10($sp)
 *     jalr  $a1
 *       addu $a0, $a0, $a2
 *     lw    $v0, 0x74($v0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function:
 * 1. Loads a function pointer from a structure at a0+0x58+0x58+0x4
 * 2. Calls it via jalr with a0+a2 as argument
 * 3. Loads a word from v0+0x74 and returns it
 */
#include "types.h"

__attribute__((noreturn)) void func_0000D418(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x58($a0)\n\t"
        "addiu $a1, $a1, 0x58\n\t"
        "lh    $a2, 0x0($a1)\n\t"
        "lw    $a1, 0x4($a1)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jalr  $a1\n\t"
        "addu  $a0, $a0, $a2\n\t"
        "lw    $v0, 0x74($v0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
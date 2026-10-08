/**
 * The Sims 2 PSP - func_000052B4 (0x000052B4, 0x48 bytes)
 *
 * Calls a function pointer from a structure, passing float arguments
 * through the stack.
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a2, 0x0($a0)
 *     lwc1  $f13, 0x0($a1)
 *     addiu $a2, $a2, 0x20
 *     lh    $a3, 0x0($a2)
 *     lwc1  $f14, 0x4($a1)
 *     swc1  $f13, 0x10($sp)
 *     mtc1  $zero, $f12
 *     swc1  $f14, 0x14($sp)
 *     swc1  $f12, 0x18($sp)
 *     lw    $a2, 0x4($a2)
 *     addu  $a0, $a0, $a3
 *     sw    $ra, 0x1C($sp)
 *     jalr  $a2
 *       addiu $a1, $sp, 0x10
 *     lw    $ra, 0x1C($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function:
 * 1. Loads a function pointer from $a0+0x20
 * 2. Loads two floats from $a1 (f13, f14)
 * 3. Loads a half-word from the function pointer structure
 * 4. Prepares arguments on stack (f13, f14, 0.0f)
 * 6. Calls the function pointer via jalr with args on stack
 */
#include "types.h"

__attribute__((noreturn)) void func_000052B4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "addiu $a2, $a2, 0x20\n\t"
        "lh    $a3, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "swc1  $f13, 0x10($sp)\n\t"
        "mtc1  $zero, $f12\n\t"
        "swc1  $f14, 0x14($sp)\n\t"
        "swc1  $f12, 0x18($sp)\n\t"
        "lw    $a2, 0x4($a2)\n\t"
        "addu  $a0, $a0, $a3\n\t"
        "sw    $ra, 0x1C($sp)\n\t"
        "jalr  $a2\n\t"
        "addiu $a1, $sp, 0x10\n\t"
        "lw    $ra, 0x1C($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
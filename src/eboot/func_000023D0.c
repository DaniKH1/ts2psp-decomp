/**
 * The Sims 2 PSP - func_000023D0 (0x000023D0, 0x48 bytes)
 *
 * Compares two pointers and conditionally calls a function, initializing
 * float values on the false path.
 *
 *     addiu $sp, $sp, -0x20
 *     or    $a3, $a2, $zero
 *     lw    $t0, 0xDC($a0)
 *     or    $a2, $a0, $zero
 *     sw    $ra, 0x10($sp)
 *     bne   $a3, $t0, .Leboot_000023FC
 *       or  $a0, $a3, $zero
 *     mtc1  $zero, $f12
 *     swc1  $f12, 0x0($a1)
 *     b     .Leboot_0000240C
 *       swc1 $f12, 0x4($a1)
 *   .Leboot_000023FC:
 *     or    $a3, $a0, $zero
 *     or    $a0, $a2, $zero
 *     jal   func_00000360
 *       or  $a2, $a3, $zero
 *   .Leboot_0000240C:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * If the pointer at a0+0xDC equals a2:
 *   - Call func_00000360(a2)
 * Else:
 *   - Store 0.0f at a1 and a1+4
 */
#include "types.h"

__attribute__((noreturn)) void func_000023D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "or    $a3, $a2, $zero\n\t"
        "lw    $t0, 0xDC($a0)\n\t"
        "or    $a2, $a0, $zero\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "bne   $a3, $t0, .Leboot_000023FC\n\t"
        "or    $a0, $a3, $zero\n\t"
        "mtc1  $zero, $f12\n\t"
        "swc1  $f12, 0x0($a1)\n\t"
        "b     .Leboot_0000240C\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        ".Leboot_000023FC:\n\t"
        "or    $a3, $a0, $zero\n\t"
        "or    $a0, $a2, $zero\n\t"
        "jal   func_00000360\n\t"
        "or    $a2, $a3, $zero\n\t"
        ".Leboot_0000240C:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
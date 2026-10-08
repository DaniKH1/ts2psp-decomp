/**
 * The Sims 2 PSP - func_000012A4 (0x000012A4, 0x34 bytes)
 *
 * Conditionally initializes a float field to 0.0f based on a byte flag.
 *
 *     addiu $sp, $sp, -0x20
 *     lbu   $a1, 0xA8($a0)
 *     sw    $ra, 0x10($sp)
 *     bnez  $a1, .Leboot_000012CC
 *     nop
 *     ori   $a1, $zero, 0x1
 *     mtc1  $zero, $f12
 *     sb    $a1, 0xA8($a0)
 *     jal   func_00000B28
 *       swc1 $f12, 0xAC($a0)
 *   .Leboot_000012CC:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * If the byte at offset 0xA8 of the object is non-zero, it calls
 * func_00000B28 (which does some FP math). Otherwise, it sets the
 * flag at 0xA8 to 1, initializes f12 to 0.0f, stores it at offset 0xAC
 * via func_00000B28, and returns.
 */
#include "types.h"

__attribute__((noreturn)) void func_000012A4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lbu   $a1, 0xA8($a0)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "bnez  $a1, .Leboot_000012CC\n\t"
        "nop\n\t"
        "ori   $a1, $zero, 0x1\n\t"
        "mtc1  $zero, $f12\n\t"
        "sb    $a1, 0xA8($a0)\n\t"
        "jal   func_00000B28\n\t"
        "swc1  $f12, 0xAC($a0)\n\t"
        ".Leboot_000012CC:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_0000F6A8 (0x0000F6A8, 0x28 bytes)
 *
 *     mult  $a1, $a0
 *     mflo  $a0
 *     sra   $v0, $a0, 3
 *     sll   $a1, $v0, 3
 *     slt   $a0, $a1, $a0
 *     beqz  $a0, .Leboot_0000F6C8
 *       nop
 *     addiu $v0, $v0, 1
 *   .Leboot_0000F6C8:
 *     jr    $ra
 *     nop
 *
 * This function multiplies a0 * a1, divides the result by 8 (shift right 3),
 * multiplies back by 8 (shift left 3), and checks if there was a remainder.
 * If there was a remainder (a1 != a0), it increments the quotient by 1.
 * This is essentially a ceiling division by 8.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000F6A8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "mult  $a1, $a0\n\t"
        "mflo  $a0\n\t"
        "sra   $v0, $a0, 3\n\t"
        "sll   $a1, $v0, 3\n\t"
        "slt   $a0, $a1, $a0\n\t"
        "beqz  $a0, .Leboot_0000F6C8\n\t"
        "nop\n\t"
        "addiu $v0, $v0, 1\n\t"
        ".Leboot_0000F6C8:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
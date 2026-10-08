/**
 * The Sims 2 PSP - func_0000F57C (0x0000F57C, 0x2C bytes)
 *
 *     sltiu $a2, $a0, 0x2
 *     bnez  $a2, .Leboot_0000F59C
 *     ori   $a1, $zero, 0x0
 *   .Leboot_0000F588:
 *     sll   $a1, $a1, 1
 *     srl   $a0, $a0, 1
 *     sltiu $a2, $a0, 0x2
 *     beqz  $a2, .Leboot_0000F588
 *       ori   $a1, $a1, 0x1
 *   .Leboot_0000F59C:
 *     addiu $v0, $zero, -0x1
 *     jr    $ra
 *       xor  $v0, $a1, $v0
 *
 * This function computes the number of trailing zero bits in $a0
 * (count trailing zeros) using a loop that shifts right until the
 * value is less than 2. If the original value was less than 2,
 * it returns -1. Otherwise it returns the count of trailing zeros.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000F57C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "sltiu $a2, $a0, 0x2\n\t"
        "bnez  $a2, .Leboot_0000F59C\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        ".Leboot_0000F588:\n\t"
        "sll   $a1, $a1, 1\n\t"
        "srl   $a0, $a0, 1\n\t"
        "sltiu $a2, $a0, 0x2\n\t"
        "beqz  $a2, .Leboot_0000F588\n\t"
        "ori   $a1, $a1, 0x1\n\t"
        ".Leboot_0000F59C:\n\t"
        "addiu $v0, $zero, -0x1\n\t"
        "jr    $ra\n\t"
        "xor   $v0, $a1, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
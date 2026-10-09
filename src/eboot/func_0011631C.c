/**
 * The Sims 2 PSP - func_0011631C (0x0011631C, 0x2C bytes)
 *
 *     sltiu  $a2, $a0, 0x8
 *     bnez   $a2, .Leboot_0011633C
 *       ori    $a1, $zero, 0x0
 *   .Leboot_00116328:
 *     addiu  $a0, $a0, 0x1
 *     srl    $a0, $a0, 1
 *     sltiu  $a2, $a0, 0x8
 *     beqz   $a2, .Leboot_00116328
 *       addiu $a1, $a1, 0x1
 *   .Leboot_0011633C:
 *     sll    $v0, $a1, 3
 *     jr     $ra
 *       or     $v0, $v0, $a0
 *
 * The base-8 logarithm of $a0, computed as
 *
 *     return (log8(n) << 3) | (n / 8^log8(n))
 *
 * **The loop works in floating point, not integer division.**  Each pass
 * divides the argument by 2 and counts the division in $a1; the final
 * `sll 3` restores the base.  Because the quotient is produced by
 * `srl` rather than by a divide, the result is exact only for powers of
 * two times a unit - `n = 10` returns `(1 << 3) | 5`, and the odd part
 * is only guaranteed to be below 8 while the loop condition holds.
 *
 * **`sltiu $a2, $a0, 0x8` is the loop test, and the constant 0x8 in it
 * is the very bound the result is reduced to.**  Both the entry test
 * and every loop iteration use the same test, so a zero or negative
 * input skips the loop and returns $a0 unchanged in the low half with a
 * zero high half.
 *
 * No frame is pushed: the loop is a leaf over its own argument.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011631C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "sltiu  $a2, $a0, 0x8\n\t"
        "bnez   $a2, .Leboot_0011633C\n\t"
        "ori    $a1, $zero, 0x0\n\t"
        ".Leboot_00116328:\n\t"
        "addiu  $a0, $a0, 0x1\n\t"
        "srl    $a0, $a0, 1\n\t"
        "sltiu  $a2, $a0, 0x8\n\t"
        "beqz   $a2, .Leboot_00116328\n\t"
        "addiu  $a1, $a1, 0x1\n\t"
        ".Leboot_0011633C:\n\t"
        "sll    $v0, $a1, 3\n\t"
        "jr     $ra\n\t"
        "or     $v0, $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
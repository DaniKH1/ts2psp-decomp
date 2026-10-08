/**
 * The Sims 2 PSP - func_0008073C (0x0008073C, 0x1C bytes)
 *
 * Loads a word from offset 4 of the argument, masks with 0xF0, shifts right
 * by 4, XORs with 8, subtracts 8, and returns 1 if the result was negative
 * (i.e. the original nibble was < 8), 0 otherwise.
 *
 *     lw   $a0, 0x4($a0)
 *     andi $a0, $a0, 0xF0
 *     srl  $a0, $a0, 4
 *     xori $a0, $a0, 0x8
 *     addiu $v0, $a0, -0x8
 *     jr   $ra
 *     sltu $v0, $zero, $v0
 *
 * **Extracts the high nibble of a byte at offset 4**, XORs with 8 (flips bit
 * 3), subtracts 8, and returns whether it went negative.  This is a branchless
 * comparison: `(nibble ^ 8) < 8` is true exactly when `nibble < 8`.
 *
 * **The delay slot does `sltu $v0, $zero, $v0`** - sets $v0 to 1 if $v0 was
 * negative (sign bit set), 0 otherwise.  Since $v0 is signed, `sltu` with
 * $zero is the standard "is negative" test.
 *
 * No arguments used except the pointer; returns a boolean.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0008073C(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x4(%[p])\n\t"
        "andi %[p], %[p], 0xF0\n\t"
        "srl  %[p], %[p], 4\n\t"
        "xori %[p], %[p], 0x8\n\t"
        "addiu $v0, %[p], -0x8\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltu $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
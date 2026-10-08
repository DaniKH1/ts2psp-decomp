/**
 * The Sims 2 PSP - func_001AF144 (0x001AF144, 0x18 bytes)
 *
 * Bit manipulation: returns ((a0 + a1) - 1) & ~(a1 - 1).
 *
 *     addu  $a0, $a0, $a1       a0 = a0 + a1
 *     addiu $a1, $a1, -0x1      a1 = a1 - 1
 *     addiu $v0, $a0, -0x1      v0 = a0 - 1
 *     not   $a0, $a1            a0 = ~a1
 *     jr    $ra
 *     and   $v0, $v0, $a0       v0 = v0 & a0  (delay slot)
 *
 * **The result is v0 = (a0 - 1) & ~(a1 - 1) where a0 = original_a0 + original_a1.**
 *
 * This is a bitmask operation that clears the low bits corresponding to
 * (a1 - 1). If a1 was a power of two, this aligns the sum down to that
 * boundary. The `not` and `and` in the delay slot is the standard
 * branchless alignment trick.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_001AF144(u32 a0, u32 a1) {
    register u32 r0 asm("$a0") = a0;
    register u32 r1 asm("$a1") = a1;
    __asm__ __volatile__(
        "addu %[r0], %[r0], %[r1]\n\t"
        "addiu %[r1], %[r1], -0x1\n\t"
        "addiu $v0, %[r0], -0x1\n\t"
        "not  %[r0], %[r1]\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "and  $v0, $v0, %[r0]\n\t"
        ".set reorder\n\t"
        : [r0] "+r"(r0), [r1] "+r"(r1)
        :
        : "memory", "$v0");
}
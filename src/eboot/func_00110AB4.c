/**
 * The Sims 2 PSP - func_00110AB4 (0x00110AB4, 0x28 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a3, 0x8($a0)
 *     sll   $a1, $a1, 3
 *     subu  $a1, $a3, $a1
 *     sw    $ra, 0x10($sp)
 *     jal   func_00113458
 *       addiu $a1, $a1, -0x8
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Indexes *backwards* into the array at 0x8($a0): it takes $a1 as an
 * index, scales it by 8, and computes
 *
 *     ptr = 0x8($a0) - (index * 8) - 8
 *
 * The `subu` then the `addiu -8` in the delay slot is one subtraction
 * split across two instructions, so the whole thing is
 *
 *     ptr = base - (index + 1) * 8
 *
 * **The stride of 8 is the same element size func_00110014 pushes**, a
 * word plus a float.  An index of 0 therefore yields the element *one
 * before* the cursor, not the first element - this walks the array
 * from its current end towards its start, which is what a pop loop
 * wants and is why the constant is -8 and not 0.
 */
#include "types.h"

__attribute__((noreturn)) void func_00110AB4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a3, 0x8($a0)\n\t"
        "sll   $a1, $a1, 3\n\t"
        "subu  $a1, $a3, $a1\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00113458\n\t"
        "addiu $a1, $a1, -0x8\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_000014D0 (0x000014D0, 0x3C bytes)
 *
 * Calls a function, checks its return value, and conditionally sets a byte
 * flag on the object.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     lw    $a0, 0x3C($s0)
 *     sw    $ra, 0x14($sp)
 *     jal   func_00096C80
 *       addiu $a0, $a0, 0x3C
 *     sltiu $a0, $v0, 0x1
 *     sb    $a0, 0xB9($s0)
 *     lbu   $v0, 0xB9($s0)
 *     sltiu $v0, $v0, 0x1
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * The function:
 * 1. Saves $s0 and $ra
 * 2. Loads a pointer from object+0x3C
 * 3. Calls func_00096C80 with that pointer+0x3C
 * 4. Sets $a0 = (return < 1) ? 1 : 0
 * 5. Stores the byte at object+0xB9
 * 5. Returns (value == 0) ? 1 : 0 in $v0
 */
#include "types.h"

__attribute__((noreturn)) void func_000014D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x3C($s0)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_00096C80\n\t"
        "addiu $a0, $a0, 0x3C\n\t"
        "sltiu $a0, $v0, 0x1\n\t"
        "sb    $a0, 0xB9($s0)\n\t"
        "lbu   $v0, 0xB9($s0)\n\t"
        "sltiu $v0, $v0, 0x1\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
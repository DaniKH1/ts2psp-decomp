/**
 * The Sims 2 PSP - func_00004F90 (0x00004F90, 0x4C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     ori   $s1, $zero, 0x0
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     jal   elem_operator_new
 *       ori   $a0, $zero, 0x30
 *     or    $s0, $v0, $zero
 *     beqz  $s0, .Leboot_00004FC4
 *       nop
 *     jal   func_000051BC
 *       or  $a0, $s0, $zero
 *     or    $s1, $s0, $zero
 *   .Leboot_00004FC4:
 *     or    $v0, $s1, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function allocates an object via elem_operator_new (size 0x30),
 * then calls func_000051BC on it. If allocation fails (returns 0),
 * it returns 0. Otherwise it calls func_000051BC on the new object
 * and returns its result.
 */
#include "types.h"

__attribute__((noreturn)) void func_00004F90(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "ori   $s1, $zero, 0x0\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "jal   elem_operator_new\n\t"
        "ori   $a0, $zero, 0x30\n\t"
        "or    $s0, $v0, $zero\n\t"
        "beqz  $s0, .Leboot_00004FC4\n\t"
        "nop\n\t"
        "jal   func_000051BC\n\t"
        "or    $a0, $s0, $zero\n\t"
        "or    $s1, $s0, $zero\n\t"
        ".Leboot_00004FC4:\n\t"
        "or    $v0, $s1, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
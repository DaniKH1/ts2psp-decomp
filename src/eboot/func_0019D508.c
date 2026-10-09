/**
 * The Sims 2 PSP - func_0019D508 (0x0019D508, 0x5C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_0019D550
 *       or    $s0, $a1, $zero
 *     lui   $a0, %hi(sym_001EA4B8)
 *     addiu $a0, $a0, %lo(sym_001EA4B8)
 *     sw    $a0, 0x18($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_000B9C88
 *       or    $a1, $zero, $zero
 *     andi  $a0, $s0, 0x1
 *     beqz  $a0, .Leboot_0019D550
 *     nop
 *     jal   func_0012771C
 *       or    $a0, $s1, $zero
 *   .Leboot_0019D550:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * func_0019D4AC with `sym_001EA4B8` in place of `sym_001EA3E8`.  Same
 * vtable offset 0x18, same base constructor, same bit-0 test, same
 * void return.
 *
 * **Three consecutive constructors, identical but for one `lui`.**
 * That is the shape of a class hierarchy in which each derived class
 * overrides nothing in the constructor body - only the vtable changes -
 * so the compiler had no reason to share and emitted three copies.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019D508(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_0019D550\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, %%hi(sym_001EA4B8)\n\t"
        "addiu $a0, $a0, %%lo(sym_001EA4B8)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "beqz  $a0, .Leboot_0019D550\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_0019D550:\n\t"
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
/**
 * The Sims 2 PSP - func_00112464 (0x00112464, 0x40 bytes)
 *
 *     lw    $a2, 0x0($a0)
 *     lw    $a0, 0x4($a0)
 *     sltu  $a3, $a2, $a0
 *     beqz  $a3, .Leboot_00112490
 *       nop
 *   .Leboot_00112478:
 *     beq   $a1, $a2, .Leboot_00112498
 *       nop
 *     addiu $a2, $a2, 0x8
 *     sltu  $a3, $a2, $a0
 *     bnez  $a3, .Leboot_00112478
 *       nop
 *   .Leboot_00112490:
 *     b     .Leboot_0011249C
 *       or    $v0, $zero, $zero
 *   .Leboot_00112498:
 *     ori   $v0, $zero, 0x1
 *   .Leboot_0011249C:
 *     jr    $ra
 *       nop
 *
 * Searches an array of 8-byte elements for a value: reads begin and
 * end out of $a0 (0x0 and 0x4), then walks `$a2` from begin to end in
 * strides of 8 comparing it against $a1.  Returns 1 on a hit, 0
 * otherwise.
 *
 * **The stride of 8 is the third sighting of the element size
 * func_00110014 pushes** - a word plus a float.  This is the fourth
 * function in the module to use it, so the layout is a house
 * convention rather than a local one.
 *
 * The two exits are arranged so `$v0` is written exactly once per path:
 * the not-found path materialises 0 in the delay slot of the branch
 * that jumps past the hit case, and the hit path falls into the `ori`.
 * The empty-range case (`begin == end`) takes the same not-found path
 * without entering the loop, so a zero-length array can never report a
 * hit - **the compare happens after the advance test would have
 * stopped the loop, not before it.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00112464(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a0, 0x4($a0)\n\t"
        "sltu  $a3, $a2, $a0\n\t"
        "beqz  $a3, .Leboot_00112490\n\t"
        "nop\n\t"
        ".Leboot_00112478:\n\t"
        "beq   $a1, $a2, .Leboot_00112498\n\t"
        "nop\n\t"
        "addiu $a2, $a2, 0x8\n\t"
        "sltu  $a3, $a2, $a0\n\t"
        "bnez  $a3, .Leboot_00112478\n\t"
        "nop\n\t"
        ".Leboot_00112490:\n\t"
        "b     .Leboot_0011249C\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_00112498:\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_0011249C:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_000001BC (0x000001BC, 0x48 bytes)
 *
 * Calls updateNodeGraph_03FC, then func_0012DDD8 with a pointer offset
 * from the object.
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $s0, 0x1C($sp)
 *     sw    $s1, 0x20($sp)
 *     or    $s1, $a0, $zero
 *     or    $s0, $a1, $zero
 *     sw    $ra, 0x24($sp)
 *     jal   updateNodeGraph_03FC
 *       or  $a0, $s0, $zero
 *     lw    $a1, 0x8($s0)
 *     or    $a0, $v0, $zero
 *     addiu $a1, $a1, 0x4
 *     jal   func_0012DDD8
 *       or  $a2, $s1, $zero
 *     lw    $s0, 0x1C($sp)
 *     lw    $s1, 0x20($sp)
 *     lw    $ra, 0x24($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x30
 */
#include "types.h"

__attribute__((noreturn)) void func_000001BC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x1C($sp)\n\t"
        "sw    $s1, 0x20($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "or    $s0, $a1, $zero\n\t"
        "sw    $ra, 0x24($sp)\n\t"
        "jal   updateNodeGraph_03FC\n\t"
        "or    $a0, $s0, $zero\n\t"
        "lw    $a1, 0x8($s0)\n\t"
        "or    $a0, $v0, $zero\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "jal   func_0012DDD8\n\t"
        "or    $a2, $s1, $zero\n\t"
        "lw    $s0, 0x1C($sp)\n\t"
        "lw    $s1, 0x20($sp)\n\t"
        "lw    $ra, 0x24($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
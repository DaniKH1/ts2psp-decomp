/**
 * The Sims 2 PSP - func_00000204 (0x00000204, 0xA8 bytes)
 *
 * Copies 16 words (64 bytes) from a source struct to a destination struct,
 * then calls updateNodeGraph_03FC with the second argument.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     sw    $ra, 0x14($sp)
 *     jal   updateNodeGraph_03FC
 *       or    $a0, $a1, $zero
 *     lw    $a0, 0x0($v0)
 *     lw    $a1, 0x4($v0)
 *     lw    $a2, 0x8($v0)
 *     sw    $a0, 0x0($s0)
 *     lw    $a0, 0xC($v0)
 *     sw    $a1, 0x4($s0)
 *     lw    $a1, 0x10($v0)
 *     sw    $a2, 0x8($s0)
 *     lw    $a2, 0x14($v0)
 *     sw    $a0, 0xC($s0)
 *     ... (continues copying 16 words total)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * The function copies 16 consecutive words (0x0 through 0x3C) from the
 * source struct pointed to by $v0 to the destination struct at $s0
 * (which is the original first argument $a0). It also calls
 * updateNodeGraph_03FC with the second argument ($a1) as the first argument.
 */
#include "types.h"

__attribute__((noreturn, optimize("falign-functions=4"))) void func_00000204(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   updateNodeGraph_03FC\n\t"
        "or    $a0, $a1, $zero\n\t"
        "lw    $a0, 0x0($v0)\n\t"
        "lw    $a1, 0x4($v0)\n\t"
        "lw    $a2, 0x8($v0)\n\t"
        "sw    $a0, 0x0($s0)\n\t"
        "lw    $a0, 0xC($v0)\n\t"
        "sw    $a1, 0x4($s0)\n\t"
        "lw    $a1, 0x10($v0)\n\t"
        "sw    $a2, 0x8($s0)\n\t"
        "lw    $a2, 0x14($v0)\n\t"
        "sw    $a0, 0xC($s0)\n\t"
        "lw    $a0, 0x18($v0)\n\t"
        "sw    $a1, 0x10($s0)\n\t"
        "lw    $a1, 0x1C($v0)\n\t"
        "sw    $a2, 0x14($s0)\n\t"
        "lw    $a2, 0x20($v0)\n\t"
        "sw    $a0, 0x18($s0)\n\t"
        "lw    $a0, 0x24($v0)\n\t"
        "sw    $a1, 0x1C($s0)\n\t"
        "lw    $a1, 0x24($v0)\n\t"
        "sw    $a2, 0x20($s0)\n\t"
        "lw    $a2, 0x28($v0)\n\t"
        "sw    $a0, 0x24($s0)\n\t"
        "lw    $a0, 0x2C($v0)\n\t"
        "sw    $a1, 0x28($s0)\n\t"
        "lw    $a1, 0x30($v0)\n\t"
        "sw    $a2, 0x2C($s0)\n\t"
        "lw    $a2, 0x34($v0)\n\t"
        "sw    $a0, 0x30($s0)\n\t"
        "lw    $a0, 0x3C($v0)\n\t"
        "sw    $a1, 0x34($s0)\n\t"
        "lw    $a1, 0x38($v0)\n\t"
        "sw    $a2, 0x38($s0)\n\t"
        "lw    $a2, 0x3C($v0)\n\t"
        "sw    $a0, 0x3C($s0)\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00000F6C (0x00000F6C, 0x68 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lbu   $a1, 0xBA($a0)
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     beqz  $a1, .Leboot_00000FC0
 *     nop
 *   .L00000F84:
 *     jal   func_00000058
 *     nop
 *     lui   $a1, %hi(str_congestionSpeedPercent)
 *     lui   $a2, (0x41C80000 >> 16)
 *     or    $a0, $v0, $zero
 *     mtc1  $a2, $f12
 *     jal   func_000A44D4
 *       addiu $a1, $a1, %lo(str_congestionSpeedPercent)
 *     lwc1  $f12, 0x50($s0)
 *     lui   $a0, (0x42C80000 >> 16)
 *     mul.s $f0, $f12, $f0
 *     mtc1  $a0, $f13
 *     div.s $f0, $f0, $f13
 *     b     .Leboot_00000FC4
 *       nop
 *   .Leboot_00000FC0:
 *     lwc1  $f0, 0x50($s0)
 *   .Leboot_00000FC4:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * This function checks a byte flag at offset 0xBA. If set, it calls
 * func_00000058 and formats a congestion speed percentage string.
 * Otherwise it loads a cached float value.
 */
#include "types.h"

__attribute__((noreturn)) void func_00000F6C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lbu   $a1, 0xBA($a0)\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "beqz  $a1, .Leboot_00000FC0\n\t"
        "or    $s0, $a0, $zero\n\t"
        "jal   func_00000058\n\t"
        "nop\n\t"
        "lui   $a1, %%hi(str_congestionSpeedPercent)\n\t"
        "lui   $a2, (0x41C80000 >> 16)\n\t"
        "or    $a0, $v0, $zero\n\t"
        "mtc1  $a2, $f12\n\t"
        "jal   func_000A44D4\n\t"
        "addiu $a1, $a1, %%lo(str_congestionSpeedPercent)\n\t"
        "lwc1  $f12, 0x50($s0)\n\t"
        "lui   $a0, (0x42C80000 >> 16)\n\t"
        "mul.s $f0, $f12, $f0\n\t"
        "mtc1  $a0, $f13\n\t"
        "div.s $f0, $f0, $f13\n\t"
        "b     .Leboot_00000FC4\n\t"
        "nop\n\t"
        ".Leboot_00000FC0:\n\t"
        "lwc1  $f0, 0x50($s0)\n\t"
        ".Leboot_00000FC4:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
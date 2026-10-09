/**
 * The Sims 2 PSP - func_000B9E2C (0x000B9E2C, 0x15C bytes)
 *
 * The shared virtual method at slot +0x28 of `sym_001EA3E8`.  It opens
 * with the same two-stage interning lookup as func_000B9D3C - the same
 * `bnel` pair, the same sentinel 1, the same `lhu` at +8 - and then
 * diverges: where func_000B9D3C calls a callback per index, this one
 * copies bytes into a buffer and writes floats into an array.
 *
 * **The buffer copy is an overlap-safe `memmove` written by hand.**
 * The sequence at 0x000B9F00 is the standard idiom:
 *
 *     sltiu $a3, $a2, 0x4            ; is the distance below 4?
 *     bnel  $a3, $zero, .L100        ; yes -> copy forwards
 *       or    $a0, $a2, $zero        ; no  -> use the raw length
 *     addu  $a0, $a2, $a0            ; end = src + len
 *     sltu  $a2, $a2, $a0            ; did src + len wrap?
 *     beqz  $a2, .L138
 *       or    $a1, $s5, $zero        ; no -> forwards is safe
 *
 * The `bnel` nullifies the `or`, so the raw length survives only on the
 * short-distance path where forwards would be wrong.  The pointer-arith
 * overflow check via `sltu` is the part that distinguishes this from a
 * naive `memcpy` - **it is written to be correct on wrapped addresses,
 * which is why the destination is set only after the check passes.**
 *
 * `$f20` is saved and restored around the whole body but only ever holds
 * the constant 0.0f, and it is stored into `0x10($sp)` then written out
 * to the float array at `0x8($s0) + $s6`.  **The accumulator is a
 * constant**: the array is being cleared or defaulted, not summed.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B9E2C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x40\n\t"
        "sw    $s0, 0x18($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x14($s0)\n\t"
        "sw    $s1, 0x1C($sp)\n\t"
        "or    $s1, $a1, $zero\n\t"
        "addiu $a0, $a0, 0xC\n\t"
        "lw    $a1, 0x0($a0)\n\t"
        "sw    $s3, 0x24($sp)\n\t"
        "ori   $s3, $zero, 0x0\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "swc1  $f20, 0x14($sp)\n\t"
        "sw    $s2, 0x20($sp)\n\t"
        "sw    $s4, 0x28($sp)\n\t"
        "sw    $s5, 0x2C($sp)\n\t"
        "sw    $s6, 0x30($sp)\n\t"
        "sw    $ra, 0x34($sp)\n\t"
        "bnel  $a1, $a2, .Leboot_000B9E78\n\t"
        "addu  $s3, $a0, $a1\n\t"
        ".Leboot_000B9E78:\n\t"
        "or    $a0, $s3, $zero\n\t"
        "ori   $s3, $zero, 0x0\n\t"
        "bnel  $a0, $zero, .Leboot_000B9E88\n\t"
        "lhu   $s3, 0x8($a0)\n\t"
        ".Leboot_000B9E88:\n\t"
        "jal   func_000BA2D8\n\t"
        "or    $a0, $s0, $zero\n\t"
        "or    $s2, $v0, $zero\n\t"
        "ori   $s4, $zero, 0x0\n\t"
        "slt   $a0, $s4, $s2\n\t"
        "beqz  $a0, .Leboot_000B9F58\n\t"
        "subu  $s3, $s2, $s3\n\t"
        "mtc1  $zero, $f20\n\t"
        "addiu $s5, $sp, 0x10\n\t"
        "ori   $s6, $zero, 0x0\n\t"
        ".Leboot_000B9EB0:\n\t"
        "slt   $a0, $s4, $s3\n\t"
        "beqz  $a0, .Leboot_000B9EE0\n\t"
        "nop\n\t"
        "lw    $a0, 0x10($s0)\n\t"
        "jal   func_00105C60\n\t"
        "or    $a1, $s4, $zero\n\t"
        "lw    $a0, 0x8($v0)\n\t"
        "andi  $a0, $a0, 0x100\n\t"
        "bnez  $a0, .Leboot_000B9EE0\n\t"
        "nop\n\t"
        "b     .Leboot_000B9F48\n\t"
        "nop\n\t"
        ".Leboot_000B9EE0:\n\t"
        "swc1  $f20, 0x10($sp)\n\t"
        "lw    $a2, 0x4($s1)\n\t"
        "lw    $a1, 0x0($s1)\n\t"
        "ori   $a0, $zero, 0x4\n\t"
        "subu  $a2, $a2, $a1\n\t"
        "sltiu $a3, $a2, 0x4\n\t"
        "bnel  $a3, $zero, .Leboot_000B9F00\n\t"
        "or    $a0, $a2, $zero\n\t"
        ".Leboot_000B9F00:\n\t"
        "or    $a2, $a1, $zero\n\t"
        "addu  $a0, $a2, $a0\n\t"
        "sltu  $a2, $a2, $a0\n\t"
        "beqz  $a2, .Leboot_000B9F38\n\t"
        "or    $a1, $s5, $zero\n\t"
        ".Leboot_000B9F14:\n\t"
        "lw    $a2, 0x0($s1)\n\t"
        "addiu $a3, $a2, 0x1\n\t"
        "sw    $a3, 0x0($s1)\n\t"
        "lb    $a2, 0x0($a2)\n\t"
        "sb    $a2, 0x0($a1)\n\t"
        "lw    $a2, 0x0($s1)\n\t"
        "sltu  $a2, $a2, $a0\n\t"
        "bnez  $a2, .Leboot_000B9F14\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        ".Leboot_000B9F38:\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lwc1  $f12, 0x10($sp)\n\t"
        "addu  $a0, $a0, $s6\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        ".Leboot_000B9F48:\n\t"
        "addiu $s4, $s4, 0x1\n\t"
        "slt   $a0, $s4, $s2\n\t"
        "bnez  $a0, .Leboot_000B9EB0\n\t"
        "addiu $s6, $s6, 0x4\n\t"
        ".Leboot_000B9F58:\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        "lwc1  $f20, 0x14($sp)\n\t"
        "lw    $s0, 0x18($sp)\n\t"
        "lw    $s1, 0x1C($sp)\n\t"
        "lw    $s2, 0x20($sp)\n\t"
        "lw    $s3, 0x24($sp)\n\t"
        "lw    $s4, 0x28($sp)\n\t"
        "lw    $s5, 0x2C($sp)\n\t"
        "lw    $s6, 0x30($sp)\n\t"
        "lw    $ra, 0x34($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00000150 (0x00000150, 0x6C bytes)
 *
 *     or    $a3, $a1, $zero
 *     or    $a1, $a2, $zero
 *     lwc1  $f12, 0x0($a3)
 *     lwc1  $f13, 0x0($a1)
 *     lwc1  $f14, 0x4($a3)
 *     lwc1  $f15, 0x4($a1)
 *     mul.s $f12, $f12, $f13
 *     mul.s $f14, $f14, $f15
 *     mtc1  $zero, $f16
 *     add.s $f12, $f12, $f14
 *     c.le.s $f12, $f16
 *     nop
 *     bc1t  .Leboot_000001A4
 *       nop
 *     lwc1  $f12, 0x0($a1)
 *     lwc1  $f13, 0x4($a1)
 *     neg.s $f12, $f12
 *     neg.s $f13, $f13
 *     swc1  $f12, 0x0($a0)
 *     b     .Leboot_000001B4
 *       swc1 $f13, 0x4($a0)
 *   .Leboot_000001A4:
 *     lwc1  $f12, 0x0($a1)
 *     swc1  $f12, 0x0($a0)
 *     lwc1  $f12, 0x4($a1)
 *     swc1  $f12, 0x4($a0)
 *   .Leboot_000001B4:
 *     jr    $ra
 *     nop
 *
 * This function computes the product of two complex numbers (or 2D vectors)
 * represented as pairs of floats at a3 and a1, and stores the result at a0.
 * If the magnitude squared of the second vector is <= 0, it negates the
 * components of the first vector before storing.
 */
#include "types.h"

__attribute__((noreturn)) void func_00000150(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "or    $a3, $a1, $zero\n\t"
        "or    $a1, $a2, $zero\n\t"
        "lwc1  $f12, 0x0($a3)\n\t"
        "lwc1  $f13, 0x0($a1)\n\t"
        "lwc1  $f14, 0x4($a3)\n\t"
        "lwc1  $f15, 0x4($a1)\n\t"
        "mul.s $f12, $f12, $f13\n\t"
        "mul.s $f14, $f14, $f15\n\t"
        "mtc1  $zero, $f16\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "c.le.s $f12, $f16\n\t"
        "nop\n\t"
        "bc1t  .Leboot_000001A4\n\t"
        "nop\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "neg.s $f12, $f12\n\t"
        "neg.s $f13, $f13\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "b     .Leboot_000001B4\n\t"
        "swc1  $f13, 0x4($a0)\n\t"
        ".Leboot_000001A4:\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "lwc1  $f12, 0x4($a1)\n\t"
        "swc1  $f12, 0x4($a0)\n\t"
        ".Leboot_000001B4:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
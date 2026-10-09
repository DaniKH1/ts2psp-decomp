/**
 * The Sims 2 PSP - func_001248D0 (0x001248D0, 0x5C bytes)
 *
 *     lwc1  $f14, 0xC($a0)
 *     lwc1  $f13, 0x10($a0)
 *     add.s $f14, $f14, $f12
 *     ori   $a1, $zero, 0x0
 *     swc1  $f14, 0xC($a0)
 *     c.lt.s $f14, $f13
 *     nop
 *     bc1fl .Leboot_001248F4
 *       ori   $a1, $zero, 0x1
 *   .Leboot_001248F4:
 *     andi  $a1, $a1, 0xFF
 *     beqz  $a1, .Leboot_0012490C
 *     nop
 *     lwc1  $f12, 0x4($a0)
 *     b     .Leboot_00124924
 *       swc1  $f12, 0x0($a0)
 *   .Leboot_0012490C:
 *     div.s $f12, $f12, $f13
 *     lwc1  $f14, 0x8($a0)
 *     lwc1  $f15, 0x0($a0)
 *     mul.s $f12, $f12, $f14
 *     add.s $f12, $f15, $f12
 *     swc1  $f12, 0x0($a0)
 *   .Leboot_00124924:
 *     jr    $ra
 *     nop
 *
 * Advances a cursor at 0xC($a0) by the incoming step and, if it
 * overflows the bound at 0x10($a0), rescales the value at 0x0($a0)
 * back into range.
 *
 * **The five floats are a one-dimensional interval, and the layout
 * says which:**
 *
 *     0x0  current value
 *     0x4  period / full scale   (used as the wrap value)
 *     0x8  scale factor          (multiplied after the divide)
 *     0xC  cursor / accumulator  (the thing being advanced)
 *     0x10 bound
 *
 * **`bc1fl` is the nullifying form and the nullification is what makes
 * the flag correct.**  On the not-taken path the `ori $a1, 1` in the
 * delay slot is *discarded*, so `$a1` stays 0 and the `beqz` sends
 * control to the rescale.  On the taken path `c.lt.s` was false - the
 * cursor reached the bound - and `$a1` is 1, so the wrap branch is
 * skipped.  Read as: **flag is set when no wrap happened.**
 *
 * The rescale is `(value / bound) * scale + value`, with the three
 * field reads staggered around the `div.s` because `$f12` is the only
 * register live across it.
 *
 * **Note the ordering: 0x8 is multiplied *after* the divide, and 0x4 is
 * never used on the wrap path at all** - it only supplies the value
 * copied back on the non-wrap path.  Whether 0x8 is a pixel-per-unit
 * scale or a tick rate is not something these bytes decide.
 */
#include "types.h"

__attribute__((noreturn)) void func_001248D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1  $f14, 0xC($a0)\n\t"
        "lwc1  $f13, 0x10($a0)\n\t"
        "add.s $f14, $f14, $f12\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "swc1  $f14, 0xC($a0)\n\t"
        "c.lt.s $f14, $f13\n\t"
        "nop\n\t"
        "bc1fl .Leboot_001248F4\n\t"
        "ori   $a1, $zero, 0x1\n\t"
        ".Leboot_001248F4:\n\t"
        "andi  $a1, $a1, 0xFF\n\t"
        "beqz  $a1, .Leboot_0012490C\n\t"
        "nop\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "b     .Leboot_00124924\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        ".Leboot_0012490C:\n\t"
        "div.s $f12, $f12, $f13\n\t"
        "lwc1  $f14, 0x8($a0)\n\t"
        "lwc1  $f15, 0x0($a0)\n\t"
        "mul.s $f12, $f12, $f14\n\t"
        "add.s $f12, $f15, $f12\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        ".Leboot_00124924:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
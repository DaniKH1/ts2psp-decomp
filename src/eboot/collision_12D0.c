/**
 * The Sims 2 PSP - collision_12D0 (0x001B1AF0, 0xB4 bytes)
 *
 *     lwc1   $f12, 0x0($a2)
 *     lwc1   $f13, 0x0($a1)
 *     addiu  $t0, $a1, 0xC
 *     addiu  $a3, $a2, 0xC
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl  .Leboot_001B1B10
 *       mov.s $f12, $f13
 *   .Leboot_001B1B10:
 *     swc1   $f12, 0x0($a0)
 *     lwc1   $f12, 0x4($a2)
 *     lwc1   $f13, 0x4($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl  .Leboot_001B1B2C
 *       mov.s $f12, $f13
 *   .Leboot_001B1B2C:
 *     swc1   $f12, 0x4($a0)
 *     lwc1   $f12, 0x8($a2)
 *     lwc1   $f13, 0x8($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl  .Leboot_001B1B48
 *       mov.s $f12, $f13
 *   .Leboot_001B1B48:
 *     swc1   $f12, 0x8($a0)
 *     lwc1   $f12, 0x0($a3)
 *     lwc1   $f13, 0x0($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl  .Leboot_001B1B64
 *       mov.s $f12, $f13
 *   .Leboot_001B1B64:
 *     swc1   $f12, 0xC($a0)
 *     lwc1   $f12, 0x4($a3)
 *     lwc1   $f13, 0x4($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl  .Leboot_001B1B80
 *       mov.s $f12, $f13
 *   .Leboot_001B1B80:
 *     swc1   $f12, 0x10($a0)
 *     lwc1   $f12, 0x8($a3)
 *     lwc1   $f13, 0x8($t0)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl  .Leboot_001B1B9C
 *       mov.s $f12, $f13
 *   .Leboot_001B1B9C:
 *     jr     $ra
 *       swc1  $f12, 0x14($a0)
 *
 * Box union: writes the bounding box of the two 24-byte records at `$a1`
 * and `$a2` into the record at `$a0`.
 *
 * **The result never reads `$a0`.**  Unlike its two-argument neighbour
 * `collision_1384`, which grows a box in place around a point, this one
 * only ever *writes* `0x0`, `0x4`, `0x8`, `0xC`, `0x10` and `0x14` of
 * `$a0` and reads nothing from it.  **So `$a0` may alias either input** -
 * the same pointer passed three times is a legal call, not a special case.
 *
 * **The first three components take the maximum and the last three take
 * the minimum**, and the branch polarity decides that, not the names:
 * `c.lt.s $f13, $f12` compares `$a1` against `$a2`; when `$a1` is the
 * smaller one the `bc1tl` nullifies the `mov.s` and `$a2`'s value
 * survives, so the survivor is always the larger.  The `c.le.s` half is
 * the mirror image under `bc1fl` and keeps the smaller.  Both halves then
 * `swc1` unconditionally, so each component costs five instructions and
 * there is exactly one store per output float.
 *
 * **`addiu $t0, $a1, 0xC` and `addiu $a3, $a2, 0xC` are hoisted to the top
 * and never recomputed**, which is the compiler noticing that the second
 * half needs `+0xC` on both inputs.  **Those two instructions are the
 * only writes to a general-purpose register in the whole function** - the
 * rest of the arithmetic is `$f12`/`$f13` alone, and `$f13` alternates
 * between being the first and the second operand of each compare.  That
 * is a strong hint the original was six inline `fminf`/`fmaxf`-shaped
 * expressions rather than a loop, but the bytes only show the unrolling.
 *
 * Both comparisons are ordered (`c.lt.s`, `c.le.s`), so a NaN in either
 * input makes the comparison false: the maximum half then stores `$a1`'s
 * NaN into `$a0` and the minimum half stores `$a2`'s NaN.  **Whether the
 * engine ever feeds a NaN corner here is not determinable from these
 * bytes.**
 *
 * `.Leboot_001B1B9C` is the return itself, with the final
 * `swc1 $f12, 0x14($a0)` riding in the `jr $ra` delay slot - so the last
 * of the six stores completes after the branch has been taken.
 */
#include "types.h"

__attribute__((noreturn)) void collision_12D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1   $f12, 0x0($a2)\n\t"
        "lwc1   $f13, 0x0($a1)\n\t"
        "addiu  $t0, $a1, 0xC\n\t"
        "addiu  $a3, $a2, 0xC\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1B10\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B10:\n\t"
        "swc1   $f12, 0x0($a0)\n\t"
        "lwc1   $f12, 0x4($a2)\n\t"
        "lwc1   $f13, 0x4($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1B2C\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B2C:\n\t"
        "swc1   $f12, 0x4($a0)\n\t"
        "lwc1   $f12, 0x8($a2)\n\t"
        "lwc1   $f13, 0x8($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1B48\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B48:\n\t"
        "swc1   $f12, 0x8($a0)\n\t"
        "lwc1   $f12, 0x0($a3)\n\t"
        "lwc1   $f13, 0x0($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1B64\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B64:\n\t"
        "swc1   $f12, 0xC($a0)\n\t"
        "lwc1   $f12, 0x4($a3)\n\t"
        "lwc1   $f13, 0x4($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1B80\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B80:\n\t"
        "swc1   $f12, 0x10($a0)\n\t"
        "lwc1   $f12, 0x8($a3)\n\t"
        "lwc1   $f13, 0x8($t0)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1B9C\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1B9C:\n\t"
        "jr     $ra\n\t"
        "swc1   $f12, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
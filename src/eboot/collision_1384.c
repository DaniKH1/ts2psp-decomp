/**
 * The Sims 2 PSP - collision_1384 (0x001B1BA4, 0xAC bytes)
 *
 *     lwc1   $f14, 0x0($a0)
 *     lwc1   $f13, 0x0($a1)
 *     c.lt.s $f13, $f14
 *     nop
 *     bc1tl  .Leboot_001B1BBC
 *       mov.s $f14, $f13
 *   .Leboot_001B1BBC:
 *     swc1   $f14, 0x0($a0)
 *     lwc1   $f13, 0x4($a0)
 *     lwc1   $f14, 0x4($a1)
 *     c.lt.s $f14, $f13
 *     nop
 *     bc1tl  .Leboot_001B1BD8
 *       mov.s $f13, $f14
 *   .Leboot_001B1BD8:
 *     swc1   $f13, 0x4($a0)
 *     lwc1   $f12, 0x8($a0)
 *     lwc1   $f13, 0x8($a1)
 *     c.lt.s $f13, $f12
 *     nop
 *     bc1tl  .Leboot_001B1BF4
 *       mov.s $f12, $f13
 *   .Leboot_001B1BF4:
 *     swc1   $f12, 0x8($a0)
 *     lwc1   $f12, 0xC($a0)
 *     lwc1   $f13, 0x0($a1)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl  .Leboot_001B1C10
 *       mov.s $f12, $f13
 *   .Leboot_001B1C10:
 *     swc1   $f12, 0xC($a0)
 *     lwc1   $f12, 0x10($a0)
 *     lwc1   $f14, 0x4($a1)
 *     c.le.s $f14, $f12
 *     nop
 *     bc1fl  .Leboot_001B1C2C
 *       mov.s $f12, $f14
 *   .Leboot_001B1C2C:
 *     swc1   $f12, 0x10($a0)
 *     lwc1   $f12, 0x14($a0)
 *     lwc1   $f13, 0x8($a1)
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl  .Leboot_001B1C48
 *       mov.s $f12, $f13
 *   .Leboot_001B1C48:
 *     jr     $ra
 *       swc1  $f12, 0x14($a0)
 *
 * Grows the 24-byte record at `$a0` outward so that it encloses the 12-byte
 * vector at `$a1`, then returns void.
 *
 * **The record is two triples of floats and the vector is one.**  `$a0` is
 * read at `0x0`, `0x4`, `0x8`, `0xC`, `0x10` and `0x14`; `$a1` only ever
 * at `0x0`, `0x4` and `0x8`.  So this is a box being made to contain a
 * point: **six floats on the left, three on the right, and every output
 * float is read back out of `$a0` and written straight back**, so the
 * operation is in-place and there is no second argument to clobber.
 *
 * **The two halves move in opposite directions, and the bytes settle which
 * is which.**  The first three components use `c.lt.s` with `bc1tl`; if
 * `b[i] < a[i]` the delay slot is nullified and `a[i]` is kept, otherwise
 * `a[i]` becomes `b[i]`.  Since `a[i]` is the one kept when it is *larger*,
 * **offsets `0x0`, `0x4`, `0x8` are raised to the maximum.**  The last
 * three use `c.le.s` with `bc1fl`: if `b[i] <= a[i]` the delay slot runs
 * and `a[i]` becomes `b[i]`; otherwise it is nullified and `a[i]` is kept -
 * and again the one kept is the *smaller*.  **Offsets `0xC`, `0x10` and
 * `0x14` are lowered to the minimum.**  That is exactly "grow the box so it
 * contains the point", with the high corner first and the low corner
 * second in memory.
 *
 * **Every branch here is a nullifying form, `bc1tl` and `bc1fl`, and the
 * `mov.s` in the delay slot is the conditional part of the operation.**
 * This is what lets the store be unconditional: the branch decides whether
 * the register still holds the old value or the new one, and the single
 * `swc1` after the label does the write either way.  Six branches, six
 * unconditional stores, no second copy of the store code.
 *
 * **The `l` in `bc1tl`/`bc1fl` is the branch-likely hint, and it is what
 * the encoding actually records** - the words are `0x45030001` and
 * `0x45020001`.  On this hardware the hint has no architectural effect on
 * a nullifying branch, but it is not padding: it distinguishes these from
 * the non-nullifying `bc1t`/`bc1f`, and writing `bc1t` here would drop the
 * `mov.s` on the taken path and produce the wrong result.
 *
 * **The last store is in the `jr $ra` delay slot**, so `.Leboot_001B1C48`
 * has to land on the return, not on a separate epilogue.  The five
 * intermediate labels each land on the `swc1` that completes their pair,
 * which is why the branch target is `pc + 8` and not `pc + 4`.
 *
 * `c.lt.s`/`c.le.s` are ordered, so a NaN in either operand reads as
 * false; on the first three components that stores `$a1`'s value into
 * `$a0`, and on the last three it stores `$a0`'s own value back.  The
 * bytes do not say whether NaNs reach here.
 */
#include "types.h"

__attribute__((noreturn)) void collision_1384(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lwc1   $f14, 0x0($a0)\n\t"
        "lwc1   $f13, 0x0($a1)\n\t"
        "c.lt.s $f13, $f14\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1BBC\n\t"
        "mov.s  $f14, $f13\n\t"
        ".Leboot_001B1BBC:\n\t"
        "swc1   $f14, 0x0($a0)\n\t"
        "lwc1   $f13, 0x4($a0)\n\t"
        "lwc1   $f14, 0x4($a1)\n\t"
        "c.lt.s $f14, $f13\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1BD8\n\t"
        "mov.s  $f13, $f14\n\t"
        ".Leboot_001B1BD8:\n\t"
        "swc1   $f13, 0x4($a0)\n\t"
        "lwc1   $f12, 0x8($a0)\n\t"
        "lwc1   $f13, 0x8($a1)\n\t"
        "c.lt.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1tl  .Leboot_001B1BF4\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1BF4:\n\t"
        "swc1   $f12, 0x8($a0)\n\t"
        "lwc1   $f12, 0xC($a0)\n\t"
        "lwc1   $f13, 0x0($a1)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1C10\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1C10:\n\t"
        "swc1   $f12, 0xC($a0)\n\t"
        "lwc1   $f12, 0x10($a0)\n\t"
        "lwc1   $f14, 0x4($a1)\n\t"
        "c.le.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1C2C\n\t"
        "mov.s  $f12, $f14\n\t"
        ".Leboot_001B1C2C:\n\t"
        "swc1   $f12, 0x10($a0)\n\t"
        "lwc1   $f12, 0x14($a0)\n\t"
        "lwc1   $f13, 0x8($a1)\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl  .Leboot_001B1C48\n\t"
        "mov.s  $f12, $f13\n\t"
        ".Leboot_001B1C48:\n\t"
        "jr     $ra\n\t"
        "swc1   $f12, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
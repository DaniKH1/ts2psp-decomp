/**
 * The Sims 2 PSP - collision_1430 (0x001B1C50, 0xA4 bytes)
 *
 *     addiu  $a3, $a1, 0xC
 *     lwc1   $f12, 0x0($a0)
 *     lwc1   $f13, 0x0($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       ori   $t0, $zero, 0x0
 *     addiu  $a2, $a0, 0xC
 *     lwc1   $f12, 0x0($a1)
 *     lwc1   $f13, 0x0($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       nop
 *     lwc1   $f12, 0x4($a0)
 *     lwc1   $f13, 0x4($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       nop
 *     lwc1   $f12, 0x4($a1)
 *     lwc1   $f13, 0x4($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       nop
 *     lwc1   $f12, 0x8($a0)
 *     lwc1   $f13, 0x8($a3)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       nop
 *     lwc1   $f12, 0x8($a1)
 *     lwc1   $f13, 0x8($a2)
 *     c.le.s $f12, $f13
 *     nop
 *     bc1f   .Leboot_001B1CEC
 *       nop
 *     ori    $t0, $zero, 0x1
 *   .Leboot_001B1CEC:
 *     jr     $ra
 *       andi  $v0, $t0, 0xFF
 *
 * Axis-aligned box overlap test: returns 1 when the two boxes intersect,
 * 0 when any of six separating-axis comparisons fails.
 *
 * **Both arguments are 24-byte records read as two triples of floats.**
 * The code hoists `addiu $a3, $a1, 0xC` before the first comparison and
 * `addiu $a2, $a0, 0xC` into the second, then reads offsets `0x0`, `0x4`
 * and `0x8` from `$a0`/`$a2` *and* from `$a3`/`$a1`.  Those are
 * `(Vec3 lo, Vec3 hi)` - **a minimum corner at +0 and a maximum corner at
 * +0xC, which is the canonical 24-byte AABB.**  The six tests are
 *
 *     a.lo.x <= b.hi.x   b.lo.x <= a.hi.x
 *     a.lo.y <= b.hi.y   b.lo.y <= a.hi.y
 *     a.lo.z <= b.hi.z   b.lo.z <= a.hi.z
 *
 * **the textbook six-comparison AABB intersection predicate**, one per
 * axis per direction.  There is no epsilon and no `fabsf`; the
 * comparisons are raw `<=`, so touching or exactly coincident faces count
 * as an overlap.
 *
 * **`c.le.s` is the ordered less-or-equal, so an unordered compare (NaN
 * in either operand) reads as false** and the `bc1f` fires, returning 0.
 * A NaN corner is therefore treated as "disjoint", not as "overlapping" -
 * which is the safe direction for a cull test.
 *
 * **The short-circuit lives in the branch, and the first delay slot is the
 * whole false-result path.**  Every one of the six `bc1f` instructions
 * targets the same `.Leboot_001B1CEC`, the shared epilogue; the `false`
 * value is produced by `ori $t0, $zero, 0x0`, which sits in the delay slot
 * of the *first* branch.  The remaining five carry `nop` because `$t0` is
 * already 0 and nothing overwrites it before the epilogue runs.  The
 * `true` path is the single `ori $t0, $zero, 0x1` at the end, reached only
 * when all six comparisons fall through.
 *
 * `andi $v0, $t0, 0xFF` in the return delay slot normalises the result to
 * an unsigned byte, so the C return type is a one-byte boolean.
 *
 * **The loads are interleaved rather than grouped by operand.**  The
 * first pair (a.lo.x vs b.hi.x) is immediately followed by the *second*
 * axis's first comparison, so the compiler scheduled
 * `lwc1 $f12, 0x0($a1)` / `lwc1 $f13, 0x0($a2)` into the shadow of the
 * first test's branch.  **This means the comparison order at runtime is
 * x, x, y, y, z, z - not x, y, z, x, y, z** - but since all six must
 * hold, the interleaving has no effect on the result.
 *
 * Whether these boxes come from a broad phase or from a per-triangle
 * cache is not determinable from these bytes.
 */
#include "types.h"

__attribute__((noreturn)) void collision_1430(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu  $a3, $a1, 0xC\n\t"
        "lwc1   $f12, 0x0($a0)\n\t"
        "lwc1   $f13, 0x0($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "ori    $t0, $zero, 0x0\n\t"
        "addiu  $a2, $a0, 0xC\n\t"
        "lwc1   $f12, 0x0($a1)\n\t"
        "lwc1   $f13, 0x0($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1   $f12, 0x4($a0)\n\t"
        "lwc1   $f13, 0x4($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1   $f12, 0x4($a1)\n\t"
        "lwc1   $f13, 0x4($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1   $f12, 0x8($a0)\n\t"
        "lwc1   $f13, 0x8($a3)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "lwc1   $f12, 0x8($a1)\n\t"
        "lwc1   $f13, 0x8($a2)\n\t"
        "c.le.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   .Leboot_001B1CEC\n\t"
        "nop\n\t"
        "ori    $t0, $zero, 0x1\n\t"
        ".Leboot_001B1CEC:\n\t"
        "jr     $ra\n\t"
        "andi   $v0, $t0, 0xFF\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - collision_011C (0x001B093C, 0x7C bytes)
 *
 *     slt    $t0, $a2, $a3
 *     beqz   $t0, .Leboot_001B09B0
 *       nop
 *     lw     $t0, 0x20($a0)
 *     addiu  $t1, $a0, 0x20
 *     lw     $t2, 0x18($a0)
 *     addiu  $a0, $a0, 0x18
 *     addu   $t0, $t1, $t0
 *     addu   $a0, $a0, $t2
 *     addu   $t1, $a3, $a2
 *   .Leboot_001B0964:
 *     sra    $t1, $t1, 1
 *     sll    $t2, $t1, 2
 *     addu   $t2, $t0, $t2
 *     lbu    $t2, 0x0($t2)
 *     addu   $t3, $t2, $t2
 *     addu   $t2, $t2, $t3
 *     addu   $t2, $t2, $t2
 *     addu   $t2, $a0, $t2
 *     lh     $t2, 0x0($t2)
 *     slt    $t2, $a1, $t2
 *     beqz   $t2, .Leboot_001B09A0
 *       nop
 *     or     $a3, $t1, $zero
 *     b      .Leboot_001B09A8
 *       slt   $t1, $a2, $t1
 *   .Leboot_001B09A0:
 *     addiu  $a2, $t1, 0x1
 *     slt    $t1, $a2, $a3
 *   .Leboot_001B09A8:
 *     bnez   $t1, .Leboot_001B0964
 *       addu  $t1, $a3, $a2
 *   .Leboot_001B09B0:
 *     jr     $ra
 *       or    $v0, $a2, $zero
 *
 * Binary search for the half-open index range `[$a2, $a3)` in a
 * permutation table, returning the surviving bound in `$v0`.
 *
 * **The digit is one byte and the stride is six.**  The per-index byte at
 * `$t0[mid]` (where `$t0 = this + 0x20 + this[0x20]`) is multiplied by 6
 * by three `addu`s - `x2`, `+x` -> 3x, `x2` -> 6x - and then used to index
 * a *signed 16-bit* key at `$a0 + 6*digit` (`$a0 = this + 0x18 +
 * this[0x18]`).  A 256-entry table with six bytes per entry holding a
 * 16-bit key is **a per-digit bucket descriptor, not a key array**: six
 * bytes is exactly enough for a 16-bit value plus room for a 32-bit
 * field, which is why the digit can address it directly.
 *
 * **So this is not looking up the value; it is looking up a bucket
 * boundary.**  The byte at `mid` names a bucket, the bucket's signed
 * 16-bit field is compared against `$a1`, and the comparison narrows the
 * index range - a search over *buckets* whose membership is given by a
 * parallel byte array.  The 256 possible byte values and the signed
 * 16-bit bucket key are what make this a radix-style dispatch.
 *
 * The loop is a textbook binary search on `lo = $a2`, `hi = $a3`,
 * `mid = (lo + hi) >> 1` (`sra`, so the midpoint is arithmetic - the
 * operands are treated as signed).  `slt $t2, $a1, key` and
 * `beqz` means: key <= $a1 collapses the range from above
 * (`hi = mid` via `$a3`), otherwise it collapses from below
 * (`lo = mid + 1`) **and stops setting `$a3`**.
 *
 * **The found case is the interesting one.**  On the `$a1 < key` path it
 * does `or $a3, $t1, $zero` (hi = mid) and then *still* re-tests
 * `slt $t1, $a2, $t1` before branching back to the top; the loop's own
 * back edge carries `addu $t1, $a3, $a2` in its delay slot, recomputing
 * the sum from the live registers rather than from `mid`.  So the
 * recurrence is driven entirely by `$a2`/`$a3` and `mid` is recomputed
 * every iteration - nothing is cached across the branch.
 *
 * The guard `slt $t0, $a2, $a3` on entry is taken *before* either
 * `this[0x18]` or `this[0x20]` is loaded, so **an empty range returns
 * without dereferencing the table at all** - that is what makes this safe
 * to call on a zero-count bucket.  Whether the call is guaranteed to pass
 * a non-empty range is not determinable from these bytes.
 */
#include "types.h"

__attribute__((noreturn)) void collision_011C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "slt    $t0, $a2, $a3\n\t"
        "beqz   $t0, .Leboot_001B09B0\n\t"
        "nop\n\t"
        "lw     $t0, 0x20($a0)\n\t"
        "addiu  $t1, $a0, 0x20\n\t"
        "lw     $t2, 0x18($a0)\n\t"
        "addiu  $a0, $a0, 0x18\n\t"
        "addu   $t0, $t1, $t0\n\t"
        "addu   $a0, $a0, $t2\n\t"
        "addu   $t1, $a3, $a2\n\t"
        ".Leboot_001B0964:\n\t"
        "sra    $t1, $t1, 1\n\t"
        "sll    $t2, $t1, 2\n\t"
        "addu   $t2, $t0, $t2\n\t"
        "lbu    $t2, 0x0($t2)\n\t"
        "addu   $t3, $t2, $t2\n\t"
        "addu   $t2, $t2, $t3\n\t"
        "addu   $t2, $t2, $t2\n\t"
        "addu   $t2, $a0, $t2\n\t"
        "lh     $t2, 0x0($t2)\n\t"
        "slt    $t2, $a1, $t2\n\t"
        "beqz   $t2, .Leboot_001B09A0\n\t"
        "nop\n\t"
        "or     $a3, $t1, $zero\n\t"
        "b      .Leboot_001B09A8\n\t"
        "slt    $t1, $a2, $t1\n\t"
        ".Leboot_001B09A0:\n\t"
        "addiu  $a2, $t1, 0x1\n\t"
        "slt    $t1, $a2, $a3\n\t"
        ".Leboot_001B09A8:\n\t"
        "bnez   $t1, .Leboot_001B0964\n\t"
        "addu   $t1, $a3, $a2\n\t"
        ".Leboot_001B09B0:\n\t"
        "jr     $ra\n\t"
        "or     $v0, $a2, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
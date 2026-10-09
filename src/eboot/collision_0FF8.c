/**
 * The Sims 2 PSP - collision_0FF8 (0x001B1818, 0x218 bytes)
 *
 *     addiu      $sp, $sp, -0x40
 *     lw         $t1, 0x1C($a3)
 *     lw         $t0, 0x18($a3)
 *     lui        $t2, %hi(sym_001DE6B0)
 *     sw         $s5, 0x28($sp)
 *     lw         $s5, %lo(sym_001DE6B4)($t2)
 *     sw         $s4, 0x24($sp)
 *     lw         $s4, %lo(sym_001DE6B0)($t2)
 *     xor        $t2, $t1, $s5
 *     sltiu      $t2, $t2, 0x1
 *     sltu       $t0, $t0, $s4
 *     slt        $t3, $t1, $s5
 *     and        $t0, $t2, $t0
 *     or         $t0, $t0, $t3
 *     sw         $s0, 0x14($sp)
 *     sw         $s1, 0x18($sp)
 *     sw         $s2, 0x1C($sp)
 *     or         $s0, $a0, $zero
 *     andi       $t0, $t0, 0xFF
 *     or         $s1, $a1, $zero
 *     or         $s2, $a2, $zero
 *     sw         $s3, 0x20($sp)
 *     sw         $s6, 0x2C($sp)
 *     sw         $ra, 0x30($sp)
 *     bnez       $t0, .Leboot_001B193C
 *       or        $a0, $a3, $zero
 *   .Leboot_001B1880:
 *     lw         $s3, 0x28($a0)
 *     lw         $s6, 0x2C($a0)
 *     or         $a0, $s1, $zero
 *     jal        collision_1430
 *       or        $a1, $s6, $zero
 *     beqz       $v0, .Leboot_001B18F0
 *       nop
 *     or         $a0, $s1, $zero
 *     jal        collision_1430
 *       or        $a1, $s3, $zero
 *     beqz       $v0, .Leboot_001B18C8
 *       or        $a0, $s6, $zero
 *     or         $a0, $s0, $zero
 *     or         $a1, $s1, $zero
 *     or         $a2, $s2, $zero
 *     jal        collision_0FF8
 *       or        $a3, $s3, $zero
 *     or         $a0, $s6, $zero
 *   .Leboot_001B18C8:
 *     lw         $a3, 0x1C($a0)
 *     lw         $a2, 0x18($a0)
 *     xor        $a1, $a3, $s5
 *     sltiu      $a1, $a1, 0x1
 *     sltu       $a2, $a2, $s4
 *     slt        $t0, $a3, $s5
 *     and        $a1, $a1, $a2
 *     or         $s6, $a1, $t0
 *     b          .Leboot_001B1934
 *       andi      $s6, $s6, 0xFF
 *   .Leboot_001B18F0:
 *     or         $a0, $s1, $zero
 *     jal        collision_1430
 *       or        $a1, $s3, $zero
 *     beqz       $v0, .Leboot_001B192C
 *       or        $a0, $s3, $zero
 *     lw         $a3, 0x1C($a0)
 *     lw         $a2, 0x18($a0)
 *     xor        $a1, $a3, $s5
 *     sltiu      $a1, $a1, 0x1
 *     sltu       $a2, $a2, $s4
 *     slt        $t0, $a3, $s5
 *     and        $a1, $a1, $a2
 *     or         $s6, $a1, $t0
 *     b          .Leboot_001B1934
 *       andi      $s6, $s6, 0xFF
 *   .Leboot_001B192C:
 *     b          .Leboot_001B1A08
 *       nop
 *   .Leboot_001B1934:
 *     beqz       $s6, .Leboot_001B1880
 *       nop
 *   .Leboot_001B193C:
 *     addiu      $a1, $s0, 0x8
 *     beq        $a0, $a1, .Leboot_001B196C
 *       nop
 *     lw         $a1, 0xC($s2)
 *     or         $s0, $a0, $zero
 *     addiu      $s1, $a1, 0x1
 *     sltu       $a2, $s1, $a1
 *     sll        $s3, $s1, 2
 *     bnez       $a2, .Leboot_001B1974
 *       sll       $a0, $s1, 2
 *     b          .Leboot_001B19B0
 *       nop
 *   .Leboot_001B196C:
 *     b          .Leboot_001B1A08
 *       nop
 *   .Leboot_001B1974:
 *     lw         $a2, 0x4($s2)
 *     or         $a3, $a1, $zero
 *     addu       $a1, $a2, $s3
 *     sll        $s3, $a3, 2
 *     addu       $s3, $a2, $s3
 *     beq        $s3, $a1, .Leboot_001B199C
 *       nop
 *     addiu      $s3, $s3, -0x4
 *   .Leboot_001B1994:
 *     bne        $s3, $a1, .Leboot_001B1994
 *       addiu     $s3, $s3, -0x4
 *   .Leboot_001B199C:
 *     or         $a1, $a0, $zero
 *     jal        func_0012C914
 *       or        $a0, $s2, $zero
 *     b          .Leboot_001B1A08
 *       sw        $s1, 0xC($s2)
 *   .Leboot_001B19B0:
 *     or         $a1, $a0, $zero
 *     jal        func_0012C914
 *       or        $a0, $s2, $zero
 *     srl        $a2, $v0, 2
 *     lw         $a0, 0xC($s2)
 *     sltu       $a3, $s1, $a2
 *     lw         $a1, 0x4($s2)
 *     bnel       $a3, $zero, .Leboot_001B19D4
 *       or        $a2, $s1, $zero
 *   .Leboot_001B19D4:
 *     sll        $a0, $a0, 2
 *     addu       $a3, $a1, $a0
 *     addu       $a0, $a1, $s3
 *     or         $a1, $a3, $zero
 *     beq        $a1, $a0, .Leboot_001B1A04
 *       nop
 *   .Leboot_001B19EC:
 *     or         $a3, $a1, $zero
 *     bnel       $a3, $zero, .Leboot_001B19F8
 *       sw        $s0, 0x0($a3)
 *   .Leboot_001B19F8:
 *     addiu      $a1, $a1, 0x4
 *     bne        $a1, $a0, .Leboot_001B19EC
 *       nop
 *   .Leboot_001B1A04:
 *     sw         $a2, 0xC($s2)
 *   .Leboot_001B1A08:
 *     lw         $s0, 0x14($sp)
 *     lw         $s1, 0x18($sp)
 *     lw         $s2, 0x1C($sp)
 *     lw         $s3, 0x20($sp)
 *     lw         $s4, 0x24($sp)
 *     lw         $s5, 0x28($sp)
 *     lw         $s6, 0x2C($sp)
 *     lw         $ra, 0x30($sp)
 *     jr         $ra
 *       addiu    $sp, $sp, 0x40
 *
 * **This is a recursive spatial-hierarchy walk that fills a vector - i.e. a
 * broad-phase / frustum-culling collector.**  Everything in the bytes fits
 * that reading, and the recursion is explicit (it calls itself by name).
 *
 * Four arguments:
 *
 *     a0 -> $s0 : the node being visited (walked by *reference*)
 *     a1 -> $s1 : the query object, passed unchanged to every test
 *     a2 -> $s2 : the output vector (base at +0x4, count at +0xC)
 *     a3       : the candidate object to test, used before $a0 is needed
 *
 * **Two globals are loaded through a `lui`/`lw(%lo)($t2)` pair**:
 * `sym_001DE6B0` into `$s4` and `sym_001DE6B4` into `$s5` - two *adjacent*
 * words, i.e. a two-element table read as one 64-bit pair, and both are
 * loaded as **data words** (`lw` from their own addresses, not their
 * addresses).  `$s4` is always compared unsigned (`sltu`), `$s5` compared
 * signed (`slt`).
 *
 * The culling predicate, computed three times in three different
 * inlined copies, always over the same pair of object fields `0x18` (lo)
 * and `0x1C` (hi):
 *
 *     pred = (hi == s5) ? (lo <u s4) : (hi <s s5)
 *
 * (`xor`+`sltiu 1` is the equality test, `andi 0xFF` normalises it to a
 * byte-sized bool, and `beqz $s6` at `.Leboot_001B1934` branches on it.)
 * That is the codewarrior shape of a **clamp-to-range test on a single
 * axis**: the `hi == limit` case still tests `lo < min`, so a box sitting
 * exactly on the far plane is accepted only when its `lo` is still inside.
 * Read as culling it says: **`pred` true means "this object is entirely
 * outside on that axis, keep descending"**; false means "keep it".  The
 * bytes do not say *which* axis - only that one 32-bit bound at `0x18` is
 * compared unsigned against `$s4` and one at `0x1C` against `$s5`, which is
 * what a min/max pair of a 1-D bound looks like, not a full 6-component box.
 *
 * **The hierarchy is a binary tree of nodes**: `$a0 + 0x28` (`$s3`) and
 * `$a0 + 0x2C` (`$s6`) are the two child pointers, and `collision_1430`
 * is called as `collision_1430(query, child)` - a **subtree visibility
 * test**.  At `.Leboot_001B1880` both children are tested; the node is
 * descended into **only if both pass**, and the descent into `$s3` is the
 * recursive `jal collision_0FF8`.  Two of the three copies of the predicate
 * are the *children's* own bounds being tested immediately after their
 * `collision_1430` call succeeded, and `.Leboot_001B192C` (both tests
 * failed) simply returns without recording anything - **a rejected subtree
 * contributes nothing to the output**.
 *
 * **Recording** happens at `.Leboot_001B193C`, and it is a plain
 * **`vector.push_back(node)`**:
 *
 *     if (node != s0 + 8) { s0 = node; s2->count += 1; ... }
 *
 * The `s0 + 8` comparison is a **sentinel/terminator check**: the slot
 * immediately after the root header is the "end of list" marker and is never
 * pushed, which is why walking past it just returns.
 *
 * The growth code is the same `base`/`count` vector as `collision_0F08`,
 * and it is again readable in full:
 *
 * - `sltu $a2, $s1, $a1` is the **unsigned-overflow check on `count + 1`**;
 *   if it wraps, the function goes straight to `.Leboot_001B1974`.
 * - `.Leboot_001B1974` calls **`func_0012C914(s2, count * 4)`** and stores
 *   the new count - i.e. **reserve `bytes` for `n` pointers**, the
 *   `n << 2` in `$a0` being the byte size.  The `sll`/`addu`/`beq` before
 *   it only compares the old and new end pointers, the classic
 *   "**loop that runs zero times when nothing changed**".
 * - `.Leboot_001B19B0` is the other side: `func_0012C914(s2, bytes)` again
 *   and **`srl $a2, $v0, 2`** converts the returned size into a capacity in
 *   elements, then `sltu $a3, $s1, $a2` asks whether the new count fits.
 *   The `bnel` on `$a3` is **branch-likely into the fill loop**, and in its
 *   delay slot it sets `a2 = s1`, the new count to store afterwards.  When
 *   the new count does *not* fit, the loop at `.Leboot_001B19EC` runs and its
 *   `bnel $a3, $zero` delay slot does the actual work: **`sw $s0, 0x0($a3)`
 *   writes the node pointer into the newly reserved slots**, advancing by 4
 *   from the old end to the new end.  `bnel` always runs the delay slot, so
 *   the store happens every iteration; only the skip-to-`.Leboot_001B19F8`
 *   is conditional on `$a3` being non-zero.
 * - Both growth arms end at `.Leboot_001B1A04` (`sw $a2, 0xC($s2)`) or the
 *   `.Leboot_001B1A08` epilogue with the count stored in the `b` delay slot.
 *
 * So: **`collision_0FF8(node, query, out_vector, candidate)` walks a binary
 * hierarchy, prunes subtrees whose children fail `collision_1430`, and
 * appends the surviving leaves to `out_vector`, growing that 4-byte-element
 * vector through `func_0012C914` as needed.**  Whether the hierarchy is an
 * octree, a BVH or a visibility-graph subdivision is not determinable from
 * these bytes; the binary-node-plus-two-bound-words shape is all that is
 * actually stated.
 */
#include "types.h"

__attribute__((noreturn)) void collision_0FF8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu      $sp, $sp, -0x40\n\t"
        "lw         $t1, 0x1C($a3)\n\t"
        "lw         $t0, 0x18($a3)\n\t"
        "lui        $t2, %%hi(sym_001DE6B0)\n\t"
        "sw         $s5, 0x28($sp)\n\t"
        "lw         $s5, %%lo(sym_001DE6B4)($t2)\n\t"
        "sw         $s4, 0x24($sp)\n\t"
        "lw         $s4, %%lo(sym_001DE6B0)($t2)\n\t"
        "xor        $t2, $t1, $s5\n\t"
        "sltiu      $t2, $t2, 0x1\n\t"
        "sltu       $t0, $t0, $s4\n\t"
        "slt        $t3, $t1, $s5\n\t"
        "and        $t0, $t2, $t0\n\t"
        "or         $t0, $t0, $t3\n\t"
        "sw         $s0, 0x14($sp)\n\t"
        "sw         $s1, 0x18($sp)\n\t"
        "sw         $s2, 0x1C($sp)\n\t"
        "or         $s0, $a0, $zero\n\t"
        "andi       $t0, $t0, 0xFF\n\t"
        "or         $s1, $a1, $zero\n\t"
        "or         $s2, $a2, $zero\n\t"
        "sw         $s3, 0x20($sp)\n\t"
        "sw         $s6, 0x2C($sp)\n\t"
        "sw         $ra, 0x30($sp)\n\t"
        "bnez       $t0, .Leboot_001B193C\n\t"
        "  or        $a0, $a3, $zero\n\t"
        ".Leboot_001B1880:\n\t"
        "lw         $s3, 0x28($a0)\n\t"
        "lw         $s6, 0x2C($a0)\n\t"
        "or         $a0, $s1, $zero\n\t"
        "jal        collision_1430\n\t"
        "  or        $a1, $s6, $zero\n\t"
        "beqz       $v0, .Leboot_001B18F0\n\t"
        "  nop\n\t"
        "or         $a0, $s1, $zero\n\t"
        "jal        collision_1430\n\t"
        "  or        $a1, $s3, $zero\n\t"
        "beqz       $v0, .Leboot_001B18C8\n\t"
        "  or        $a0, $s6, $zero\n\t"
        "or         $a0, $s0, $zero\n\t"
        "or         $a1, $s1, $zero\n\t"
        "or         $a2, $s2, $zero\n\t"
        "jal        collision_0FF8\n\t"
        "  or        $a3, $s3, $zero\n\t"
        "or         $a0, $s6, $zero\n\t"
        ".Leboot_001B18C8:\n\t"
        "lw         $a3, 0x1C($a0)\n\t"
        "lw         $a2, 0x18($a0)\n\t"
        "xor        $a1, $a3, $s5\n\t"
        "sltiu      $a1, $a1, 0x1\n\t"
        "sltu       $a2, $a2, $s4\n\t"
        "slt        $t0, $a3, $s5\n\t"
        "and        $a1, $a1, $a2\n\t"
        "or         $s6, $a1, $t0\n\t"
        "b          .Leboot_001B1934\n\t"
        "  andi      $s6, $s6, 0xFF\n\t"
        ".Leboot_001B18F0:\n\t"
        "or         $a0, $s1, $zero\n\t"
        "jal        collision_1430\n\t"
        "  or        $a1, $s3, $zero\n\t"
        "beqz       $v0, .Leboot_001B192C\n\t"
        "  or        $a0, $s3, $zero\n\t"
        "lw         $a3, 0x1C($a0)\n\t"
        "lw         $a2, 0x18($a0)\n\t"
        "xor        $a1, $a3, $s5\n\t"
        "sltiu      $a1, $a1, 0x1\n\t"
        "sltu       $a2, $a2, $s4\n\t"
        "slt        $t0, $a3, $s5\n\t"
        "and        $a1, $a1, $a2\n\t"
        "or         $s6, $a1, $t0\n\t"
        "b          .Leboot_001B1934\n\t"
        "  andi      $s6, $s6, 0xFF\n\t"
        ".Leboot_001B192C:\n\t"
        "b          .Leboot_001B1A08\n\t"
        "  nop\n\t"
        ".Leboot_001B1934:\n\t"
        "beqz       $s6, .Leboot_001B1880\n\t"
        "  nop\n\t"
        ".Leboot_001B193C:\n\t"
        "addiu      $a1, $s0, 0x8\n\t"
        "beq        $a0, $a1, .Leboot_001B196C\n\t"
        "  nop\n\t"
        "lw         $a1, 0xC($s2)\n\t"
        "or         $s0, $a0, $zero\n\t"
        "addiu      $s1, $a1, 0x1\n\t"
        "sltu       $a2, $s1, $a1\n\t"
        "sll        $s3, $s1, 2\n\t"
        "bnez       $a2, .Leboot_001B1974\n\t"
        "  sll       $a0, $s1, 2\n\t"
        "b          .Leboot_001B19B0\n\t"
        "  nop\n\t"
        ".Leboot_001B196C:\n\t"
        "b          .Leboot_001B1A08\n\t"
        "  nop\n\t"
        ".Leboot_001B1974:\n\t"
        "lw         $a2, 0x4($s2)\n\t"
        "or         $a3, $a1, $zero\n\t"
        "addu       $a1, $a2, $s3\n\t"
        "sll        $s3, $a3, 2\n\t"
        "addu       $s3, $a2, $s3\n\t"
        "beq        $s3, $a1, .Leboot_001B199C\n\t"
        "  nop\n\t"
        "addiu      $s3, $s3, -0x4\n\t"
        ".Leboot_001B1994:\n\t"
        "bne        $s3, $a1, .Leboot_001B1994\n\t"
        "  addiu     $s3, $s3, -0x4\n\t"
        ".Leboot_001B199C:\n\t"
        "or         $a1, $a0, $zero\n\t"
        "jal        func_0012C914\n\t"
        "  or        $a0, $s2, $zero\n\t"
        "b          .Leboot_001B1A08\n\t"
        "  sw        $s1, 0xC($s2)\n\t"
        ".Leboot_001B19B0:\n\t"
        "or         $a1, $a0, $zero\n\t"
        "jal        func_0012C914\n\t"
        "  or        $a0, $s2, $zero\n\t"
        "srl        $a2, $v0, 2\n\t"
        "lw         $a0, 0xC($s2)\n\t"
        "sltu       $a3, $s1, $a2\n\t"
        "lw         $a1, 0x4($s2)\n\t"
        "bnel       $a3, $zero, .Leboot_001B19D4\n\t"
        "  or        $a2, $s1, $zero\n\t"
        ".Leboot_001B19D4:\n\t"
        "sll        $a0, $a0, 2\n\t"
        "addu       $a3, $a1, $a0\n\t"
        "addu       $a0, $a1, $s3\n\t"
        "or         $a1, $a3, $zero\n\t"
        "beq        $a1, $a0, .Leboot_001B1A04\n\t"
        "  nop\n\t"
        ".Leboot_001B19EC:\n\t"
        "or         $a3, $a1, $zero\n\t"
        "bnel       $a3, $zero, .Leboot_001B19F8\n\t"
        "  sw        $s0, 0x0($a3)\n\t"
        ".Leboot_001B19F8:\n\t"
        "addiu      $a1, $a1, 0x4\n\t"
        "bne        $a1, $a0, .Leboot_001B19EC\n\t"
        "  nop\n\t"
        ".Leboot_001B1A04:\n\t"
        "sw         $a2, 0xC($s2)\n\t"
        ".Leboot_001B1A08:\n\t"
        "lw         $s0, 0x14($sp)\n\t"
        "lw         $s1, 0x18($sp)\n\t"
        "lw         $s2, 0x1C($sp)\n\t"
        "lw         $s3, 0x20($sp)\n\t"
        "lw         $s4, 0x24($sp)\n\t"
        "lw         $s5, 0x28($sp)\n\t"
        "lw         $s6, 0x2C($sp)\n\t"
        "lw         $ra, 0x30($sp)\n\t"
        "jr         $ra\n\t"
        "  addiu    $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
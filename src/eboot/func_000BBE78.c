/**
 * The Sims 2 PSP - func_000BBE78 (0x000BBE78, 0x260 bytes)
 *
 * The +0x30 override of `sym_001EA588`, and the largest of the three.
 * It advances a transform node by a weight and, when the motion leaves
 * the node's interval, pushes the leftover on to its parent and retries
 * at the root.  It is the only one of the three that shares no shape
 * with the others at all.
 *
 * **The node is a floating-point interval with explicit sentinels.**
 * 0x10, 0x14, 0x18 and 0xC are each tested against 0.0f before being
 * trusted, and 0x20 holds the running position.  A field that is
 * *exactly* 0.0f means "unbounded in that direction", so the tests are
 * not bounds checks - they are "is this limit configured at all".
 *
 *     0x0C  lower limit (0.0f = none)    0x10  upper limit (0.0f = none)
 *     0x14  step size   (0.0f = none)    0x18  step size   (0.0f = none)
 *     0x1C  accumulated weight            0x20  position
 *     0x24  propagated parent position    0x28  resolved step (or 0.0f)
 *     0x2C  resolved node value           0x34  dirty byte
 *
 * **Every `c.eq.s $f17, $f16` in the body is that sentinel test**, and
 * they come in mirrored pairs - once testing 0x10 and 0x4, then 0x4
 * and 0x8, then 0x8 and 0xC - so the same three-way resolution runs
 * twice, once for the "too low" direction and once for "too high".
 * `.Leboot_000BBF08` and `.Leboot_000BBF70` are the two copies of that
 * cascade, byte-identical apart from their field offsets.
 *
 * The retry loop at `.Leboot_000BC00C` divides the remaining weight by
 * the step and adds or subtracts it, then clamps: if the parent is
 * already past the limit, the result is the parent's position, not the
 * computed one.  **That is why `.Leboot_000BC008` and `.Leboot_000BC048`
 * are separate labels that only run `mov.s $f13, $f14`** - they are the
 * two "give up and use the parent" arms of the clamp, and neither of
 * them is a shortcut.
 *
 * **`.Leboot_000BC09C` is the second of the two clamp exits, and it sits
 * far from the first.**  The first is `.Leboot_000BC008`; this one is
 * reached from the `bc1t` at 0xBBFB4, which is 0x39 instructions past
 * its own position, and its body is `c.eq.s $f14, $f13` / `nop` /
 * `bc1f .Leboot_000BC0C4` - **the mirror image of the test at
 * `.Leboot_000BBFAC` that branches into it.**  The pair together
 * decides, after the weight has been pushed to the parent, whether the
 * parent's result already satisfies the node or has to be overridden.
 *
 * Note where the label goes: after the bitmap update and the
 * `swc1 $f13, 0x1C($a1)`, not next to the arithmetic that produced
 * `$f13`.  Placing it at the arithmetic instead - the intuitive
 * reading, since the name follows the same address as the second
 * `div.s` - moves it 0x1D instructions and every branch to it by that
 * much.
 *
 * The tail is a sparse bitset update.  `sra $t0, $a1, 5` followed by
 * `sll $t0, $t0, 2` is `(bit >> 3) * 4` - the *word* index in bytes -
 * and `andi $a1, $a1, 0x1F` with `sllv $a1, $t1, $a1` is the bit index
 * within that word.  **`0x1F` rather than 0x3F means 32 bits per word
 * on a 32-bit register**, so this is a hand-rolled bitmap, and the
 * `1 << bit` is computed with a `sllv` against `$t1` rather than a
 * variable shift - because the assembler had no variable-shift form
 * the compiler could use here.
 *
 * `lui $a2, (0x80000000 >> 16)` then `or $a1, $a1, $a2` sets bit 31 of
 * the flags word at 0xC($a0).  **Bit 31 of the flags is the dirty
 * bit**, and it is set unconditionally on every path that reaches the
 * tail.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BBE78(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw        $a2, 0x8($a0)\n\t"
        "mtc1      $zero, $f16\n\t"
        "lwc1      $f12, 0x0($a2)\n\t"
        "c.eq.s    $f12, $f16\n\t"
        "nop\n\t"
        "bc1f      .Leboot_000BBEC8\n\t"
        "nop\n\t"
        "lw        $a3, 0x0($a0)\n\t"
        "lwc1      $f14, 0x20($a2)\n\t"
        "lw        $a3, 0x8($a3)\n\t"
        "lwc1      $f13, 0x1C($a2)\n\t"
        "lwc1      $f15, 0x1C($a3)\n\t"
        "lwc1      $f17, 0x10($a2)\n\t"
        "lui       $a3, (0x3F800000 >> 16)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBED0\n\t"
        "mtc1      $a3, $f12\n\t"
        "b         .Leboot_000BBF38\n\t"
        "nop\n\t"
        ".Leboot_000BBEC8:\n\t"
        "b         .Leboot_000BC0D0\n\t"
        "nop\n\t"
        ".Leboot_000BBED0:\n\t"
        "lwc1      $f17, 0x4($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBEEC\n\t"
        "nop\n\t"
        "b         .Leboot_000BBF2C\n\t"
        "mov.s     $f14, $f12\n\t"
        ".Leboot_000BBEEC:\n\t"
        "lwc1      $f17, 0x8($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBF08\n\t"
        "nop\n\t"
        "b         .Leboot_000BBF2C\n\t"
        "mov.s     $f14, $f16\n\t"
        ".Leboot_000BBF08:\n\t"
        "andi      $a1, $a1, 0x8\n\t"
        "beqz      $a1, .Leboot_000BBF2C\n\t"
        "nop\n\t"
        "lwc1      $f17, 0xC($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBF2C\n\t"
        "nop\n\t"
        "sub.s     $f14, $f12, $f14\n\t"
        ".Leboot_000BBF2C:\n\t"
        "swc1      $f14, 0x20($a2)\n\t"
        "b         .Leboot_000BBFAC\n\t"
        "lw        $a2, 0x8($a0)\n\t"
        ".Leboot_000BBF38:\n\t"
        "c.eq.s    $f14, $f13\n\t"
        "nop\n\t"
        "bc1f      .Leboot_000BBFAC\n\t"
        "nop\n\t"
        "andi      $a1, $a1, 0x8\n\t"
        "beqz      $a1, .Leboot_000BBF70\n\t"
        "nop\n\t"
        "lwc1      $f17, 0xC($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBF70\n\t"
        "nop\n\t"
        "b         .Leboot_000BBFA4\n\t"
        "sub.s     $f14, $f12, $f14\n\t"
        ".Leboot_000BBF70:\n\t"
        "lwc1      $f17, 0x4($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBF8C\n\t"
        "nop\n\t"
        "b         .Leboot_000BBFA4\n\t"
        "mov.s     $f14, $f12\n\t"
        ".Leboot_000BBF8C:\n\t"
        "lwc1      $f17, 0x8($a2)\n\t"
        "c.eq.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BBFA4\n\t"
        "nop\n\t"
        "mov.s     $f14, $f16\n\t"
        ".Leboot_000BBFA4:\n\t"
        "swc1      $f14, 0x20($a2)\n\t"
        "lw        $a2, 0x8($a0)\n\t"
        ".Leboot_000BBFAC:\n\t"
        "c.eq.s    $f14, $f13\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BC09C\n\t"
        "nop\n\t"
        "lui       $a1, (0x3F000000 >> 16)\n\t"
        "mtc1      $a1, $f17\n\t"
        "c.le.s    $f14, $f17\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BC014\n\t"
        "nop\n\t"
        "lwc1      $f17, 0x14($a2)\n\t"
        "c.le.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BC008\n\t"
        "nop\n\t"
        "div.s     $f15, $f15, $f17\n\t"
        "add.s     $f13, $f13, $f15\n\t"
        "c.lt.s    $f14, $f13\n\t"
        "nop\n\t"
        "bc1f      .Leboot_000BC00C\n\t"
        "nop\n\t"
        "b         .Leboot_000BC00C\n\t"
        "mov.s     $f13, $f14\n\t"
        ".Leboot_000BC008:\n\t"
        "mov.s     $f13, $f14\n\t"
        ".Leboot_000BC00C:\n\t"
        "b         .Leboot_000BC050\n\t"
        "lw        $a1, 0xC($a0)\n\t"
        ".Leboot_000BC014:\n\t"
        "lwc1      $f17, 0x18($a2)\n\t"
        "c.le.s    $f17, $f16\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BC048\n\t"
        "nop\n\t"
        "div.s     $f15, $f15, $f17\n\t"
        "sub.s     $f13, $f13, $f15\n\t"
        "c.le.s    $f14, $f13\n\t"
        "nop\n\t"
        "bc1t      .Leboot_000BC04C\n\t"
        "nop\n\t"
        "b         .Leboot_000BC04C\n\t"
        "mov.s     $f13, $f14\n\t"
        ".Leboot_000BC048:\n\t"
        "mov.s     $f13, $f14\n\t"
        ".Leboot_000BC04C:\n\t"
        "lw        $a1, 0xC($a0)\n\t"
        ".Leboot_000BC050:\n\t"
        "lui       $a2, (0x80000000 >> 16)\n\t"
        "or        $a1, $a1, $a2\n\t"
        "lw        $a2, 0x0($a0)\n\t"
        "sw        $a1, 0xC($a0)\n\t"
        "lw        $a1, 0x4($a0)\n\t"
        "lw        $a3, 0x38($a2)\n\t"
        "sra       $t0, $a1, 5\n\t"
        "sll       $t0, $t0, 2\n\t"
        "addu      $a3, $a3, $t0\n\t"
        "lw        $t0, 0x0($a3)\n\t"
        "andi      $a1, $a1, 0x1F\n\t"
        "ori       $t1, $zero, 0x1\n\t"
        "sllv      $a1, $t1, $a1\n\t"
        "or        $a1, $t0, $a1\n\t"
        "sw        $a1, 0x0($a3)\n\t"
        "sb        $zero, 0x34($a2)\n\t"
        "lw        $a1, 0x8($a0)\n\t"
        "swc1      $f13, 0x1C($a1)\n\t"
        "lw        $a2, 0x8($a0)\n\t"
        ".Leboot_000BC09C:\n\t"
        "c.eq.s    $f14, $f13\n\t"
        "nop\n\t"
        "bc1f      .Leboot_000BC0C4\n\t"
        "nop\n\t"
        "swc1      $f14, 0x24($a2)\n\t"
        "lw        $a1, 0x8($a0)\n\t"
        "swc1      $f12, 0x28($a1)\n\t"
        "lw        $a0, 0x8($a0)\n\t"
        "b         .Leboot_000BC0D0\n\t"
        "swc1      $f16, 0x2C($a0)\n\t"
        ".Leboot_000BC0C4:\n\t"
        "swc1      $f16, 0x28($a2)\n\t"
        "lw        $a0, 0x8($a0)\n\t"
        "swc1      $f12, 0x2C($a0)\n\t"
        ".Leboot_000BC0D0:\n\t"
        "jr        $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
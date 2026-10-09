/**
 * The Sims 2 PSP - func_000BFF34 (0x000BFF34, 0x178 bytes)
 *
 * Slot +0x034 of `sym_001EB560` - the eleventh class, and **the only
 * member of this slot that is gated on its arguments at all.**
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a2, 0x4000
 *     sw    $s0, 0x10($sp)
 *     and   $a2, $a1, $a2
 *     or    $s0, $a0, $zero
 *     sw    $s1, 0x14($sp)
 *     sw    $s2, 0x18($sp)
 *     sw    $ra, 0x1C($sp)
 *     beqz  $a2, .Leboot_000BFF98
 *       or    $s1, $a1, $zero
 *     lw    $a0, 0x8($s0)
 *     mtc1  $zero, $f12
 *     lwc1  $f13, 0x4($a0)
 *     c.eq.s $f13, $f12
 *     nop
 *     bc1t  .Leboot_000BFF98
 *       nop
 *     lw    $s2, 0x24($s0)
 *     jal   updateNodeGraph_03FC
 *       or    $a0, $s0, $zero
 *     or    $a0, $s2, $zero
 *     jal   func_000CB344
 *       or    $a1, $v0, $zero
 *     jal   func_000CB644
 *       lw    $a0, 0x24($s0)
 *   .Leboot_000BFF98:
 *     lui   $a0, %hi(D_BFFFFFFF)
 *     addiu $a0, $a0, %lo(D_BFFFFFFF)
 *     and   $a0, $s1, $a0
 *     beqz  $a0, .Leboot_000C0094
 *       nop
 *     ... the copies
 *   .Leboot_000C0094:
 *     lw    $s0, 0x10($sp)
 *     ...
 *
 * **Two gates, and the first one does not skip the second.**  Bit
 * 0x40000000 of the second argument is tested first and, when clear,
 * branches to `.Leboot_000BFF98` - which is the *second* gate, not the
 * epilogue.  **So a caller without that bit skips the three calls and
 * still runs everything below.**
 *
 * **The float test uses `bc1t` with a `nop` in its delay slot, not a
 * nullifying form.**  `c.eq.s $f13, $f12` against `$f12 = 0.0f` asks
 * whether `source->0x4` is zero; if it is, control leaves without
 * running any of the three calls.  **This is the third nullifying-branch
 * idiom in this cluster and the third distinct use**: `bc1tl` with a
 * `mov.s` here would have been the clamp of `func_000BDCF4`, and
 * `bc1fl` below is a third thing again.  The branch suffix is doing the
 * work each time and the three do not mean the same thing.
 *
 * **`0x3F000000` is 0.5f**, so the first copy is a halving:
 * `dest->0x1E4 = source->0x8 * 0.5f`.  The next two are plain float
 * copies, `0xC` to `0x1E8` and `0x10` to `0x1EC`, and the two 12-byte
 * blocks that follow are `0x14` to `0x1F0` and `0x20` to `0x1FC` - the
 * same stride the other members use.
 *
 * **`bc1fl` at the end materialises a boolean, and the nullify bit is
 * the whole of it:**
 *
 *   c.eq.s $f14, $f12          f12 is 0.0f
 *   bc1fl .Leboot_000C008C
 *     ori   $a0, $zero, 0x1
 *   .Leboot_000C008C:
 *   andi  $a0, $a0, 0xFF
 *   sb    $a0, 0x222($a1)
 *
 * When `source->0x30` *is* zero the branch is taken and the `ori` is
 * nullified, leaving `$a0` at the 0 it was set to four instructions
 * earlier; otherwise the `ori` runs and `$a0` becomes 1.  **So the byte
 * at `0x222` is `(source->0x30 != 0.0f)`, and writing it this way costs
 * five instructions instead of a branch.**  Compare the same cluster's
 * `xor` / `sltiu` / `andi` sequence in `func_000BFC2C`, which
 * materialises a pointer equality the long way - **two different ways of
 * writing "is it non-zero" in the same module.**
 *
 * `lwc1 0x2C` is truncated to a float-to-word conversion and stored with
 * `sb` at `0x221`, so **a float becomes a single byte**, and `0x34` is
 * truncated to a word and passed as the integer argument to
 * `func_000CB36C`.
 *
 * **The destination is at least 0x225 bytes**, against 0x130 for
 * `func_000BDF18` and 0x108 for `func_000BE220`.  Three members of one
 * slot now put the same object at three different sizes.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BFF34(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a2, 0x4000\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "and   $a2, $a1, $a2\n\t"
        "or    $s0, $a0, $zero\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        "sw    $ra, 0x1C($sp)\n\t"
        "beqz  $a2, .Leboot_000BFF98\n\t"
        "or    $s1, $a1, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "lwc1  $f13, 0x4($a0)\n\t"
        "c.eq.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1t  .Leboot_000BFF98\n\t"
        "nop\n\t"
        "lw    $s2, 0x24($s0)\n\t"
        "jal   updateNodeGraph_03FC\n\t"
        "or    $a0, $s0, $zero\n\t"
        "or    $a0, $s2, $zero\n\t"
        "jal   func_000CB344\n\t"
        "or    $a1, $v0, $zero\n\t"
        "jal   func_000CB644\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        ".Leboot_000BFF98:\n\t"
        "lui   $a0, %%hi(D_BFFFFFFF)\n\t"
        "addiu $a0, $a0, %%lo(D_BFFFFFFF)\n\t"
        "and   $a0, $s1, $a0\n\t"
        "beqz  $a0, .Leboot_000C0094\n\t"
        "nop\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x8($a0)\n\t"
        "lui   $a0, 0x3F00\n\t"
        "mtc1  $a0, $f13\n\t"
        "mul.s $f12, $f12, $f13\n\t"
        "swc1  $f12, 0x1E4($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0xC($a0)\n\t"
        "swc1  $f12, 0x1E8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x10($a0)\n\t"
        "swc1  $f12, 0x1EC($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x14\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x1F0\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x20\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "addiu $a1, $a1, 0x1FC\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "sw    $a3, 0x4($a1)\n\t"
        "sw    $a0, 0x8($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x2C($a0)\n\t"
        "trunc.w.s $f12, $f12\n\t"
        "mfc1  $a0, $f12\n\t"
        "sb    $a0, 0x221($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lwc1  $f13, 0x34($a0)\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "trunc.w.s $f13, $f13\n\t"
        "jal   func_000CB36C\n\t"
        "mfc1  $a1, $f13\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lwc1  $f14, 0x30($a0)\n\t"
        "ori   $a0, $zero, 0x0\n\t"
        "c.eq.s $f14, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_000C008C\n\t"
        "ori   $a0, $zero, 0x1\n\t"
        ".Leboot_000C008C:\n\t"
        "andi  $a0, $a0, 0xFF\n\t"
        "sb    $a0, 0x222($a1)\n\t"
        ".Leboot_000C0094:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $s2, 0x18($sp)\n\t"
        "lw    $ra, 0x1C($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
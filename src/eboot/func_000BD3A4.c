/**
 * The Sims 2 PSP - func_000BD3A4 (0x000BD3A4, 0xD4 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $s1, 0x20($sp)
 *     or    $s1, $a0, $zero
 *     lw    $a0, 0x18($s1)
 *     sw    $s0, 0x1C($sp)
 *     or    $s0, $a1, $zero
 *     addiu $a0, $a0, 0xD0
 *     lh    $a1, 0x0($a0)
 *     lw    $a2, 0x4($a0)
 *     sw    $s2, 0x24($sp)
 *     sw    $ra, 0x28($sp)
 *     jalr  $a2
 *       addu  $a0, $s1, $a1
 *     lw    $a0, 0x8($s1)
 *     lwc1  $f13, 0x10($a0)
 *     mtc1  $zero, $f12
 *     or    $s2, $v0, $zero
 *     ori   $a0, $zero, 0x0
 *     c.le.s $f13, $f12
 *     nop
 *     bc1fl .Leboot_000BD3FC
 *       ori   $a0, $zero, 0x1
 *   .Leboot_000BD3FC:
 *     andi  $a0, $a0, 0xFF
 *     sb    $a0, 0xA0($s2)
 *     lui   $a0, 0x4000
 *     and   $a0, $s0, $a0
 *     beqz  $a0, .Leboot_000BD428
 *       nop
 *     jal   updateNodeGraph_03FC
 *       or    $a0, $s1, $zero
 *     or    $a0, $s2, $zero
 *     jal   func_000C30DC
 *       or    $a1, $v0, $zero
 *   .Leboot_000BD428:
 *     andi  $a0, $s0, 0xE
 *     beqz  $a0, .Leboot_000BD460
 *       nop
 *     lw    $a0, 0x8($s1)
 *     addiu $a0, $a0, 0x4
 *     lwc1  $f12, 0x0($a0)
 *     addiu $a1, $sp, 0x10
 *     swc1  $f12, 0x10($sp)
 *     lwc1  $f12, 0x4($a0)
 *     swc1  $f12, 0x14($sp)
 *     lwc1  $f12, 0x8($a0)
 *     or    $a0, $s2, $zero
 *     jal   func_000C2FE4
 *       swc1  $f12, 0x18($sp)
 *   .Leboot_000BD460:
 *     lw    $s0, 0x1C($sp)
 *     lw    $s1, 0x20($sp)
 *     lw    $s2, 0x24($sp)
 *     lw    $ra, 0x28($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x30
 *
 * Slot +0x034 of `sym_001EB2E8` - **the tenth member of this slot, and
 * the one every other member calls.**  `jal func_000BD3A4` is the first
 * thing all ten do; this function is the slot's shared prologue.
 *
 * **And it is the fourth instance of the multiple-inheritance thunk, and
 * the second inlined one:**
 *
 *     lw    $a0, 0x18($s1)
 *     addiu $a0, $a0, 0xD0
 *     lh    $a1, 0x0($a0)
 *     lw    $a2, 0x4($a0)
 *     jalr  $a2
 *       addu  $a0, $s1, $a1
 *
 * **`func_000BE138` already pinned the thunk array at +0xD0 from its own
 * loop, and this function reads +0xD0 from `0x18($s1)` - two independent
 * call sites agreeing on the same offset.**  The entry is 8 bytes: a
 * signed half-word adjustment at +0 and a function pointer at +4, and
 * `$a0` is formed as `this + adjustment` in the delay slot.
 *
 * **The boolean at `0xA0($s2)` uses `c.le.s`, and that is what makes it a
 * sign test rather than a non-zero test.**  `$a0` starts at 0 and the
 * delay slot sets it to 1:
 *
 *   f13 <= 0.0  ->  branch taken,  `ori` nullified, $a0 stays 0
 *   f13 >  0.0  ->  branch clear,  `ori` runs,     $a0 = 1
 *
 * so the byte is `(source->0x10 > 0.0f)`.  `func_000BFF34` writes the
 * same five-instruction shape with `c.eq.s`, which asks about zero rather
 * than about sign - **the two are the same idiom one comparison apart.**
 * With `bc1tl` and a `mov.s` in the delay slot, as in `func_000BDCF4`,
 * the nullify bit instead produces a clamp.
 *
 * **Bit 0x40000000 of the second argument gates a call to
 * `updateNodeGraph_03FC` followed by `func_000C30DC` - and
 * `func_000BFF34`, the eleventh member, gates on that same bit and calls
 * the same `updateNodeGraph_03FC`.**  Two functions in different slots
 * of different vtables keying off one bit of one argument and reaching
 * for the same callee is about as strong a link as bytes alone can give.
 *
 * **Bits 1, 2 and 3 of the same argument gate a three-float struct
 * passed by value.**  `func_000C2FE4` receives `$s2` and a pointer to
 * `$sp + 0x10`, where three floats from `0x4($s1->0x8)` have just been
 * stored - **the `swc1`s go to this frame's own stack, not the caller's,
 * which is what by-value passing of a small struct looks like under this
 * ABI.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD3A4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s1, 0x20($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "lw    $a0, 0x18($s1)\n\t"
        "sw    $s0, 0x1C($sp)\n\t"
        "or    $s0, $a1, $zero\n\t"
        "addiu $a0, $a0, 0xD0\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "sw    $s2, 0x24($sp)\n\t"
        "sw    $ra, 0x28($sp)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s1, $a1\n\t"
        "lw    $a0, 0x8($s1)\n\t"
        "lwc1  $f13, 0x10($a0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "or    $s2, $v0, $zero\n\t"
        "ori   $a0, $zero, 0x0\n\t"
        "c.le.s $f13, $f12\n\t"
        "nop\n\t"
        "bc1fl .Leboot_000BD3FC\n\t"
        "ori   $a0, $zero, 0x1\n\t"
        ".Leboot_000BD3FC:\n\t"
        "andi  $a0, $a0, 0xFF\n\t"
        "sb    $a0, 0xA0($s2)\n\t"
        "lui   $a0, 0x4000\n\t"
        "and   $a0, $s0, $a0\n\t"
        "beqz  $a0, .Leboot_000BD428\n\t"
        "nop\n\t"
        "jal   updateNodeGraph_03FC\n\t"
        "or    $a0, $s1, $zero\n\t"
        "or    $a0, $s2, $zero\n\t"
        "jal   func_000C30DC\n\t"
        "or    $a1, $v0, $zero\n\t"
        ".Leboot_000BD428:\n\t"
        "andi  $a0, $s0, 0xE\n\t"
        "beqz  $a0, .Leboot_000BD460\n\t"
        "nop\n\t"
        "lw    $a0, 0x8($s1)\n\t"
        "addiu $a0, $a0, 0x4\n\t"
        "lwc1  $f12, 0x0($a0)\n\t"
        "addiu $a1, $sp, 0x10\n\t"
        "swc1  $f12, 0x10($sp)\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "swc1  $f12, 0x14($sp)\n\t"
        "lwc1  $f12, 0x8($a0)\n\t"
        "or    $a0, $s2, $zero\n\t"
        "jal   func_000C2FE4\n\t"
        "swc1  $f12, 0x18($sp)\n\t"
        ".Leboot_000BD460:\n\t"
        "lw    $s0, 0x1C($sp)\n\t"
        "lw    $s1, 0x20($sp)\n\t"
        "lw    $s2, 0x24($sp)\n\t"
        "lw    $ra, 0x28($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
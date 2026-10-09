/**
 * The Sims 2 PSP - func_000BE138 (0x000BE138, 0xE8 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $s0, 0x10($sp)
 *     sw    $s1, 0x14($sp)
 *     sw    $s2, 0x18($sp)
 *     sw    $s3, 0x1C($sp)
 *     sw    $s4, 0x20($sp)
 *     sw    $ra, 0x24($sp)
 *     jal   func_000B9D34
 *       or    $s0, $a0, $zero
 *     beqz  $v0, .Leboot_000BE184
 *       nop
 *     lw    $s4, 0x14($s0)
 *     lw    $a0, 0x24($s0)
 *     jal   func_000C4274
 *       lw    $a1, 0x28($s4)
 *     bnez  $v0, .Leboot_000BE18C
 *       nop
 *     b     .Leboot_000BE200
 *       or    $v0, $zero, $zero
 *   .Leboot_000BE184:
 *     b     .Leboot_000BE200
 *       or    $v0, $zero, $zero
 *   .Leboot_000BE18C:
 *     lw    $a0, 0x28($s4)
 *     ori   $s3, $zero, 0x0
 *     slt   $a0, $s3, $a0
 *     beqz  $a0, .Leboot_000BE1FC
 *       addiu $s2, $s4, 0x2C
 *     ori   $s1, $zero, 0x0
 *   .Leboot_000BE1A4:
 *     lw    $a0, 0x2C($s4)
 *     lw    $a1, 0x0($s0)
 *     addu  $a0, $s2, $a0
 *     addu  $a0, $a0, $s1
 *     lw    $a2, 0x0($a0)
 *     or    $a0, $a1, $zero
 *     jal   func_000BA9FC
 *       or    $a1, $a2, $zero
 *     lw    $a0, 0x18($v0)
 *     addiu $a0, $a0, 0xD0
 *     lh    $a1, 0x0($a0)
 *     lw    $a2, 0x4($a0)
 *     jalr  $a2
 *       addu  $a0, $v0, $a1
 *     lw    $a0, 0x24($s0)
 *     jal   func_000C42C8
 *       or    $a1, $v0, $zero
 *     lw    $a0, 0x28($s4)
 *     addiu $s3, $s3, 0x1
 *     slt   $a0, $s3, $a0
 *     bnez  $a0, .Leboot_000BE1A4
 *       addiu $s1, $s1, 0x4
 *   .Leboot_000BE1FC:
 *     ori   $v0, $zero, 0x1
 *   .Leboot_000BE200:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $s2, 0x18($sp)
 *     lw    $s3, 0x1C($sp)
 *     lw    $s4, 0x20($sp)
 *     lw    $ra, 0x24($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x30
 *
 * The only override at slot +0x01C that can answer 0, and the only one
 * with a loop in it.
 *
 * **Three ways to return 0, two of them the same.**  `func_000C4274` is
 * called once on `0x24($s0)` with `0x28($s4)`; if it returns 0 the
 * function returns 0 there and then.  That is the live test.  The other
 * two are the `.Leboot_000BE184` block, which the `beqz $v0` after
 * `func_000B9D34` reaches, and the `bnez`'s own false arm - **and both
 * of those set `$v0` to 0 and branch to the same epilogue, and neither
 * is reachable.**  So the three `or $v0, $zero, $zero` in the listing
 * are one live branch and two dead copies of it.
 *
 * **When `func_000C4274` succeeds it walks a count.**  `s3` indexes
 * `0..0x28($s4) - 1` and `s1` is a byte offset stepping by 4, so the
 * body visits one 4-byte entry of the array at `0x2C($s4)` per
 * iteration and always returns 1 - **the loop cannot change the answer,
 * only the amount of work.**  That is why `.Leboot_000BE1FC` is reached
 * both by `beqz $a0` when the count is 0 and by falling out the bottom
 * of the loop.
 *
 * **The loop contains the only `jalr` in this family of overrides, and
 * it is the multiple-inheritance thunk for the third time in the
 * module.**  `lw $a0, 0x18($v0)` then `addiu $a0, $a0, 0xD0` reaches a
 * table; at that address `lh` reads a **signed half-word** and `lw` at
 * +4 reads a function pointer; the delay slot forms
 * `$a1 = $v0 + adjustment` and calls through.  **Both ends of that
 * idiom are already on record** - `func_0019D11C` reads a
 * `{i16 adjust; void (*fn)()} `entry and adjusts `this` before its
 * `jalr`, and `func_000805D4` returns `this + 8` to hand back a
 * sub-object.  **This is a third instance and the first one inlined**:
 * here the compiler expanded the dispatch into the loop body instead of
 * calling a helper, which is why the adjustment field being a half-word
 * and the entry being 8 bytes are both visible directly.  It also pins
 * the thunk array at **+0xD0** of whatever `0x18` points to.
 *
 * **The `addu $a0, $s2, $a0` followed by `addu $a0, $a0, $s1` is the
 * manual inlining of an array index**: `0x2C($s4)` is loaded as an index,
 * added to the array's own address in `$s2`, and biased by the running
 * byte offset.  Loading an offset rather than a pointer is what makes
 * `addu` (no overflow trap) correct here.
 *
 * Note the loop-back branch keeps `addiu $s1, $s1, 0x4` in its delay
 * slot, so the increment of the byte offset is **load-bearing** and this
 * function also needs `.set noreorder`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE138(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        "sw    $s3, 0x1C($sp)\n\t"
        "sw    $s4, 0x20($sp)\n\t"
        "sw    $ra, 0x24($sp)\n\t"
        "jal   func_000B9D34\n\t"
        "or    $s0, $a0, $zero\n\t"
        "beqz  $v0, .Leboot_000BE184\n\t"
        "nop\n\t"
        "lw    $s4, 0x14($s0)\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "jal   func_000C4274\n\t"
        "lw    $a1, 0x28($s4)\n\t"
        "bnez  $v0, .Leboot_000BE18C\n\t"
        "nop\n\t"
        "b     .Leboot_000BE200\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BE184:\n\t"
        "b     .Leboot_000BE200\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BE18C:\n\t"
        "lw    $a0, 0x28($s4)\n\t"
        "ori   $s3, $zero, 0x0\n\t"
        "slt   $a0, $s3, $a0\n\t"
        "beqz  $a0, .Leboot_000BE1FC\n\t"
        "addiu $s2, $s4, 0x2C\n\t"
        "ori   $s1, $zero, 0x0\n\t"
        ".Leboot_000BE1A4:\n\t"
        "lw    $a0, 0x2C($s4)\n\t"
        "lw    $a1, 0x0($s0)\n\t"
        "addu  $a0, $s2, $a0\n\t"
        "addu  $a0, $a0, $s1\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "or    $a0, $a1, $zero\n\t"
        "jal   func_000BA9FC\n\t"
        "or    $a1, $a2, $zero\n\t"
        "lw    $a0, 0x18($v0)\n\t"
        "addiu $a0, $a0, 0xD0\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $v0, $a1\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "jal   func_000C42C8\n\t"
        "or    $a1, $v0, $zero\n\t"
        "lw    $a0, 0x28($s4)\n\t"
        "addiu $s3, $s3, 0x1\n\t"
        "slt   $a0, $s3, $a0\n\t"
        "bnez  $a0, .Leboot_000BE1A4\n\t"
        "addiu $s1, $s1, 0x4\n\t"
        ".Leboot_000BE1FC:\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_000BE200:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $s2, 0x18($sp)\n\t"
        "lw    $s3, 0x1C($sp)\n\t"
        "lw    $s4, 0x20($sp)\n\t"
        "lw    $ra, 0x24($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
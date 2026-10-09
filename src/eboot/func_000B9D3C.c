/**
 * The Sims 2 PSP - func_000B9D3C (0x000B9D3C, 0xF0 bytes)
 *
 * A shared virtual method (slot +0x20 of `sym_001EA3E8`), walking the
 * symbol/intern table behind `0x14($s0)` and writing 4-byte entries
 * through a caller-supplied callback.
 *
 * The prologue is a two-stage lookup, and **both stages use `bnel`, whose
 * nullification is what encodes the test**:
 *
 *     lw    $a1, 0x0($a0)          ; tag at 0xC of the table header
 *     ori   $a2, $zero, 0x1
 *     bnel  $a1, $a2, .L80         ; if tag != 1, skip
 *       addu  $s2, $a0, $a1        ; else  s2 = header + tag
 *     ...
 *     bnel  $a0, $zero, .L90       ; if pointer != 0, skip
 *       lhu   $s2, 0x8($a0)        ; else  s2 = u16 at +8 of the node
 *
 * **So the constant 1 is a sentinel meaning "not interned yet"** - the
 * usual pattern is a sentinel, then the real index once interned.  The
 * delay-slot instruction is discarded exactly when the branch is taken,
 * so the arithmetic only runs on the path where the sentinel says the
 * value is still missing.
 *
 * The loop then counts `func_000BA2D8`'s result down from the header's
 * u16, and for each index calls `func_001224E0` with a fixed 4 -
 * **`$a2` is the entry size, so `addiu $s5, $s5, 0x4` is the walk
 * stride and the callback receives byte offsets**.
 *
 * `andi $a0, $a0, 0x100` tests a flag word at 0x8 of whatever
 * `func_00105C60` returned; when the bit is set the callback is
 * skipped, which is a "this symbol is filtered out" path.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B9D3C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x14($s0)\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a1, $zero\n\t"
        "addiu $a0, $a0, 0xC\n\t"
        "lw    $a1, 0x0($a0)\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        "ori   $s2, $zero, 0x0\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "sw    $s3, 0x1C($sp)\n\t"
        "sw    $s4, 0x20($sp)\n\t"
        "sw    $s5, 0x24($sp)\n\t"
        "sw    $ra, 0x28($sp)\n\t"
        "bnel  $a1, $a2, .Leboot_000B9D80\n\t"
        "addu  $s2, $a0, $a1\n\t"
        ".Leboot_000B9D80:\n\t"
        "or    $a0, $s2, $zero\n\t"
        "ori   $s2, $zero, 0x0\n\t"
        "bnel  $a0, $zero, .Leboot_000B9D90\n\t"
        "lhu   $s2, 0x8($a0)\n\t"
        ".Leboot_000B9D90:\n\t"
        "or    $s3, $s2, $zero\n\t"
        "jal   func_000BA2D8\n\t"
        "or    $a0, $s0, $zero\n\t"
        "or    $s2, $v0, $zero\n\t"
        "ori   $s4, $zero, 0x0\n\t"
        "slt   $a0, $s4, $s2\n\t"
        "beqz  $a0, .Leboot_000B9E08\n\t"
        "subu  $s3, $s2, $s3\n\t"
        "ori   $s5, $zero, 0x0\n\t"
        ".Leboot_000B9DB4:\n\t"
        "slt   $a0, $s4, $s3\n\t"
        "beqz  $a0, .Leboot_000B9DE4\n\t"
        "nop\n\t"
        "lw    $a0, 0x10($s0)\n\t"
        "jal   func_00105C60\n\t"
        "or    $a1, $s4, $zero\n\t"
        "lw    $a0, 0x8($v0)\n\t"
        "andi  $a0, $a0, 0x100\n\t"
        "bnez  $a0, .Leboot_000B9DE4\n\t"
        "nop\n\t"
        "b     .Leboot_000B9DF8\n\t"
        "nop\n\t"
        ".Leboot_000B9DE4:\n\t"
        "lw    $a1, 0x8($s0)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "addu  $a1, $a1, $s5\n\t"
        "jal   func_001224E0\n\t"
        "ori   $a2, $zero, 0x4\n\t"
        ".Leboot_000B9DF8:\n\t"
        "addiu $s4, $s4, 0x1\n\t"
        "slt   $a0, $s4, $s2\n\t"
        "bnez  $a0, .Leboot_000B9DB4\n\t"
        "addiu $s5, $s5, 0x4\n\t"
        ".Leboot_000B9E08:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $s2, 0x18($sp)\n\t"
        "lw    $s3, 0x1C($sp)\n\t"
        "lw    $s4, 0x20($sp)\n\t"
        "lw    $s5, 0x24($sp)\n\t"
        "lw    $ra, 0x28($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
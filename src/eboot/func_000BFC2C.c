/**
 * The Sims 2 PSP - func_000BFC2C (0x000BFC2C, 0x28C bytes)
 *
 * Slot +0x014 of `sym_001EB560` - the eleventh class, and **the only
 * member of this slot that is not the 20-word template.**  It is 163
 * words where the other ten are 20, and it carries twenty labels to
 * their two.
 *
 * **What it shares with the template is only the opening.**  Ask
 * `func_000A7AC8` whether to proceed, bail out returning 0 if not; then
 * call the per-class function on `0x18($s1)`, store its result into
 * `0x24($s0)` - the same field the whole slot writes - and bail out
 * returning 0 if that is zero too.  Both bails go to the epilogue with
 * `or $v0, $zero, $zero`, so there are two distinct early returns
 * instead of the template's one.
 *
 * **The polarity is inverted, though, and that is decidable.**  The
 * other ten use `bnez $v0` on `func_000A7AC8`'s result; this uses
 * `beqz $v0`.  So the two conventions disagree about what the callee's
 * answer means - "non-zero means go" against "non-zero means stop".
 * Either the callee returns something this class inverts, or the two
 * sites were written against different helpers with the same name in
 * different scopes.  **The bytes cannot say which.**
 *
 * **The body is an inlined 16-bit `memmove`, and the element width is
 * visible everywhere.**  Every copy step is `lhu` / `sh` with both
 * pointers advancing by 2, and every length is divided by two with
 *
 *     sra  $a3, $a2, 1
 *     srl  $a3, $a3, 31
 *     addu $a2, $a2, $a3
 *     sra  $a2, $a2, 1
 *
 * which is **signed** division by two done as an arithmetic shift
 * followed by a borrow from the sign bit - not the `srl $a2, $a2, 1`
 * that an unsigned count would use.  Three separate sites do it this
 * way, so the pointer differences being halved are signed.
 *
 * **The overlap test is a three-instruction pointer comparison:**
 * `xor $a1, $s1, $a0` then `sltiu $a1, $a1, 0x1` then `andi $a1, $a1,
 * 0xFF`.  That is "are these two pointers equal" as a 0-or-1 value, and
 * `beqz` on it skips the whole second phase when they are - **so the
 * expensive half of this function only runs when a copy actually
 * happened.**
 *
 * The copy direction then branches on where the write pointer ended up:
 * `.Leboot_000BFCF0` walks forward while `.Leboot_000BFDB8` walks
 * backward from `$s2`, both bounded by a limit computed as
 * `0x28(base) + count` advanced by `0x24(base) * 2`.  **That fixes the
 * layout of the object at `0x14($s0)`: an element count at 0x24 and a
 * base pointer at 0x28, both 16-bit-strided.**  The same doubling of
 * `0x24($a1)` appears in the constructor group as well, so it is how
 * this class family stores array bounds.
 *
 * **Two calls to `func_00170654`, both `func_00170654(0, 0)`, and both
 * results discarded.**  Immediately before each call the code does
 * `sb $zero` to a stack slot and then `lb`s it straight back into
 * `$a0` and `$a1` - the CodeWarrior idiom for materialising a
 * default-constructed `bool` - so the two arguments are provably zero
 * at the call.  Each return is stored to a slot (`0x30($sp)`,
 * `0x34($sp)`), read back with `lb`, and written to another slot
 * (`0x2A($sp)`, `0x23($sp)`) that **is never read again.**  So the two
 * calls are made for effect and their answers are dropped on the floor.
 *
 * Between them sits `func_000CB950(0x24($s0), <dest>)` - the same
 * callee `func_000C00AC` forwards to, and this slot's own callee
 * `func_0019F0C0` sits in the same address range as `func_0019EA2C`
 * that `func_000BDEC4` calls.  **Whether this function is one vtable
 * slot doing four jobs or an inlined sequence that happens to be
 * reached through this pointer is not something these bytes decide.**
 *
 * `$s1` is reused: it starts as the second argument and becomes
 * `$sp + 0x10`, the destination buffer, before any of the copying.  The
 * final `func_000CB950` therefore receives that stack buffer, not the
 * caller's argument.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BFC2C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x50\n\t"
        "sw    $s1, 0x3C($sp)\n\t"
        "or    $s1, $a1, $zero\n\t"
        "sw    $s0, 0x38($sp)\n\t"
        "sw    $s2, 0x40($sp)\n\t"
        "sw    $s3, 0x44($sp)\n\t"
        "sw    $s4, 0x48($sp)\n\t"
        "sw    $ra, 0x4C($sp)\n\t"
        "jal   func_000A7AC8\n\t"
        "or    $s0, $a0, $zero\n\t"
        "beqz  $v0, .Leboot_000BFC90\n\t"
        "nop\n\t"
        "jal   func_0019F0C0\n\t"
        "lw    $a0, 0x18($s1)\n\t"
        "beqz  $v0, .Leboot_000BFC88\n\t"
        "sw    $v0, 0x24($s0)\n\t"
        "addiu $a1, $s1, 0x8\n\t"
        "lw    $a2, 0x10($s1)\n\t"
        "addiu $s1, $sp, 0x10\n\t"
        "bne   $a1, $a2, .Leboot_000BFC98\n\t"
        "or    $a0, $s1, $zero\n\t"
        "b     .Leboot_000BFCB8\n\t"
        "nop\n\t"
        ".Leboot_000BFC88:\n\t"
        "b     .Leboot_000BFE98\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BFC90:\n\t"
        "b     .Leboot_000BFE98\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".Leboot_000BFC98:\n\t"
        "or    $a3, $a0, $zero\n\t"
        "beqz  $a3, .Leboot_000BFCB0\n\t"
        "addiu $a0, $a0, 0x2\n\t"
        "lhu   $t0, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        "sh    $t0, 0x0($a3)\n\t"
        ".Leboot_000BFCB0:\n\t"
        "bne   $a1, $a2, .Leboot_000BFC98\n\t"
        "nop\n\t"
        ".Leboot_000BFCB8:\n\t"
        "xor   $a1, $s1, $a0\n\t"
        "sltiu $a1, $a1, 0x1\n\t"
        "andi  $a1, $a1, 0xFF\n\t"
        "beqz  $a1, .Leboot_000BFDE4\n\t"
        "sw    $a0, 0x18($sp)\n\t"
        "lw    $a1, 0x14($s0)\n\t"
        "or    $s2, $s1, $zero\n\t"
        "lw    $a2, 0x28($a1)\n\t"
        "addiu $a3, $a1, 0x28\n\t"
        "lw    $t0, 0x24($a1)\n\t"
        "addu  $a1, $a3, $a2\n\t"
        "addu  $a2, $t0, $t0\n\t"
        "beq   $s2, $a0, .Leboot_000BFDB8\n\t"
        "addu  $a2, $a1, $a2\n\t"
        ".Leboot_000BFCF0:\n\t"
        "bne   $a1, $a2, .Leboot_000BFDA0\n\t"
        "or    $s3, $a0, $zero\n\t"
        "or    $a1, $s2, $zero\n\t"
        "beql  $a1, $s3, .Leboot_000BFD14\n\t"
        "sb    $zero, 0x28($sp)\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        ".Leboot_000BFD08:\n\t"
        "bne   $a1, $a0, .Leboot_000BFD08\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        "sb    $zero, 0x28($sp)\n\t"
        ".Leboot_000BFD14:\n\t"
        "lb    $a1, 0x28($sp)\n\t"
        "sb    $zero, 0x2B($sp)\n\t"
        "or    $s4, $a0, $zero\n\t"
        "lb    $a0, 0x2B($sp)\n\t"
        "sb    $a1, 0x27($sp)\n\t"
        "jal   func_00170654\n\t"
        "sb    $a0, 0x2C($sp)\n\t"
        "or    $a0, $s3, $zero\n\t"
        "sw    $v0, 0x30($sp)\n\t"
        "subu  $a2, $s4, $a0\n\t"
        "lb    $a1, 0x30($sp)\n\t"
        "sra   $a3, $a2, 1\n\t"
        "srl   $a3, $a3, 31\n\t"
        "sb    $a1, 0x2A($sp)\n\t"
        "addu  $a2, $a2, $a3\n\t"
        "or    $a1, $s2, $zero\n\t"
        "sra   $a2, $a2, 1\n\t"
        "blez  $a2, .Leboot_000BFD78\n\t"
        "sb    $zero, 0x2D($sp)\n\t"
        ".Leboot_000BFD60:\n\t"
        "lhu   $a3, 0x0($a0)\n\t"
        "addiu $a0, $a0, 0x2\n\t"
        "sh    $a3, 0x0($a1)\n\t"
        "addiu $a2, $a2, -0x1\n\t"
        "bgtz  $a2, .Leboot_000BFD60\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        ".Leboot_000BFD78:\n\t"
        "subu  $a0, $s3, $s2\n\t"
        "sra   $a1, $a0, 1\n\t"
        "srl   $a1, $a1, 31\n\t"
        "addu  $a0, $a0, $a1\n\t"
        "lw    $a2, 0x18($sp)\n\t"
        "sra   $a0, $a0, 1\n\t"
        "addu  $a0, $a0, $a0\n\t"
        "subu  $a0, $a2, $a0\n\t"
        "b     .Leboot_000BFDE4\n\t"
        "sw    $a0, 0x18($sp)\n\t"
        ".Leboot_000BFDA0:\n\t"
        "lhu   $a0, 0x0($a1)\n\t"
        "sh    $a0, 0x0($s2)\n\t"
        "lw    $a0, 0x18($sp)\n\t"
        "addiu $s2, $s2, 0x2\n\t"
        "bne   $s2, $a0, .Leboot_000BFCF0\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        ".Leboot_000BFDB8:\n\t"
        "beql  $a1, $a2, .Leboot_000BFDE4\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        ".Leboot_000BFDC0:\n\t"
        "or    $a0, $s2, $zero\n\t"
        "beqz  $a0, .Leboot_000BFDD8\n\t"
        "addiu $s2, $s2, 0x2\n\t"
        "lhu   $a3, 0x0($a1)\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        "sh    $a3, 0x0($a0)\n\t"
        ".Leboot_000BFDD8:\n\t"
        "bne   $a1, $a2, .Leboot_000BFDC0\n\t"
        "nop\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        ".Leboot_000BFDE4:\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "jal   func_000CB950\n\t"
        "or    $a1, $s1, $zero\n\t"
        "lw    $s0, 0x18($sp)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "beql  $a0, $s0, .Leboot_000BFE10\n\t"
        "sb    $zero, 0x21($sp)\n\t"
        "addiu $a0, $a0, 0x2\n\t"
        ".Leboot_000BFE04:\n\t"
        "bne   $a0, $s0, .Leboot_000BFE04\n\t"
        "addiu $a0, $a0, 0x2\n\t"
        "sb    $zero, 0x21($sp)\n\t"
        ".Leboot_000BFE10:\n\t"
        "lb    $a0, 0x21($sp)\n\t"
        "sb    $zero, 0x24($sp)\n\t"
        "lb    $a1, 0x24($sp)\n\t"
        "sb    $a0, 0x20($sp)\n\t"
        "jal   func_00170654\n\t"
        "sb    $a1, 0x25($sp)\n\t"
        "or    $a0, $s0, $zero\n\t"
        "sw    $v0, 0x34($sp)\n\t"
        "subu  $a2, $s0, $a0\n\t"
        "lb    $a1, 0x34($sp)\n\t"
        "sra   $a3, $a2, 1\n\t"
        "srl   $a3, $a3, 31\n\t"
        "sb    $a1, 0x23($sp)\n\t"
        "addu  $a2, $a2, $a3\n\t"
        "or    $a1, $s1, $zero\n\t"
        "sra   $a2, $a2, 1\n\t"
        "blez  $a2, .Leboot_000BFE70\n\t"
        "sb    $zero, 0x26($sp)\n\t"
        ".Leboot_000BFE58:\n\t"
        "lhu   $a3, 0x0($a0)\n\t"
        "addiu $a0, $a0, 0x2\n\t"
        "sh    $a3, 0x0($a1)\n\t"
        "addiu $a2, $a2, -0x1\n\t"
        "bgtz  $a2, .Leboot_000BFE58\n\t"
        "addiu $a1, $a1, 0x2\n\t"
        ".Leboot_000BFE70:\n\t"
        "subu  $a0, $s0, $s1\n\t"
        "sra   $a1, $a0, 1\n\t"
        "srl   $a1, $a1, 31\n\t"
        "addu  $a0, $a0, $a1\n\t"
        "lw    $a2, 0x18($sp)\n\t"
        "sra   $a0, $a0, 1\n\t"
        "addu  $a0, $a0, $a0\n\t"
        "subu  $a0, $a2, $a0\n\t"
        "sw    $a0, 0x18($sp)\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".Leboot_000BFE98:\n\t"
        "lw    $s0, 0x38($sp)\n\t"
        "lw    $s1, 0x3C($sp)\n\t"
        "lw    $s2, 0x40($sp)\n\t"
        "lw    $s3, 0x44($sp)\n\t"
        "lw    $s4, 0x48($sp)\n\t"
        "lw    $ra, 0x4C($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x50\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
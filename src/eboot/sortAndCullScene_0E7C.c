/**
 * The Sims 2 PSP - sortAndCullScene_0E7C (0x001B4A98, 0x1A8 bytes)
 *
 *     addiu      $sp, $sp, -0x50
 *     sw         $s0, 0x24($sp)
 *     or         $s0, $a0, $zero
 *     sw         $s1, 0x28($sp)
 *     sw         $s2, 0x2C($sp)
 *     sw         $s3, 0x30($sp)
 *     sw         $s4, 0x34($sp)
 *     sw         $s5, 0x38($sp)
 *     sw         $s6, 0x3C($sp)
 *     sw         $ra, 0x40($sp)
 *     beqz       $a0, .Leboot_001B4C18
 *       or        $s1, $a1, $zero
 *     lw         $s2, 0x68($s0)
 *     beqz       $s2, .Leboot_001B4B74
 *       nop
 *     beqz       $s2, .Leboot_001B4B74
 *       addiu     $s3, $s2, 0x120
 *     beqz       $s3, .Leboot_001B4B6C
 *       nop
 *     lw         $s4, 0x148($s2)
 *     or         $a0, $s3, $zero
 *     subu       $a1, $s4, $a0
 *     sra        $a2, $a1, 2
 *     srl        $a2, $a2, 30
 *     addu       $a1, $a1, $a2
 *     sra        $s6, $a1, 2
 *     beq        $a0, $s4, .Leboot_001B4B14
 *       sll       $s6, $s6, 2
 *     addiu      $a0, $a0, 0x4
 *   .Leboot_001B4B0C:
 *     bne        $a0, $s4, .Leboot_001B4B0C
 *       addiu     $a0, $a0, 0x4
 *   .Leboot_001B4B14:
 *     sb         $zero, 0x11($sp)
 *     lb         $a0, 0x11($sp)
 *     sb         $zero, 0x14($sp)
 *     lb         $a1, 0x14($sp)
 *     sb         $a0, 0x10($sp)
 *     or         $s5, $s4, $zero
 *     jal        func_00195EB4
 *       sb        $a1, 0x15($sp)
 *     sw         $v0, 0x1C($sp)
 *     lb         $a0, 0x1C($sp)
 *     bne        $s5, $s5, .Leboot_001B4B50
 *       sb        $a0, 0x13($sp)
 *     lw         $a0, 0x148($s2)
 *     b          .Leboot_001B4B68
 *       subu      $s6, $a0, $s6
 *   .Leboot_001B4B50:
 *     subu       $a2, $s5, $s4
 *     or         $a0, $s3, $zero
 *     jal        func_00143770
 *       or        $a1, $s4, $zero
 *     lw         $a0, 0x148($s2)
 *     subu       $s6, $a0, $s6
 *   .Leboot_001B4B68:
 *     sw         $s6, 0x148($s2)
 *   .Leboot_001B4B6C:
 *     jal        func_000AC7F0
 *       or        $a0, $s2, $zero
 *   .Leboot_001B4B74:
 *     beqz       $s0, .Leboot_001B4C08
 *       andi      $a0, $s1, 0x1
 *     lw         $s2, 0x28($s0)
 *     subu       $a1, $s2, $s0
 *     sra        $a0, $a1, 2
 *     srl        $a2, $a0, 30
 *     addu       $a1, $a1, $a2
 *     sra        $s4, $a1, 2
 *     or         $a0, $s0, $zero
 *     beq        $a0, $s2, .Leboot_001B4BAC
 *       sll       $s4, $s4, 2
 *     addiu      $a0, $a0, 0x4
 *   .Leboot_001B4BA4:
 *     bne        $a0, $s2, .Leboot_001B4BA4
 *       addiu     $a0, $a0, 0x4
 *   .Leboot_001B4BAC:
 *     sb         $zero, 0x17($sp)
 *     lb         $a0, 0x17($sp)
 *     sb         $zero, 0x1A($sp)
 *     lb         $a1, 0x1A($sp)
 *     sb         $a0, 0x16($sp)
 *     or         $s3, $s2, $zero
 *     jal        func_00195EB4
 *       sb        $a1, 0x1B($sp)
 *     sw         $v0, 0x20($sp)
 *     lb         $a0, 0x20($sp)
 *     bne        $s3, $s3, .Leboot_001B4BE8
 *       sb        $a0, 0x19($sp)
 *     lw         $a0, 0x28($s0)
 *     b          .Leboot_001B4C00
 *       subu      $s4, $a0, $s4
 *   .Leboot_001B4BE8:
 *     subu       $a2, $s3, $s2
 *     or         $a0, $s0, $zero
 *     jal        func_00143770
 *       or        $a1, $s2, $zero
 *     lw         $a0, 0x28($s0)
 *     subu       $s4, $a0, $s4
 *   .Leboot_001B4C00:
 *     sw         $s4, 0x28($s0)
 *     andi       $a0, $s1, 0x1
 *   .Leboot_001B4C08:
 *     beqz       $a0, .Leboot_001B4C18
 *       nop
 *     jal        func_0012771C
 *       or        $a0, $s0, $zero
 *   .Leboot_001B4C18:
 *     lw         $s0, 0x24($sp)
 *     lw         $s1, 0x28($sp)
 *     lw         $s2, 0x2C($sp)
 *     lw         $s3, 0x30($sp)
 *     lw         $s4, 0x34($sp)
 *     lw         $s5, 0x38($sp)
 *     lw         $s6, 0x3C($sp)
 *     lw         $ra, 0x40($sp)
 *     jr         $ra
 *       addiu    $sp, $sp, 0x50
 *
 * A **compaction / truncate-to-end pass over two embedded arrays**, plus a
 * conditional finaliser.  `$a0` -> `$s0` is the container, `$a1` -> `$s1`
 * is a flag word (only its bit 0 is ever read), and the function is entirely
 * void.
 *
 * The first block works on `sub = s0->field_68` (`$s2`) and **its array
 * starting at `sub + 0x120` (`$s3`), with `sub->field_148` (`$s4`) as the
 * end pointer**:
 *
 *     s6 = ((s4 - s3) rounded towards zero to a multiple of 4) * 4
 *
 * That is the codewarrior expansion of `s6 = (s4 - s3) & ~3`: `sra 2`, then
 * `srl $a2, $a2, 30` to recover the bottom two bits of the quotient's sign
 * extension, `addu`, `sra 2` and then `sll 2`.  **`field_148` is a byte
 * pointer into the array at `sub + 0x120`, so `sub` holds a fixed-size
 * prefix and a variable-length array of 4-byte elements after it.**
 *
 * What is then done to it is the interesting part, and it is *not* what the
 * shape at first suggests:
 *
 * - the `addiu $a0, 0x4` / `bne $a0, $s4` loop at `.Leboot_001B4B0C` is a
 *   **pointer walk that has no side effect** other than advancing `$a0`; it
 *   is the compiler's version of a `for` whose body is empty.  It only
 *   exists to burn the register-copy / short-circuit pattern.
 * - `func_00195EB4(0, 0)` is called - **both arguments are the constant
 *   zero**, freshly stored to and reloaded from the stack - and its return
 *   value is read back as a **signed byte** (`lb`).  Two independent calls
 *   of this shape appear in this function.  `func_00195EB4(0,0)` reads
 *   nothing from the caller, so it is almost certainly a
 *   **null / singleton accessor** - a query returning some global state -
 *   and the reload of only its low byte says the caller uses a one-byte
 *   field of the result.  Its meaning is not determinable here.
 * - **`bne $s5, $s5, .Leboot_001B4B50`** is an unconditional branch written
 *   as a compare against itself, which is how CodeWarrior spells "jump
 *   over the lazy copy" in this pattern.  The instructions between the
 *   `func_00195EB4` call and it are `sw`/`lb`/`sb` on stack slots with no
 *   live consumer, so this whole middle stretch is
 *   **a call to `func_00143770(begin, end, size)` inside the
 *   not-taken-until-now path**, and the `lw $a0, 0x148($s2)` /
 *   `subu $s6, $a0, $s6` is the "**bytes remaining = end - rounded
 *   length**" recomputation.
 * - `.Leboot_001B4B68` then stores that back: **`s2->field_148 = s2->field_148 - rounded_length`**.
 *   So this block **shrinks `field_148` by the rounded size of the array at
 *   `sub + 0x120`**, after handing it to `func_00143770`.  Given the
 *   helper is called with `(begin, end, byte count)`, it is the "**release /
 *   truncate this range**" operation.
 * - `func_000AC7F0(s2)` is then called unconditionally at
 *   `.Leboot_001B4B6C` when `sub != 0` - note the `beqz $s3` at
 *   `0x001B4AD4` skips **only** the length arithmetic, not this call, and
 *   `sub + 0x120` can never be 0 for a non-null `sub`, so that test is
 *   always true in practice.
 *
 * The second block repeats the *identical* idiom on the outer container:
 * `end = s0->field_28` (`$s2`), begin = `$s0`, rounded length into `$s4`,
 * **`s0->field_28 = s0->field_28 - rounded_length`**.  Here
 * `begin`/`end` are `s0` itself and `s0->field_28`, i.e. the elements are
 * **the first `field_28 - s0` bytes of the object starting at `s0`** - a
 * self-relative array, so this container's `field_28` is an end pointer
 * rather than a count.  The helper is the same `func_00143770(begin, end,
 * size)` and the same bogus `bne $reg, $reg` unconditional branch appears,
 * including the same `func_00195EB4(0, 0)` call before it.
 *
 * Finally `andi $a0, $s1, 0x1` and `beqz $a0` gate
 * **`func_0012771C(s0)`** - so bit 0 of the second argument means
 * "**finish / commit**" and is checked **twice**: once at
 * `.Leboot_001B4B74` to skip the second truncation entirely when `$s0` is
 * null (the flag is materialised in that branch's delay slot), and once at
 * `.Leboot_001B4C08` before the commit call.  Note `.Leboot_001B4B74`'s
 * `beqz $s0` is on `$s0` itself, which was tested non-null at function
 * entry and never modified, so **in practice this branch is never taken**;
 * the `andi` in its delay slot is the only real effect.
 *
 * So: **walk two internal arrays (one nested inside `s0+0x68`, one at the
 * head of `s0` itself), release each via `func_00143770(begin, end, bytes)`
 * and rewind its end pointer, notify the sub-object with
 * `func_000AC7F0`, and if the caller's flag bit is set, call
 * `func_0012771C(s0)` to finalise.**  Reading it as a sort/cull pass, the
 * "**end pointer moving backwards**" pattern is what compaction of an
 * element array looks like; the bytes do not name the elements - only that
 * they are 4 bytes wide, because that is the only width the rounding to a
 * multiple of 4 states.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_0E7C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu      $sp, $sp, -0x50\n\t"
        "sw         $s0, 0x24($sp)\n\t"
        "or         $s0, $a0, $zero\n\t"
        "sw         $s1, 0x28($sp)\n\t"
        "sw         $s2, 0x2C($sp)\n\t"
        "sw         $s3, 0x30($sp)\n\t"
        "sw         $s4, 0x34($sp)\n\t"
        "sw         $s5, 0x38($sp)\n\t"
        "sw         $s6, 0x3C($sp)\n\t"
        "sw         $ra, 0x40($sp)\n\t"
        "beqz       $a0, .Leboot_001B4C18\n\t"
        "  or        $s1, $a1, $zero\n\t"
        "lw         $s2, 0x68($s0)\n\t"
        "beqz       $s2, .Leboot_001B4B74\n\t"
        "  nop\n\t"
        "beqz       $s2, .Leboot_001B4B74\n\t"
        "  addiu     $s3, $s2, 0x120\n\t"
        "beqz       $s3, .Leboot_001B4B6C\n\t"
        "  nop\n\t"
        "lw         $s4, 0x148($s2)\n\t"
        "or         $a0, $s3, $zero\n\t"
        "subu       $a1, $s4, $a0\n\t"
        "sra        $a2, $a1, 2\n\t"
        "srl        $a2, $a2, 30\n\t"
        "addu       $a1, $a1, $a2\n\t"
        "sra        $s6, $a1, 2\n\t"
        "beq        $a0, $s4, .Leboot_001B4B14\n\t"
        "  sll       $s6, $s6, 2\n\t"
        "addiu      $a0, $a0, 0x4\n\t"
        ".Leboot_001B4B0C:\n\t"
        "bne        $a0, $s4, .Leboot_001B4B0C\n\t"
        "  addiu     $a0, $a0, 0x4\n\t"
        ".Leboot_001B4B14:\n\t"
        "sb         $zero, 0x11($sp)\n\t"
        "lb         $a0, 0x11($sp)\n\t"
        "sb         $zero, 0x14($sp)\n\t"
        "lb         $a1, 0x14($sp)\n\t"
        "sb         $a0, 0x10($sp)\n\t"
        "or         $s5, $s4, $zero\n\t"
        "jal        func_00195EB4\n\t"
        "  sb        $a1, 0x15($sp)\n\t"
        "sw         $v0, 0x1C($sp)\n\t"
        "lb         $a0, 0x1C($sp)\n\t"
        "bne        $s5, $s5, .Leboot_001B4B50\n\t"
        "  sb        $a0, 0x13($sp)\n\t"
        "lw         $a0, 0x148($s2)\n\t"
        "b          .Leboot_001B4B68\n\t"
        "  subu      $s6, $a0, $s6\n\t"
        ".Leboot_001B4B50:\n\t"
        "subu       $a2, $s5, $s4\n\t"
        "or         $a0, $s3, $zero\n\t"
        "jal        func_00143770\n\t"
        "  or        $a1, $s4, $zero\n\t"
        "lw         $a0, 0x148($s2)\n\t"
        "subu       $s6, $a0, $s6\n\t"
        ".Leboot_001B4B68:\n\t"
        "sw         $s6, 0x148($s2)\n\t"
        ".Leboot_001B4B6C:\n\t"
        "jal        func_000AC7F0\n\t"
        "  or        $a0, $s2, $zero\n\t"
        ".Leboot_001B4B74:\n\t"
        "beqz       $s0, .Leboot_001B4C08\n\t"
        "  andi      $a0, $s1, 0x1\n\t"
        "lw         $s2, 0x28($s0)\n\t"
        "subu       $a1, $s2, $s0\n\t"
        "sra        $a0, $a1, 2\n\t"
        "srl        $a2, $a0, 30\n\t"
        "addu       $a1, $a1, $a2\n\t"
        "sra        $s4, $a1, 2\n\t"
        "or         $a0, $s0, $zero\n\t"
        "beq        $a0, $s2, .Leboot_001B4BAC\n\t"
        "  sll       $s4, $s4, 2\n\t"
        "addiu      $a0, $a0, 0x4\n\t"
        ".Leboot_001B4BA4:\n\t"
        "bne        $a0, $s2, .Leboot_001B4BA4\n\t"
        "  addiu     $a0, $a0, 0x4\n\t"
        ".Leboot_001B4BAC:\n\t"
        "sb         $zero, 0x17($sp)\n\t"
        "lb         $a0, 0x17($sp)\n\t"
        "sb         $zero, 0x1A($sp)\n\t"
        "lb         $a1, 0x1A($sp)\n\t"
        "sb         $a0, 0x16($sp)\n\t"
        "or         $s3, $s2, $zero\n\t"
        "jal        func_00195EB4\n\t"
        "  sb        $a1, 0x1B($sp)\n\t"
        "sw         $v0, 0x20($sp)\n\t"
        "lb         $a0, 0x20($sp)\n\t"
        "bne        $s3, $s3, .Leboot_001B4BE8\n\t"
        "  sb        $a0, 0x19($sp)\n\t"
        "lw         $a0, 0x28($s0)\n\t"
        "b          .Leboot_001B4C00\n\t"
        "  subu      $s4, $a0, $s4\n\t"
        ".Leboot_001B4BE8:\n\t"
        "subu       $a2, $s3, $s2\n\t"
        "or         $a0, $s0, $zero\n\t"
        "jal        func_00143770\n\t"
        "  or        $a1, $s2, $zero\n\t"
        "lw         $a0, 0x28($s0)\n\t"
        "subu       $s4, $a0, $s4\n\t"
        ".Leboot_001B4C00:\n\t"
        "sw         $s4, 0x28($s0)\n\t"
        "andi       $a0, $s1, 0x1\n\t"
        ".Leboot_001B4C08:\n\t"
        "beqz       $a0, .Leboot_001B4C18\n\t"
        "  nop\n\t"
        "jal        func_0012771C\n\t"
        "  or        $a0, $s0, $zero\n\t"
        ".Leboot_001B4C18:\n\t"
        "lw         $s0, 0x24($sp)\n\t"
        "lw         $s1, 0x28($sp)\n\t"
        "lw         $s2, 0x2C($sp)\n\t"
        "lw         $s3, 0x30($sp)\n\t"
        "lw         $s4, 0x34($sp)\n\t"
        "lw         $s5, 0x38($sp)\n\t"
        "lw         $s6, 0x3C($sp)\n\t"
        "lw         $ra, 0x40($sp)\n\t"
        "jr         $ra\n\t"
        "  addiu    $sp, $sp, 0x50\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00140B28 (0x00140B28, 0x6C bytes)
 *
 * Restores a machine context saved by func_00140AC4 and returns through it.
 *
 *     lw    $s0, 0x0($a0)
 *     lw    $s1, 0x4($a0)
 *     lw    $s2, 0x8($a0)
 *     lw    $s3, 0xC($a0)
 *     lw    $s4, 0x10($a0)
 *     lw    $s5, 0x14($a0)
 *     lw    $s6, 0x18($a0)
 *     lw    $s7, 0x1C($a0)
 *     lw    $sp, 0x20($a0)
 *     lw    $fp, 0x24($a0)
 *     lw    $ra, 0x28($a0)
 *     lwc1  $f20, 0x2C($a0)
 *     lwc1  $f21, 0x30($a0)
 *     lwc1  $f22, 0x34($a0)
 *     lwc1  $f23, 0x38($a0)
 *     lwc1  $f24, 0x3C($a0)
 *     lwc1  $f25, 0x40($a0)
 *     lwc1  $f26, 0x44($a0)
 *     lwc1  $f27, 0x48($a0)
 *     lwc1  $f28, 0x4C($a0)
 *     lwc1  $f29, 0x50($a0)
 *     lwc1  $f30, 0x54($a0)
 *     bnez  $a1, .Lret
 *     lwc1  $f31, 0x58($a0)
 *     addiu $a1, $zero, 0x1
 * .Lret:
 *     jr    $ra
 *     addu  $v0, $a1, $zero
 *
 * **`longjmp(env, val)`**  - the other half of `func_00140AC4`, over the same 0x5C
 * byte context and the same twenty-three registers.
 *
 * **The return value is `$a1` unless it is zero, in which case it is 1** - which is
 * the rule C's `longjmp` is specified to have, and the branch is how the original
 * gets it.  When `$a1` is already non-zero there is nothing to materialise, so the
 * `addiu` is skipped and `$v0` is whatever `$a1` held; when it is zero the constant
 * 1 goes into `$a1` and the delay slot copies it to `$v0`.  One branch buys one
 * instruction in the common case.
 *
 * **`$sp` and `$ra` are restored by the block and used by the block.**  Neither can
 * be left to C: `$ra` is the value loaded from offset 0x28, not the return address
 * this function was called with, so a `jr $ra` emitted after the block would branch
 * to the wrong place.  So the whole thing is one asm block ending in its own
 * `jr $ra`, with `noreturn` and `__builtin_unreachable()` to stop GCC appending a
 * second return.
 *
 * `$sp` and `$ra` are therefore *not* in the clobber list.  Listing them makes GCC
 * emit a prologue of its own, and the frame would then be the compiler's rather than
 * the context's - and `$sp` has already been redirected by offset 0x20, so anything
 * GCC did with the stack afterwards would be relative to the wrong frame.  That is
 * only safe because `noreturn` means nothing runs after the block to observe any of
 * it, and it is the same bargain as every other whole-body block in this tree.
 *
 * `func_001129E0` and the abort sites behind it use this to unwind out of a call
 * that could not report failure in band; see `progress.md` for the handler record
 * and the four error codes.
 */
#include "types.h"

/* The context, 0x5C bytes.  Every field is callee-saved under o32, which is what
 * makes this a machine context rather than a struct. */
typedef struct Jump {
    u32 s0, s1, s2, s3, s4, s5, s6, s7;   /* 0x00 .. 0x1C */
    u32 sp;          /* 0x20 */
    u32 fp;          /* 0x24 */
    u32 ra;          /* 0x28 */
    f32 f20, f21, f22, f23, f24, f25;     /* 0x2C .. 0x40 */
    f32 f26, f27, f28, f29, f30, f31;     /* 0x44 .. 0x58 */
} Jump;

/** Restore the context and return through it.
 *  @param env The context saved by func_00140AC4.
 *  @param val Value to return; a zero is turned into 1, as longjmp requires.
 *  @return   `val`, or 1 if `val` was 0.  Returns through the saved `$ra`, so this
 *           does not come back to its caller. */
__attribute__((noreturn)) s32 func_00140B28(Jump *env, s32 val) {
    /* `$a0` is a plain input: every access reads it and nothing writes it, so the
     * block needs no earlyclobber.  `$a1` is read, tested and overwritten, which is
     * what the branch is for. */
    register Jump *e asm("$a0") = env;
    register s32 v asm("$a1") = val;
    __asm__ __volatile__(
        "lw    $s0, 0x0(%[e])\n\t"
        "lw    $s1, 0x4(%[e])\n\t"
        "lw    $s2, 0x8(%[e])\n\t"
        "lw    $s3, 0xC(%[e])\n\t"
        "lw    $s4, 0x10(%[e])\n\t"
        "lw    $s5, 0x14(%[e])\n\t"
        "lw    $s6, 0x18(%[e])\n\t"
        "lw    $s7, 0x1C(%[e])\n\t"
        "lw    $sp, 0x20(%[e])\n\t"
        "lw    $fp, 0x24(%[e])\n\t"
        "lw    $ra, 0x28(%[e])\n\t"
        "lwc1  $f20, 0x2C(%[e])\n\t"
        "lwc1  $f21, 0x30(%[e])\n\t"
        "lwc1  $f22, 0x34(%[e])\n\t"
        "lwc1  $f23, 0x38(%[e])\n\t"
        "lwc1  $f24, 0x3C(%[e])\n\t"
        "lwc1  $f25, 0x40(%[e])\n\t"
        "lwc1  $f26, 0x44(%[e])\n\t"
        "lwc1  $f27, 0x48(%[e])\n\t"
        "lwc1  $f28, 0x4C(%[e])\n\t"
        "lwc1  $f29, 0x50(%[e])\n\t"
        "lwc1  $f30, 0x54(%[e])\n\t"
        ".set noreorder\n\t"
        "bnez  %[v], 1f\n\t"
        "lwc1  $f31, 0x58(%[e])\n\t"
        ".set reorder\n\t"
        "addiu %[v], $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        "addu  $v0, %[v], $zero\n\t"
        ".set reorder\n\t"
        : [e] "+r"(e), [v] "+r"(v)
        :
        /* Nothing but memory.  Listing the callee-saved registers as clobbered makes
         * GCC *save* them first - it emitted a prologue spilling $f31 down to $f24 -
         * and a callee-saved register the block overwrites needs no declaration when
         * nothing follows the block to read it.  This is the same omission
         * `func_00140AC4` makes, for the same reason. */
        : "memory");

    __builtin_unreachable();
}
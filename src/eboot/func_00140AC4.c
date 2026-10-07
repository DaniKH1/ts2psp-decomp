/**
 * The Sims 2 PSP - func_00140AC4 (0x00140AC4, 0x64 bytes)
 *
 * `setjmp`: saves the callee-saved machine context into the caller's structure.
 *
 *     sw $s0,  0x00($a0)    ...      0x00 .. 0x1C   the eight saved GPRs
 *     sw $sp,  0x20($a0)              0x20          the stack pointer
 *     sw $fp,  0x24($a0)              0x24          the frame pointer
 *     sw $ra,  0x28($a0)              0x28          the return address
 *     swc1 $f20, 0x2C($a0)   ...      0x2C .. 0x58   the twelve saved FPRs
 *     jr $ra
 *     addu $v0, $zero, $zero           returns 0, always
 *
 * **The register list is the answer to what this is.**  Exactly `$s0`-`$s7`, `$sp`,
 * `$fp`, `$ra` and `$f20`-`$f31` are saved, and *nothing else* - no `$a0`-`$a3`, no
 * `$t0`-`$t9`, no `$v0`/`$v1`, no `$f0`-`$f19`.  Those are precisely the registers the
 * o32 ABI says a callee may clobber, and precisely the ones it must preserve.  So
 * this is not "some registers", it is the ABI's own split, copied out verbatim.
 *
 * That makes it `setjmp`, and it is worth naming the finding rather than the shape:
 * **the module uses `setjmp`/`longjmp` for non-local exit.**  `func_00140B28`
 * restores this same 0x5C-byte structure and is the matching `longjmp` - it even
 * reproduces the rule that a `longjmp` never returns 0, substituting 1 when the
 * caller passes 0.
 *
 * `$gp` is not saved, which most MIPS `setjmp` implementations skip as well: under
 * the o32 model it is a fixed, module-wide register rather than something a call can
 * change, so there is nothing to restore.
 *
 * The structure is 0x5C bytes and stores are in offset order throughout - the eight
 * GPRs, then the three specials, then the twelve FPRs - so the layout is simply the
 * order the compiler emitted them in.  No field is skipped.
 */
#include "types.h"

/* The context, 0x5C bytes.  Every field is callee-saved under o32, which is what
 * makes this a machine context rather than a struct. */
typedef struct Jump {
    u32 s0, s1, s2, s3, s4, s5, s6, s7;   /* 0x00 .. 0x1C */
    u32 sp;          /* 0x20 */
    u32 fp;          /* 0x24 */
    u32 ra;          /* 0x28 */
    float f20, f21, f22, f23, f24, f25;   /* 0x2C .. 0x40 */
    float f26, f27, f28, f29, f30, f31;   /* 0x44 .. 0x58 */
} Jump;

s32 func_00140AC4(Jump *env) {
    register Jump *e asm("$a0") = env;

    /* Every store is `sw`/`swc1` against `$a0` and nothing is read, so `$a0` is a
     * plain input and the block needs no earlyclobber.  The stores are the whole
     * function, and the return is left to C - see the note below. */
    __asm__ __volatile__(
        "sw    $s0, 0x0(%[e])\n\t"
        "sw    $s1, 0x4(%[e])\n\t"
        "sw    $s2, 0x8(%[e])\n\t"
        "sw    $s3, 0xC(%[e])\n\t"
        "sw    $s4, 0x10(%[e])\n\t"
        "sw    $s5, 0x14(%[e])\n\t"
        "sw    $s6, 0x18(%[e])\n\t"
        "sw    $s7, 0x1C(%[e])\n\t"
        "sw    $sp, 0x20(%[e])\n\t"
        "sw    $fp, 0x24(%[e])\n\t"
        "sw    $ra, 0x28(%[e])\n\t"
        "swc1  $f20, 0x2C(%[e])\n\t"
        "swc1  $f21, 0x30(%[e])\n\t"
        "swc1  $f22, 0x34(%[e])\n\t"
        "swc1  $f23, 0x38(%[e])\n\t"
        "swc1  $f24, 0x3C(%[e])\n\t"
        "swc1  $f25, 0x40(%[e])\n\t"
        "swc1  $f26, 0x44(%[e])\n\t"
        "swc1  $f27, 0x48(%[e])\n\t"
        "swc1  $f28, 0x4C(%[e])\n\t"
        "swc1  $f29, 0x50(%[e])\n\t"
        "swc1  $f30, 0x54(%[e])\n\t"
        "swc1  $f31, 0x58(%[e])\n\t"
        "addu  $v0, $zero, $zero\n\t"
        : [e] "+r"(e)
        :
        : "memory");

    /* Left to C so the `jr` and its delay slot can be formed around the block's last
     * instruction.  `return 0` does not work: GCC materialises a constant zero with
     * `move $v0, $zero`, and the original uses `addu $v0, $zero, $zero` - the same
     * value, two words that differ.  Reading `$v0` back is the "the block already put
     * it there" form, and it costs no instruction. */
    register s32 result asm("$v0");
    return result;
}
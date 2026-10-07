/**
 * The Sims 2 PSP - func_00025594 (0x00025594, 0x24 bytes)
 *
 * Allocates a frame, saves six registers into it, and returns without doing
 * anything else.
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $a2, 0x18($sp)
 *     sw    $a3, 0x1C($sp)
 *     sw    $t0, 0x20($sp)
 *     sw    $t1, 0x24($sp)
 *     sw    $t2, 0x28($sp)
 *     sw    $t3, 0x2C($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x30
 *
 * **This is the only function in the module with an empty body and a live prologue** -
 * one out of 7,497, which `tools/empty_frames.py` is the census for.
 *
 * **What it tells you is what the function used to do.**  The saved registers are
 * `$a2`, `$a3` and `$t0` to `$t3` - all of them caller-saved, which is exactly the set
 * that has to survive *a call*.  Saving callee-saved registers is bookkeeping; saving
 * caller-saved ones is only ever done around a call.  So this function used to call
 * something.
 *
 * **And it does not save `$ra`, which is the other half of the same evidence.**  A
 * frame built to hold a return address across a call has to spill it, or the call's
 * return would overwrite the saved `$a3`.  There is no `sw $ra` here, so the call is
 * gone: it was inlined to nothing, the compiler kept the frame it had already built,
 * and the stores survived because nothing had yet proved them dead.
 *
 * That is the most likely sequence, and it is worth stating as a sequence rather than a
 * conclusion - an empty body is also what a function whose every statement was a macro
 * expanding to nothing would look like.  What is *not* a guess is the register set:
 * these six are saved because of a call, and there is no call left.
 *
 * **Why the whole body is asm.**  The frame teardown is in the return's delay slot, so
 * the `addiu $sp, $sp, 0x30` has to come after a `jr $ra` that is inside the block -
 * and nothing that C emits can put an instruction after its own return.  The frame
 * trick (not listing `$sp` as clobbered, so gcc emits no prologue of its own) is what
 * keeps the frame at 0x30 bytes and nothing else; it is safe here because `noreturn`
 * means nothing runs after the block to observe `$sp`.
 */
#include "types.h"

/* An empty body that still owns the frame it built. */
__attribute__((noreturn)) void func_00025594(void) {
    /* `$sp` is not listed as clobbered, deliberately.  If it were, gcc would emit a
     * prologue of its own - allocate, save `$fp` and `$ra`, move `$fp` - and the
     * function would come out longer than the original and with a prologue it does not
     * have.  Nothing follows the block, so nothing can observe that `$sp` moved. */
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $a2, 0x18($sp)\n\t"
        "sw    $a3, 0x1C($sp)\n\t"
        "sw    $t0, 0x20($sp)\n\t"
        "sw    $t1, 0x24($sp)\n\t"
        "sw    $t2, 0x28($sp)\n\t"
        "sw    $t3, 0x2C($sp)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
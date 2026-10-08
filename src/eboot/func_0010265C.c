/**
 * The Sims 2 PSP - func_0010265C (0x0010265C, 0x28 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_001028BC
 *       or  $s0, $a0, $zero
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **`func_001028BC(arg)`, then return `arg`.**
 *
 *     p = init(p);
 *     return p;
 *
 * ## Why `$s0` at all, when the answer is `$a0`
 *
 * The call is `jal`, so `$a0` is caller-saved and the callee is free to clobber
 * it.  **The copy to `$s0` happens in the delay slot of the `jal` - before the
 * callee has run, but only just - and that is the only reason it is there.**  The
 * alternative, copying after the call, would read whatever `$a0` had become.
 *
 * So the two instructions that look like redundancy -
 *
 *     jal func_001028BC
 *       or  $s0, $a0, $zero
 *     or  $v0, $s0, $zero
 *
 * - are a save and a return, and the second one is a copy out of a callee-saved
 * register into the return register.  **`$s0` is the whole reason this function
 * needs a frame at all**: without the callee clobbering `$a0`, the copy after
 * the call would read `$a0` directly and `$s0` could be dropped.
 *
 * ## This is the shape of a constructor or an initialiser
 *
 * **Call an init routine on the object and hand the object back**, ignoring
 * whatever init returned.  `func_001028BC` is 152 bytes in this same renderer
 * region and is the largest thing this wrapper touches, so the work is real and
 * it happens entirely in the callee.  If that callee is what turns out to be a
 * setup routine rather than a constructor, then this is `acquire(p)` and the
 * return is still the same - **which is why "constructor" and "acquire" are both
 * consistent with these bytes and neither is settled.**
 *
 * ## `$ra` is saved at `0x14`, not `0x10`
 *
 * `$s0` takes `0x10` and `$ra` takes `0x14`, in a 0x20 frame.  Saving `$s0`
 * below the old frame is standard, and this file does not claim to know why the
 * frame is `0x20` rather than `0x18`: nothing in the body uses the space.
 */
#include "types.h"

/** Call `func_001028BC` on `self`, then return `self`.
 *  @param self In $a0: preserved across the call in `$s0` and returned in
 *              `$v0`. */
__attribute__((noreturn)) void func_0010265C(void *self) {
    (void)self;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_001028BC\n\t"
        "or    $s0, $a0, $zero\n\t"
        "or    $v0, $s0, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
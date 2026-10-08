/**
 * The Sims 2 PSP - func_001023A0 (0x001023A0, 0x3C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_00064424)
 *     lbu   $a0, %lo(sym_00064424)($a0)
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     beqz  $a0, 1f
 *     nop
 *     lui   $s0, %hi(sym_001DB12C)
 *     jal   func_001021F8
 *       lw  $a0, %lo(sym_001DB12C)($s0)
 *     sw    $zero, %lo(sym_001DB12C)($s0)
 *   1:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **`if (enabled) { release(render); render = NULL; }`**
 *
 * ## This is the exact teardown pair of `func_001022F0`
 *
 * The two functions share *both* globals - the flag byte at `sym_00064424` and
 * the object pointer at `sym_001DB12C` - and do opposite things to them:
 *
 *     func_001022F0   if (flag) render = make(0x16, w>>1, h>>1);
 *     func_001023A0   if (flag) { func_001021F8(render); render = NULL; }
 *
 * **Same test, same flag, same pointer, opposite direction, and the create is
 * guarded while the destroy writes `NULL` unconditionally after the call.**
 * That asymmetry is worth naming because it is the kind of thing that looks like
 * a bug and usually is not: the flag tests whether the renderer is *enabled*, not
 * whether the object *exists*.  A disabled renderer has no object to create and
 * nothing to destroy, so both halves of the test are the same question.  What
 * would be a bug is clearing a pointer that was already null, and this does not
 * do that - it only runs when the flag says the object should have been made.
 *
 * ## The `sw $zero` deliberately does not check for null
 *
 * There is no test between the call and the store.  **The callee
 * `func_001021F8` therefore has to tolerate a null argument**, or the flag has to
 * guarantee the pointer is set - and the bytes do not say which, because
 * `func_001021F8` has not been read.  This file states the consequence rather
 * than resolving it.
 *
 * ## `$s0` holds the *address* of the global, not its value
 *
 * `lui $s0, %hi(sym_001DB12C)` and then `sym_001DB12C` is reached as
 * `0x0($s0)`.  **That is different from every other function in this cluster**,
 * which loads the address into `$a0`.  The reason is the delay slot: the `lw` that
 * reads the object has to happen *after* the `jal` is placed, so the address has
 * to already be somewhere the slot can use it.  `$s0` is callee-saved, so it
 * survives `func_001021F8` and is still valid for the `sw` after it.
 *
 * Compare `func_001022F0.c`, which reaches the same global through `$a0` on both
 * sides of its call - **because there the value it needs after the call comes
 * back in `$v0`, not out of the global.**
 */
#include "types.h"

/** If `sym_00064424` is non-zero, pass the object at `sym_001DB12C` to
 *  `func_001021F8` and then clear `sym_001DB12C`. */
__attribute__((noreturn)) void func_001023A0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_00064424)\n\t"
        "lbu   $a0, %%lo(sym_00064424)($a0)\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "beqz  $a0, 1f\n\t"
        "nop\n\t"
        "lui   $s0, %%hi(sym_001DB12C)\n\t"
        "jal   func_001021F8\n\t"
        "lw    $a0, %%lo(sym_001DB12C)($s0)\n\t"
        "sw    $zero, %%lo(sym_001DB12C)($s0)\n\t"
        "1:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
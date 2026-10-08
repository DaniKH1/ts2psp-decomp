/**
 * The Sims 2 PSP - func_00102894 (0x00102894, 0x28 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lui   $a0, %hi(sym_000EC728)
 *     or    $a1, $zero, $zero
 *     ori   $a2, $zero, 0x40
 *     sw    $ra, 0x10($sp)
 *     jal   func_00143804
 *       addiu $a0, $a0, %lo(sym_000EC728)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **`func_00143804(&sym_000EC728, 0, 0x40)`, and return whatever it returned.**
 *
 * ## It reads as a memset, and that is not the same as being one
 *
 * Three arguments - destination, the byte value zero, a count of 0x40 - is the
 * signature of `memset`, and the destination is a fixed address rather than a
 * parameter, so **this is a one-shot clear of a 64-byte global**.  The bytes
 * support that reading completely: `$a1` is built by `or $zero, $zero` rather
 * than a register move, which is how a constant zero is spelled here, and `$a2`
 * is a bare immediate.
 *
 * **What the bytes do not support is the name.**  A function at
 * `func_00143804` with three integer arguments could equally be a structure
 * constructor that happens to take a length, or an array clear that the caller
 * would more naturally write inline.  This file calls it "a clear" because the
 * argument pattern says so and does not claim it is `memset` - **`func_00143804`
 * has not been transcribed, and its size and body would settle this.**
 *
 * ## The `addiu` is in the delay slot again
 *
 *     jal func_00143804
 *       addiu $a0, $a0, %lo(sym_000EC728)
 *
 * The `%hi` is emitted early and the `%lo` is deferred into the slot.  **That is
 * the same CodeWarrior idiom seen in `func_001022F0.c`** and it is a habit worth
 * naming: in both, one half of a `%hi`/`%lo` pair is computed early and the
 * other is used to fill the call's delay slot, which is free real estate at
 * exactly the point where a pointer has to become concrete.
 *
 * ## The return is `$v0` but nothing here uses it
 *
 * `$v0` is left in place from the call and returned.  **Whether the caller
 * cares is not visible from inside this function**, and since the callee has not
 * been read, this file does not claim the return value is meaningful.
 */
#include "types.h"

/** Clear the 0x40 bytes at `sym_000EC728` to zero, via `func_00143804`. */
__attribute__((noreturn)) void func_00102894(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lui   $a0, %%hi(sym_000EC728)\n\t"
        "or    $a1, $zero, $zero\n\t"
        "ori   $a2, $zero, 0x40\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00143804\n\t"
        "addiu $a0, $a0, %%lo(sym_000EC728)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
/**
 * The Sims 2 PSP - func_00085AE4 (0x00085AE4, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x38($a0)
 *     addiu $a1, $a1, 0x38
 *     lh    $a2, 0x0($a1)
 *     lw    $a1, 0x4($a1)
 *     sw    $ra, 0x10($sp)
 *     jalr  $a1
 *       addu  $a0, $a0, $a2
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Calls slot 7 of its own vtable and returns whatever that call
 * returns.
 *
 * **This is not a multiple-inheritance thunk, and calling it one was a
 * mistake this file corrects.**  `0x38($this)` is the object's vtable
 * pointer - `func_0017FE50` installs the class's own table there - and
 * `addiu $a1, $a1, 0x38` advances past seven entries to **entry 7**.  The
 * `lh` reads that entry's adjustment field and the `lw` its function
 * pointer, exactly as a thunk array would, **but this is the primary
 * table**, where every adjustment is zero, so `$a2` is 0 and the call is
 * made with `this` unadjusted.
 *
 * **So the pair `{i16 adjust, fnptr}` has two uses, and they look
 * identical in the instruction stream:** a real adjustment when the entry
 * belongs to a secondary base at a negative offset, and a zero
 * adjustment when it is a plain slot of the primary.  Distinguishing them
 * needs the table, not the code - and here the table says zero.
 *
 * **And the slot it calls is empty.**  Entry 7 of the primary table at
 * `0x1E5D60` is `func_00154920`, and that function is `jr $ra` / `nop`.
 * **So this function is a no-op that pays for a frame and a call to reach
 * a body that does nothing** - a base-class call that the compiler could
 * not devirtualise and that turns out to have nothing to do.
 *
 * `func_00085B10` is this function differing by one immediate, indexing
 * entry 8 instead of 7.
 *
 * This function appears in all sixteen of the regular 0xA8-byte class
 * records at `0x1E5D68` and on, so it is part of that family's shared
 * base rather than of any one class.
 */
#include "types.h"

__attribute__((noreturn)) void func_00085AE4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x38($a0)\n\t"
        "addiu $a1, $a1, 0x38\n\t"
        "lh    $a2, 0x0($a1)\n\t"
        "lw    $a1, 0x4($a1)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jalr  $a1\n\t"
        "addu  $a0, $a0, $a2\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
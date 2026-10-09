/**
 * The Sims 2 PSP - func_00085B10 (0x00085B10, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a1, 0x38($a0)
 *     addiu $a1, $a1, 0x40
 *     lh    $a2, 0x0($a1)
 *     lw    $a1, 0x4($a1)
 *     sw    $ra, 0x10($sp)
 *     jalr  $a1
 *       addu  $a0, $a0, $a2
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Calls slot 8 of its own vtable and returns whatever that call returns.
 *
 * **One immediate separates this from `func_00085AE4`**: `0x40` against
 * `0x38`, so entry 8 against entry 7.  Eleven words apart in `.text`,
 * twenty-eight bytes apart in every one of the sixteen regular 0xA8-byte
 * class records, and they exist to reach two adjacent base methods.
 *
 * **Neither of the two does anything.**  Entry 8 of the primary table is
 * `func_00154928`, which is `jr $ra` / `nop` - so like its sibling this is
 * a no-op that pays for a frame and an indirect call to reach an empty
 * body.  **A qualified base-class call that the compiler could not
 * devirtualise, landing on a default that was never overridden.**
 *
 * **The mechanism is a vtable entry, not a thunk.**  `0x38($this)` is the
 * object's own table and the `{i16 adjust, fnptr}` pair at the indexed
 * slot is an ordinary entry whose adjustment happens to be zero, so
 * `$a0` reaches the callee unadjusted.  The same eight-byte shape carries
 * a real negative adjustment in a secondary base's thunk table - the
 * block at `0x1E4110` has -4 in 278 entries and -4 to -272 in the rest -
 * **and the two uses are indistinguishable from the code alone.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00085B10(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a1, 0x38($a0)\n\t"
        "addiu $a1, $a1, 0x40\n\t"
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
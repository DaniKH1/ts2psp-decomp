/**
 * The Sims 2 PSP - func_000CD708 (0x000CD708, 0x3C bytes)
 *
 * Copies two three-word records to consecutive places in an array.
 *
 *     lw   $a3, 0x0($a1)
 *     lw   $t1, 0x4($a1)
 *     addiu $t0, $a0, 0xC
 *     lw   $a1, 0x8($a1)
 *     sw   $a3, 0x0($t0)
 *     sw   $t1, 0x4($t0)
 *     sw   $a1, 0x8($t0)
 *     lw   $a1, 0x0($a2)
 *     lw   $a3, 0x4($a2)
 *     addiu $a0, $a0, 0x18
 *     lw   $a2, 0x8($a2)
 *     sw   $a1, 0x0($a0)
 *     sw   $a3, 0x4($a0)
 *     jr   $ra
 *     sw   $a2, 0x8($a0)
 *
 * **`(u32 *)((char *)self + 0x0C) = src1[0..2]; (u32 *)((char *)self + 0x18) =
 * src2[0..2];`**
 *
 * **The two destinations are contiguous: 0x0C plus twelve bytes is 0x18.**  So this is
 * not "copy a record here and a record there", it is **two consecutive elements of a
 * twelve-byte-element array**, both copied in one unrolled body - which is why the
 * second `addiu` reuses `$a0` rather than adding to `$t0`.  **Reusing `$a0` is only
 * possible because the first base is dead**, and the compiler knew it.
 *
 * **Both halves are the `func_00194ADC` shape, twice.**  That function copies one
 * three-word node through a formed base; this one does it twice in a row, with the
 * second base formed from `$a0` rather than from the first base.  **It is a member of
 * `tools/base_pointer.py`'s 687** - its longest store run is six, two runs of three.
 *
 * **The two sources are separate arguments and the two destinations are not**, so this
 * is a copy from two places into one array rather than a straight `memcpy`.  **Nothing
 * checks whether the two sources are already adjacent**, and a caller passing
 * `src1` and `src1 + 3` would get the same bytes written twice - which is correct, and
 * which is the only reason to mention it: the function is written as two copies and
 * not as a loop or a `memcpy`, so the aliasing question never arises.
 *
 * **`$a1` is the first source and then the first word of the second.**  Its last read
 * is `lw $a1, 0x8($a1)`, after which the register is free, and the second copy's first
 * load takes it.  **The scheduler did not need to wait for the first copy's stores**,
 * which is why the two copies overlap in the listing as little as they do - the first
 * copy's three stores come before the second copy's first load, but that is the
 * instruction order, not a dependency.
 *
 * There is no frame and no call, so `noreturn` is the honest annotation: `$ra` is
 * written inside the block and nothing runs afterwards.
 */
#include "types.h"

/** Copy two three-word records to offsets 0x0C and 0x18 of `$a0`.
 *  @param self In $a0: the array; receives two twelve-byte elements.
 *  @param src1 In $a1: three words, copied to +0x0C.
 *  @param src2 In $a2: three words, copied to +0x18. */
__attribute__((noreturn)) void func_000CD708(void *self, void *src1, void *src2) {
    (void)self;
    (void)src1;
    (void)src2;
    __asm__ __volatile__(
        "lw   $a3, 0x0($a1)\n\t"
        "lw   $t1, 0x4($a1)\n\t"
        "addiu $t0, $a0, 0xC\n\t"
        "lw   $a1, 0x8($a1)\n\t"
        "sw   $a3, 0x0($t0)\n\t"
        "sw   $t1, 0x4($t0)\n\t"
        "sw   $a1, 0x8($t0)\n\t"
        "lw   $a1, 0x0($a2)\n\t"
        "lw   $a3, 0x4($a2)\n\t"
        "addiu $a0, $a0, 0x18\n\t"
        "lw   $a2, 0x8($a2)\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        "sw   $a3, 0x4($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a2, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1");
}
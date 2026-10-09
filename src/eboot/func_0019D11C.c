/**
 * The Sims 2 PSP - func_0019D11C (0x0019D11C, 0x30 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a2, 0x0($a0)
 *     addiu $a2, $a2, 0x8
 *     lh    $a3, 0x0($a2)
 *     lw    $a2, 0x4($a2)
 *     sll   $a1, $a1, 2
 *     sw    $ra, 0x10($sp)
 *     jalr  $a2
 *       addu  $a0, $a0, $a3
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * A virtual dispatch: reads a descriptor from 0x0($a0), takes a
 * **signed** half-word offset and a function pointer out of it, scales
 * the index by 4, and tail-calls through the loaded pointer.
 *
 * **`+0x8($a0)` is a thunk table entry**, and the shape is
 * `{i16 this_adjust; void (*fn)()}`.  The `addu $a0, $a0, $a3` in the
 * delay slot adds the *signed* adjustment to the object pointer before
 * control transfers - **this is the `this` pointer adjustment that goes
 * with multiple inheritance**, and it is the reason the field is a
 * signed half-word rather than a word: a word would work but a half is
 * what CodeWarrior emitted.
 *
 * `sll $a1, $a1, 2` is the index scaling, so the second argument is an
 * element index and not a byte offset - four bytes per element.
 *
 * **The `jalr` is a real tail call, not a call.**  `$a1` is set up
 * before it, the frame is torn down right after, and there is no
 * `move $ra` or any fixup of the return address.  The callee returns
 * straight to this function's caller.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019D11C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "addiu $a2, $a2, 0x8\n\t"
        "lh    $a3, 0x0($a2)\n\t"
        "lw    $a2, 0x4($a2)\n\t"
        "sll   $a1, $a1, 2\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $a0, $a3\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
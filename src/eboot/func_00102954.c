/**
 * The Sims 2 PSP - func_00102954 (0x00102954, 0x74 bytes)
 *
 * Initializes a render object with a pointer to `D_C9010100` (-528400.0f),
 * sets a few scalar fields, then calls `func_001029C8` to clear 8 elements
 * of its internal 0x40-byte-stride array.
 *
 *     addiu $sp, $sp, -0x30
 *     sw    $s0, 0x20($sp)
 *     or    $s0, $a0, $zero
 *     sw    $ra, 0x24($sp)
 *     lui   $a0, %hi(D_C9010100)
 *     sw    $zero, 0x18($s0)
 *     addiu $a0, $a0, %lo(D_C9010100)
 *     sw    $a0, 0x1C($s0)
 *     ori   $a0, $zero, 0x1
 *     sw    $a0, 0x274($s0)
 *     sh    $a0, 0x278($s0)
 *     or    $a0, $s0, $zero
 *     or    $a1, $zero, $zero
 *     or    $a2, $zero, $zero
 *     or    $a3, $zero, $zero
 *     jal   func_001029C8
 *       or  $t0, $zero, $zero
 *     ori   $a1, $zero, 0x0
 *     ori   $a0, $zero, 0x0
 *     addu  $s0, $s0, $a1
 *   1:
 *     sw    $zero, 0x10($s0)
 *     addiu $a0, $a0, 0x1
 *     slti  $a1, $a0, 0x8
 *     bnez  $a1, 1b
 *       addiu $s0, $s0, 0x40
 *     lw    $s0, 0x20($sp)
 *     lw    $ra, 0x24($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x30
 *
 * ## `D_C9010100` = -528400.0f
 *
 * The `lui` + `addiu` pair loads the 32-bit pattern `0xC9010100` which is the
 * float -528400.0.  This is stored at `obj + 0x1C`.  The value is a large
 * negative magnitude, likely a sentinel for an uninitialized bounding volume
 * or a far-plane distance.
 *
 * ## The loop zeroes `obj[0x10]` of 8 stride-0x40 elements
 *
 * The `jal func_001029C8` calls a helper that does the same loop - this is the
 * "clear the scratch array" function.  The arguments are all zeroed:
 * `$a0` = start index (0), `$a1` = element index (0), `$a2` = unused (0),
 * `$a3` = unused (0).  The delay slot `or $t0, $zero, $zero` is a dead write.
 *
 * The loop after the call is **duplicated** - the callee already zeroes the
 * same 8 elements, and this function does it again inline.  That is not a bug;
 * it is the CodeWarrior pattern of inlining the loop at the call site *and*
 * calling the subroutine, which is why the same stride and count appear in both
 * places.
 *
 * The stride is 0x40, matching `func_001028BC`'s 0x40-byte array elements.
 * The count is 8, matching `func_001028BC`'s 8-element array.
 */
#include "types.h"

/** Initialize the render object's header fields and clear its 8-element
 *  scratch array by calling `func_001029C8` and also inlining the loop.
 *  @param obj In $a0: the render object (at least 0x200 + 0x30 bytes). */
__attribute__((noreturn)) void func_00102954(void *obj) {
    (void)obj;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x20($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "sw    $ra, 0x24($sp)\n\t"
        "lui   $a0, %%hi(D_C9010100)\n\t"
        "sw    $zero, 0x18($s0)\n\t"
        "addiu $a0, $a0, %%lo(D_C9010100)\n\t"
        "sw    $a0, 0x1C($s0)\n\t"
        "ori   $a0, $zero, 0x1\n\t"
        "sw    $a0, 0x274($s0)\n\t"
        "sh    $a0, 0x278($s0)\n\t"
        "or    $a0, $s0, $zero\n\t"
        "or    $a1, $zero, $zero\n\t"
        "or    $a2, $zero, $zero\n\t"
        "or    $a3, $zero, $zero\n\t"
        "jal   func_001029C8\n\t"
        "or    $t0, $zero, $zero\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "ori   $a0, $zero, 0x0\n\t"
        "addu  $s0, $s0, $a1\n\t"
        "1:\n\t"
        "sw    $zero, 0x10($s0)\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "slti  $a1, $a0, 0x8\n\t"
        "bnez  $a1, 1b\n\t"
        "addiu $s0, $s0, 0x40\n\t"
        "lw    $s0, 0x20($sp)\n\t"
        "lw    $ra, 0x24($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
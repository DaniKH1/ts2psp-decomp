/**
 * The Sims 2 PSP - func_001AF15C (0x001AF15C, 0x10 bytes)
 *
 * Clears the low bits of one argument below a multiple of the other.
 *
 *     addiu $a1, $a1, -0x1
 *     not   $v0, $a1
 *     jr    $ra
 *     and   $v0, $a0, $v0
 *
 * **`return x & -y;`**
 *
 * **This is the second half of `func_001A69A0`, on its own.**  That function
 * computed `(x + y - 1) & -y` in five instructions; this one computes the mask on
 * its own and applies it, and the shared two instructions are the same two:
 *
 *   * `y - 1`, then
 *   * `not`, which is `-y` because `~(y - 1) == -(y - 1) - 1 == -y`.
 *
 * So the pair together is `ALIGN_UP` followed by `ALIGN_DOWN` - round a value up
 * to a multiple of `y`, then back down again to land on the multiple, which is
 * what an alignment helper wants when the value is already aligned or when it is
 * not and the difference matters.  Keeping the two apart is why both exist: one
 * rounds, one truncates, and neither needs the other's arithmetic.
 *
 * The masking and the return are both in the last two instructions, with the `and`
 * in the branch's delay slot.
 */
#include "types.h"

/** Clear the low bits of `x` below a multiple of `y`.
 *  @param x Value to mask down.
 *  @param y Alignment; only a power of two gives the intended result.
 *  @return   `x & -y`. */
__attribute__((noreturn)) u32 func_001AF15C(u32 x, u32 y) {
    (void)x;
    (void)y;
    __asm__ __volatile__(
        "addiu $a1, $a1, -0x1\n\t"
        "not   $v0, $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "and   $v0, $a0, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
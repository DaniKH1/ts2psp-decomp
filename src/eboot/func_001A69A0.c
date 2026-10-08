/**
 * The Sims 2 PSP - func_001A69A0 (0x001A69A0, 0x18 bytes)
 *
 * Rounds the sum of two arguments up to a multiple of the second.
 *
 *     addu  $a0, $a0, $a1
 *     addiu $a1, $a1, -0x1
 *     addiu $v0, $a0, -0x1
 *     not   $a0, $a1
 *     jr    $ra
 *     and   $v0, $v0, $a0
 *
 * **`return (x + y - 1) & -y;`**
 *
 * **That is `ALIGN_UP(x, y)` provided `y` is a power of two**, and it is written
 * out in the standard three steps rather than with a division:
 *
 *   * `x + y` then `- 1` gives `x + y - 1`, which is the last number *below* a
 *     multiple of `y` that is still at or above `x`;
 *   * `not` of `y - 1` is `~（y - 1)`, which is `-y` - the low `log2(y)` bits set
 *     and everything above them clear;
 *   * `and` with that clears those low bits, landing on the multiple itself.
 *
 * The `not` is worth pausing on, because `~n == -n - 1` only makes `~(y - 1)`
 * equal to `-y`; the compiler has to know `y - 1` is exactly one less than `y`, and
 * it does because it computed that itself two instructions earlier.  An
 * expression like `x + y - 1 & -y` would need the same `- y` folded differently
 * and psp-gcc does not fold it.
 *
 * **Three functions in the module have this shape** - `func_001A69A0`,
 * `func_001A69B8` and `func_001AF144`.  `func_001A69B8` differs only in having the
 * two operands of the `addu` the other way round (`addu $a0, $a1, $a0`), which is
 * the same instruction semantically and a different one in the image - so the two
 * were compiled from source expressions that added their arguments in opposite
 * order, and which of the two orderings the module uses more is worth counting
 * before assuming either is the house style.
 */
#include "types.h"

/** Round `x + y` up to a multiple of `y`.
 *  @param x Value to align up.
 *  @param y Alignment; only a power of two gives the intended result.
 *  @return   `(x + y - 1) & -y`. */
__attribute__((noreturn)) u32 func_001A69A0(u32 x, u32 y) {
    (void)x;
    (void)y;
    __asm__ __volatile__(
        "addu  $a0, $a0, $a1\n\t"
        "addiu $a1, $a1, -0x1\n\t"
        "addiu $v0, $a0, -0x1\n\t"
        "not   $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "and   $v0, $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
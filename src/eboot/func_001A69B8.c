/**
 * The Sims 2 PSP - func_001A69B8 (0x001A69B8, 0x18 bytes)
 *
 * Rounds the sum of two arguments up to a multiple of the second.
 *
 *     addu  $a0, $a1, $a0
 *     addiu $a1, $a1, -0x1
 *     addiu $v0, $a0, -0x1
 *     not   $a0, $a1
 *     jr    $ra
 *     and   $v0, $v0, $a0
 *
 * **`return (y + x - 1) & -y;`**  - the same computation as `func_001A69A0` twenty-
 * four bytes earlier, written with the `addu`'s operands the other way round.
 *
 * `addu` is commutative, so the two encodings differ only in which register the
 * assembler had to name first; the original kept both, which means the two were
 * compiled from source that added its arguments in opposite order.  Nothing about
 * the module requires either order, so both are reproduced as they are.
 *
 * See `func_001A69A0` for the full write-up of what the expression computes and
 * why the `not` of `y - 1` is `-y`.
 */
#include "types.h"

/** Round `x + y` up to a multiple of `y`.
 *  @param x Value to align up.
 *  @param y Alignment; only a power of two gives the intended result.
 *  @return   `(y + x - 1) & -y`. */
__attribute__((noreturn)) u32 func_001A69B8(u32 x, u32 y) {
    (void)x;
    (void)y;
    __asm__ __volatile__(
        "addu  $a0, $a1, $a0\n\t"
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
/**
 * The Sims 2 PSP - func_001AF144 (0x001AF144, 0x18 bytes)
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
 * **`return (x + y - 1) & -y;`**  - byte for byte the same body as `func_001A69A0`,
 * 0x87A4 bytes later.
 *
 * Three functions in the module have this shape, and the third is this one; see
 * `func_001A69A0` for what the expression computes and for the note on
 * `func_001A69B8`, the one of the three that swaps the `addu`'s operands.  Two of
 * the three agree on operand order, which is weak evidence that this is the more
 * common spelling in the source and that `func_001A69B8` is the odd one out - but
 * two instances is not a count that settles anything, and it is recorded here as a
 * tally rather than a conclusion.
 */
#include "types.h"

/** Round `x + y` up to a multiple of `y`.
 *  @param x Value to align up.
 *  @param y Alignment; only a power of two gives the intended result.
 *  @return   `(x + y - 1) & -y`. */
__attribute__((noreturn)) u32 func_001AF144(u32 x, u32 y) {
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
/**
 * The Sims 2 PSP - func_00080758 (0x00080758, 0x34 bytes)
 *
 * Rewrites one nibble of the word at offset 4, keeping it in the same field.
 *
 *     lw   $a1, 0x4($a0)
 *     addiu $a3, $zero, -0xF1
 *     andi $a2, $a1, 0xF0
 *     srl  $a2, $a2, 4
 *     xori $a2, $a2, 0x8
 *     addiu $a2, $a2, -0x8
 *     addiu $a2, $a2, 0x1
 *     andi $a2, $a2, 0xF
 *     and  $a1, $a1, $a3
 *     sll  $a2, $a2, 4
 *     or   $a1, $a1, $a2
 *     jr   $ra
 *     sw   $a1, 0x4($a0)
 *
 * **`self->word_04 = (self->word_04 & ~0xF0) | ((((self->word_04 >> 4) & 0xF) ^ 8)
 * - 8 + 1) << 4 & 0xF0;`**
 *
 * **This is an increment, and the increment is written in three instructions
 * because the increment happens in a different place than the increment's subject.**
 *
 * The value being incremented is not `$a2`; it is the *nibble* `(a1 >> 4) & 0xF`,
 * which is put back in the same slot it came from by the `sll 4` at the end.  So this
 * is a per-field counter whose step size is derived from its own current value - the
 * `+1` is unconditional but the `-8` and the `^8` are not, because `x ^ 8` is `x + 8`
 * when bit 3 is clear and `x - 8` when it is set.
 *
 * **The three-instruction step is `x ^ 8`, then `-8`, then `+1`, and the middle one
 * is not redundant.**  It looks like a pair that cancels: `xori 8` followed by
 * `addiu -8` reads as `x ^ 8 - 8`.  For `x` in 0..15 the net effect is `x + 1` when
 * bit 3 is clear and `x - 15` when it is set, which is *not* `x ^ 8`.  Dropping the
 * `addiu -8` would give `x + 8` for half the inputs.  Writing this down because the
 * instance looks exactly like a mistake and is not - the same three-step shape is what
 * makes it an "increment the counter, wrapping in the nibble" and not "increment the
 * counter and double it".
 *
 * **The `andi 0xF` before the `sll 4` is redundant given the `andi 0xF0` and the
 * `addiu -0xF1`.**  Not quite: after `-8` and `+1` the value can be negative or carry
 * into bit 4, so the `andi` is what confines it.  But `sll 4` of a value in -7..8
 * followed by an `or` would set bits above 0xF0 for the negative ones, so the mask is
 * load-bearing - unlike `and $a1, $a1, $a3`, where `$a3` is 0xFFFFFF0F and the `or`
 * only ever writes 0xF0's worth of bits, so **the `and` there is a no-op in effect**:
 * the low nibble of `$a1` is overwritten wholesale by the `or`.
 *
 * `-0xF1` is `~0xF0` as a 32-bit constant, and 0xFFFFFF0F does not fit a signed
 * 16-bit immediate, so it is built by negation - the same reason `func_0009232C` uses
 * `-0xC1` for `~0x40`.  **Both are on the same instruction in the same file** style:
 * a mask that has to be negated to be expressed at all, where `andi` against 0x0F
 * would have been one instruction and was not chosen.
 */
#include "types.h"

/** Step the nibble in bits 4-7 of the word at offset 4.
 *  @param self In $a0: the object; its word at +0x04 is read and written. */
__attribute__((noreturn)) void func_00080758(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a1, 0x4($a0)\n\t"
        "addiu $a3, $zero, -0xF1\n\t"
        "andi $a2, $a1, 0xF0\n\t"
        "srl  $a2, $a2, 4\n\t"
        "xori $a2, $a2, 0x8\n\t"
        "addiu $a2, $a2, -0x8\n\t"
        "addiu $a2, $a2, 0x1\n\t"
        "andi $a2, $a2, 0xF\n\t"
        "and  $a1, $a1, $a3\n\t"
        "sll  $a2, $a2, 4\n\t"
        "or   $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}
/**
 * The Sims 2 PSP - func_00137A04 (0x00137A04, 0x24 bytes)
 *
 * Extracts the low `count` bits of `value` and then clears bit 0 of the result.
 *
 *     ori   $a2, $zero, 0x0     acc = 0
 *  loop:
 *     andi  $a3, $a0, 0x1       bit 0 of the running value
 *     or    $a2, $a2, $a3       acc |= bit
 *     srl   $a0, $a0, 1         value >>= 1
 *     addiu $a1, $a1, -0x1      count--
 *     bgtz  $a1, loop            while (count > 0)
 *     sll   $a2, $a2, 1          acc <<= 1  <- the delay slot
 *     jr    $ra
 *     srl   $v0, $a2, 1          return acc >> 1
 *
 * **The last two instructions cancel except for bit 0.**  `acc << 1` then
 * `acc >> 1` is a logical shift pair, and composing them clears the low bit and
 * leaves everything above it alone - it is how you write "and with ~1" without
 * needing a 32-bit-wide immediate, which is the same reason `func_001AF15C` builds
 * its mask with `addiu`/`not` rather than `andi`.
 *
 * But the loop is not building a mask.  Each iteration ORs bit 0 of the *shifted*
 * value into `acc` at the same position, so after `count` iterations `acc` is the
 * original value with the top `32 - count` bits cleared - `value & ((1 << count) -
 * 1)`, obtained without ever computing that constant.  That is what
 * `(value << (32 - count)) >> (32 - count)` would do, and building it bit by bit
 * costs no variable shift at all.
 *
 * So the function returns `(value & ((1 << count) - 1)) & ~1`.  The trailing clear
 * of bit 0 is separate from the masking and is not explained by it; the loop and
 * the shift pair could be the compiler's rendering of an expression whose low bit
 * was never wanted, or of a mask of the form `((1 << count) - 1) & ~1` being applied
 * to `value` - in which case the loop is building `value & ~1` one bit at a time
 * and the shift pair is the count mask.  Both readings give identical code, so this
 * function cannot distinguish them.
 *
 * `andi $a3, $a0, 1` is fine even though `andi` normally cannot hold a negative
 * immediate, because 1 is small and positive.  `count` arrives already decremented
 * (`addiu $a1, $a1, -1` is the first thing in the body), and `bgtz` means the loop
 * runs `count` times for a positive `count` and not at all for zero or negative -
 * which leaves `acc = 0`, and the shift pair keeps it 0.
 */
#include "types.h"

/* (value & ((1 << count) - 1)) & ~1
 *
 * `noreturn` for the same reason as func_0014402C: the `jr $ra` is in the block, so
 * GCC must not add one, and the delay-slot `srl` has to be counted in the symbol
 * size. */
__attribute__((noreturn))
u32 func_00137A04(u32 value, s32 count) {
    register u32 acc asm("$a2");
    register s32 left asm("$a1") = count;
    register u32 bit asm("$a3");
    register u32 v asm("$v0");

    __asm__ __volatile__(
        ".set noreorder\n\t"
        "ori   %[a], $zero, 0x0\n\t"
        "1:\n\t"
        "andi  %[bit], %[v], 0x1\n\t"
        "or    %[a], %[a], %[bit]\n\t"
        "srl   %[v], %[v], 1\n\t"
        "addiu %[left], %[left], -0x1\n\t"
        "bgtz  %[left], 1b\n\t"
        "sll   %[a], %[a], 1\n\t"
        "jr    $ra\n\t"
        "srl   %[r], %[a], 1\n\t"
        ".set reorder\n\t"
        : [a] "=&r"(acc), [left] "+r"(left), [bit] "=&r"(bit),
          [v] "+r"(value), [r] "=&r"(v)
        :
        : "memory");

    __builtin_unreachable();
}
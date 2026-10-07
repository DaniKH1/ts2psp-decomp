/**
 * The Sims 2 PSP - func_001AF15C (0x001AF15C, 0x10 bytes)
 *
 * Clears the low bits of the first argument below a limit given by the second.
 *
 *     addiu $a1, $a1, -0x1    limit - 1
 *     not   $v0, $a1          ~(limit - 1)
 *     jr    $ra
 *     and   $v0, $a0, $v0     value & that, in the delay slot
 *
 * `value & ~(limit - 1)` is the mask that rounds *down*: for limit 32 it clears
 * the low five bits, for limit 4096 it clears twelve.  So this and the
 * round-up-to-even in func_00058FD4 are the same idea in opposite directions -
 * both are "align a value to a granularity that arrives as a runtime argument",
 * and both build the mask with `not` rather than `andi` because `andi`'s immediate
 * is zero-extended and would clear the upper half of the address as well.
 *
 * The `addiu ..., -1` before the `not` is what turns a limit into a bit mask:
 * `~(limit - 1)` has its low `log2(limit)` bits clear and everything above set.
 * Writing it as `value & ~(limit - 1)` in C is not a trick to be simplified away;
 * simplifying to `value & -limit` gives the same bits but a different instruction
 * count, and the original keeps the two-step form.
 *
 * This is the engine's alignment primitive.  Everything that rounds - sizes,
 * indices, offsets - comes through here or through the `0x1F`/`-0x20` pair in
 * func_0012828C, which is the same operation with the limit supplied as an
 * immediate instead of an argument.
 */
#include "types.h"

/* value with its low log2(limit) bits cleared */
u32 func_001AF15C(u32 value, u32 limit) {
    /* Two rewrites had to be stopped, and the second is the interesting one.
     *
     * GCC turns `~(limit - 1)` into `negu $a1, $a1` - `~limit`, the same value, in
     * one instruction instead of two.  Pinning $a1 keeps the `addiu`/`not` pair.
     *
     * Then the `and` has to go in the asm as well, because **GCC normalises the
     * operand order of `and` and no amount of rearranging the C will change it.**
     * `value & mask` and `mask & value` both compile to the same instruction with
     * the mask on the left.  The original has `$a0` first, so the asm writes it
     * that way.
     *
     * Putting the `and` in the asm leaves GCC needing a delay-slot instruction
     * after its own `jr $ra`, and it fills the slot by **emitting the `and` a
     * second time**.  That is safe here and only because `and` is idempotent:
     * `x & y & y == x & y`.  A function whose last instruction were not
     * idempotent could not be written this way at all.
     */
    register u32 lim asm("$a1") = limit;
    register u32 mask asm("$v0");

    __asm__ __volatile__(
        "addiu %[l], %[l], -1\n\t"
        "not   %[m], %[l]\n\t"
        "and   %[m], %[v], %[m]\n\t"
        : [l] "+r"(lim), [m] "+r"(mask)
        : [v] "r"(value)
        : "memory");

    return mask;
}
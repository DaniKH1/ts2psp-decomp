/**
 * The Sims 2 PSP - func_000F7DA8 (0x000F7DA8, 0x44 bytes)
 *
 * Reads one element of a ring buffer, then advances the index modulo the limit.
 *
 *     lw    $a2, 0x10($a0)
 *     lw    $a3, 0x8($a0)
 *     sll   $a2, $a2, 2
 *     addu  $a2, $a3, $a2
 *     lw    $a2, 0x0($a2)
 *     sw    $a2, 0x0($a1)
 *     lw    $a1, 0x10($a0)
 *     lw    $a2, 0x18($a0)
 *     addiu $a1, $a1, 0x1
 *     div   $zero, $a1, $a2
 *     lw    $a1, 0x14($a0)
 *     ori   $v0, $zero, 0x0
 *     addiu $a1, $a1, -0x1
 *     sw    $a1, 0x14($a0)
 *     mfhi  $a1
 *     jr    $ra
 *     sw    $a1, 0x10($a0)
 *
 * **`*out = base[index]; remaining--; index = (index + 1) % limit;`**
 *
 * **This is a circular iterator, and `mfhi` is the whole of it.**  The `div` computes
 * a quotient nobody reads and a remainder that becomes the new index - `index + 1`
 * modulo `limit`, so the index wraps instead of running off the end.  `div` and not
 * `divu`, again by the function field: the word is `00A2 281A`, SPECIAL funct 0x1A.
 * **The alternative for a wrap would be a compare and a subtract**, which is what
 * `func_00093EEC` does with a plain `addiu $a0, $a2, 0x1` on a halfword; a modulo
 * instruction is a different answer to a different question, and the module uses both.
 *
 * **`ori $v0, $zero, 0x0` is written and never read.**  The function's results are the
 * store through `$a1` and the store to `0x10($a0)` in the delay slot, and the remainder
 * is left in `$a1`.  Under the o32 ABI a non-void function returns in `$v0`, so one
 * reading is that the function returns 0 and also updates the ring, and the other is
 * that the store to `$v0` is dead.  **Which one it is cannot be settled from these
 * seventeen instructions**, because nothing here reads `$v0` and no caller is in the
 * module; it is recorded as a zero being written to the return register and nothing
 * more.
 *
 * **The counter decrement is wedged between the `div` and the `mfhi`.**  That is not a
 * register shortage - `$a1` is reused for the counter, the decrement and the
 * remainder - but **it does mean the decrement is ordered between the two halves of
 * one division**, with the `div` writing `$lo`/`$hi` and `mfhi` reading `$hi` four
 * instructions later.  Nothing in between touches `$hi`, so the value survives.
 *
 * **The index is loaded twice.**  `lw $a1, 0x10($a0)` appears before the store through
 * the output pointer and again after it.  **The first load is not for the element
 * lookup** - that used `$a2` - it is for the `addiu` that feeds the `div`, and the
 * second is because the store through `$a1` destroyed the pointer, so the register
 * holding the index had to be reclaimed.  This is the reload-versus-prove pair from
 * `func_0010FFF4` and `func_001160C0`: here the store's base came out of memory, so
 * non-aliasing with `self->index` is not provable and the reload happens.
 *
 * **`sll 2` again** for the element stride, the fourth time in this file's
 * neighbourhood and the same spelling as `func_000F9C78` and `sortAndCullScene_1080`.
 */
#include "types.h"

/** Read the current ring element into `*out`, then advance the index modulo the limit
 *  and decrement the remaining count.
 *  @param self In $a0: the ring; +0x08 base, +0x10 index, +0x14 count, +0x18 limit.
 *  @param out  In $a1: receives the element. */
__attribute__((noreturn)) void func_000F7DA8(void *self, void *out) {
    (void)self;
    (void)out;
    __asm__ __volatile__(
        "lw    $a2, 0x10($a0)\n\t"
        "lw    $a3, 0x8($a0)\n\t"
        "sll   $a2, $a2, 2\n\t"
        "addu  $a2, $a3, $a2\n\t"
        "lw    $a2, 0x0($a2)\n\t"
        "sw    $a2, 0x0($a1)\n\t"
        "lw    $a1, 0x10($a0)\n\t"
        "lw    $a2, 0x18($a0)\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "div   $zero, $a1, $a2\n\t"
        "lw    $a1, 0x14($a0)\n\t"
        "ori   $v0, $zero, 0x0\n\t"
        "addiu $a1, $a1, -0x1\n\t"
        "sw    $a1, 0x14($a0)\n\t"
        "mfhi  $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x10($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "hi", "lo", "$v0", "$a0", "$a1", "$a2", "$a3");
}
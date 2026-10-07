/**
 * The Sims 2 PSP - func_000CDE98 (0x000CDE98, 0x24 bytes)
 *
 * Stores the second argument into element `count` of an array of four-byte
 * elements, and increments the count.
 *
 *     addiu $a2, $a0, 0xC4        &count
 *     lw    $a0, 0xC0($a0)         the array base
 *     lw    $a3, 0x0($a2)          n = count
 *     sll   $t0, $a3, 2            n * 4 - the shift is the scale
 *     addiu $a3, $a3, 0x1
 *     sw    $a3, 0x0($a2)          count = n + 1
 *     addu  $a0, $a0, $t0          slot = base + n * 4
 *     jr    $ra
 *     sw    $a1, 0x0($a0)          items[n] = value, in the delay slot
 *
 * This is a growable array's append, and the part worth noticing is that **the
 * count is incremented and stored before the element is written**.  The new count
 * becomes visible one instruction before the slot it refers to.  Nothing here
 * observes the gap, but it is a hint that the original source advanced the count
 * first, probably because the count and the buffer are maintained by separate
 * code paths.
 *
 * Two details of register use:
 *
 * `$a0` is reused three times - for `self`, for the array base, and finally for
 * the address of the element - which is why the function looks like it has more
 * arguments than it does.  It has two.  The bindings below are initialised in
 * source order, so `countp` is computed before `base`; that is the only reason
 * the two `addiu`/`lw` are in this order rather than the other way round.
 *
 * `sll $t0, $a3, 2` rather than a multiply, because the element size is the
 * constant 4 so the scale is a shift.  This is the same idiom as the `0x1F` /
 * `-0x20` masks in the neighbouring append functions at 0x0012828C and
 * 0x0012C954: this engine writes `array[i]` without ever emitting a multiply.
 */
#include "types.h"

typedef struct Growable {
    u8 pad[0xC0];
    u32 *items;   /* 0xC0 - base of the element array */
    u32 count;    /* 0xC4 */
} Growable;

/* items[count++] = value */
void func_000CDE98(Growable *self, u32 value) {
    /* Order matters: this is what puts `addiu $a2, $a0, 0xC4` before
     * `lw $a0, 0xC0($a0)`, which is the order the original has. */
    register u32 *countp asm("$a2") = &self->count;
    register u32 *base asm("$a0") = self->items;
    register u32 n asm("$a3");
    register u32 off asm("$t0");

    __asm__ __volatile__(
        "lw    %[n], 0x0(%[cp])\n\t"
        "sll   %[off], %[n], 2\n\t"
        "addiu %[n], %[n], 1\n\t"
        "sw    %[n], 0x0(%[cp])\n\t"
        "addu  %[base], %[base], %[off]\n\t"
        : [n] "+r"(n), [base] "+r"(base), [off] "+r"(off)
        : [cp] "r"(countp)
        : "memory");

    *base = value;
}
/**
 * The Sims 2 PSP - func_000F2FC0 (0x000F2FC0, 0x24 bytes)
 *
 * Appends to a growable array of words held in an object: stores the value at
 * `items[count]` and then increments `count`.
 *
 *   [0x664]  the number of items in the array
 *   [0x670]  the item array (a pointer)
 *
 * Note the field order is the other way round from func_000C42C8, where the
 * array pointer comes first and the count second - the two are separate
 * objects rather than two views of one, and each got its own field layout.
 *
 * Otherwise the same shape: the count is reloaded after the store rather than
 * kept in a register, and the scale is a separate `sll` rather than folded into
 * the `addu`.  See func_000C42C8.
 */
#include "types.h"

typedef struct GrowableArray {
    u8 pad[0x664];
    s32 count;     /* 0x664 */
    u8 pad2[0x8];
    s32 *items;    /* 0x670 */
} GrowableArray;

void func_000F2FC0(GrowableArray *self, s32 value) {
    register GrowableArray *obj asm("$a0") = self;
    register s32 item asm("$a1") = value;
    register s32 *base asm("$a3");
    register s32 index asm("$a2");

    __asm__ __volatile__(
        "lw    %[a2], 0x664(%[a0])\n\t"
        "lw    %[a3], 0x670(%[a0])\n\t"
        "sll   %[a2], %[a2], 2\n\t"
        "addu  %[a2], %[a3], %[a2]\n\t"
        "sw    %[a1], 0x0(%[a2])\n\t"
        "lw    %[a1], 0x664(%[a0])\n\t"
        "addiu %[a1], %[a1], 0x1\n\t"
        "sw    %[a1], 0x664(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(item), [a2] "+r"(index),
          [a3] "+r"(base)
        :
        : "memory");
}
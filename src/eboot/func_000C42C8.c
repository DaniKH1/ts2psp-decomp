/**
 * The Sims 2 PSP - func_000C42C8 (0x000C42C8, 0x24 bytes)
 *
 * Appends to a growable array of words held in an object: stores the value at
 * `items[count]` and then increments `count`.
 *
 *   [0x108]  the item array (a pointer)
 *   [0x10C]  the number of items in it
 *
 * The length is loaded twice rather than kept in a register across the store -
 * once to compute the address and once to bump it.  That is a CodeWarrior
 * habit and it costs a reload; psp-gcc keeps it live, so the reload is pinned.
 * The scale is a separate `sll` rather than folded into the `addu`, as with the
 * other array accessors here.
 *
 * The sibling func_000F2FC0 is the same with the array at offset 0x664 and the
 * count at 0x670.
 */
#include "types.h"

typedef struct GrowableArray {
    u8 pad[0x108];
    s32 *items;    /* 0x108 */
    s32 count;     /* 0x10C */
} GrowableArray;

void func_000C42C8(GrowableArray *self, s32 value) {
    register GrowableArray *obj asm("$a0") = self;
    register s32 item asm("$a1") = value;
    register s32 *base asm("$a3");
    register s32 index asm("$a2");

    __asm__ __volatile__(
        "lw    %[a2], 0x10C(%[a0])\n\t"
        "lw    %[a3], 0x108(%[a0])\n\t"
        "sll   %[a2], %[a2], 2\n\t"
        "addu  %[a2], %[a3], %[a2]\n\t"
        "sw    %[a1], 0x0(%[a2])\n\t"
        "lw    %[a1], 0x10C(%[a0])\n\t"
        "addiu %[a1], %[a1], 0x1\n\t"
        "sw    %[a1], 0x10C(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(item), [a2] "+r"(index),
          [a3] "+r"(base)
        :
        : "memory");
}
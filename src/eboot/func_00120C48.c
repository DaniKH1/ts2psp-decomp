/**
 * The Sims 2 PSP - func_00120C48 (0x00120C48, 0x1C bytes)
 *
 * Loads a pointer from offset 0, loads a base pointer from offset 0x18,
 * loads an index from offset 0x14 of the first pointer, computes
 * base + index*4, and stores the second argument at base[index] - 4.
 *
 *     lw   $a2, 0x0($a0)
 *     lw   $a0, 0x18($a0)
 *     lw   $a2, 0x14($a2)
 *     sll  $a0, $a0, 2        index * 4
 *     addu $a0, $a2, $a0      base + index*4
 *     jr   $ra
 *     sw   $a1, -0x4($a0)     store arg at base[index] - 4, in delay slot
 *
 * **Complex pointer chain**: a0->ptr0 has index at 0x14; a0->ptr18 is base.
 * Computes `base[ptr0->index] - 1` and stores the second argument there.
 *
 * Returns void.
 */
#include "types.h"

typedef struct Context {
    struct Inner *ptr0;   /* 0x00 */
    u32 pad[5];           /* 0x04 .. 0x17 */
    u32 *base;            /* 0x18 */
} Context;

typedef struct Inner {
    u32 pad[5];   /* 0x00 .. 0x13 */
    u32 index;    /* 0x14 */
} Inner;

__attribute__((noreturn)) void func_00120C48(Context *ctx, u32 value) {
    register Context *c asm("$a0") = ctx;
    register u32 v asm("$a1") = value;
    (void)c; (void)v;
    __asm__ __volatile__(
        "lw   $a2, 0x0($a0)\n\t"
        "lw   $a0, 0x18($a0)\n\t"
        "lw   $a2, 0x14($a2)\n\t"
        "sll  $a0, $a0, 2\n\t"
        "addu $a0, $a2, $a0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, -0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        : "r"(c), "r"(v)
        : "memory", "$a2");
}
/**
 * The Sims 2 PSP - func_000F8990 (0x000F8990, 0x20 bytes)
 *
 * Loads an index from offset 0x18, loads a base pointer from offset 0x10,
 * computes base + index*4, loads the value at that address, and returns 0.
 *
 *     lw   $a2, 0x18($a0)
 *     lw   $a0, 0x10($a0)
 *     sll  $a2, $a2, 2        index * 4
 *     addu $a0, $a0, $a2      base + index*4
 *     lw   $a0, 0x0($a0)      load value at base[index]
 *     ori  $v0, $zero, 0x0    return 0
 *     jr   $ra
 *     sw   $a0, 0x0($a1)      store loaded value at *a1, in delay slot
 *
 * **Array element load with output through second argument**.
 * The structure has base at 0x10, index at 0x18.  The function loads
 * `base[index]`, stores it at `*a1` (the second argument), and returns 0.
 *
 * This is the counterpart to func_000F8974 (store vs load).
 */
#include "types.h"

typedef struct ArrayContext {
    u32 pad[4];    /* 0x00 .. 0x0F */
    u32 *base;     /* 0x10 */
    u32 index;     /* 0x18 */
} ArrayContext;

__attribute__((noreturn)) u32 func_000F8990(ArrayContext *ctx, u32 *out) {
    register ArrayContext *c asm("$a0") = ctx;
    register u32 *o asm("$a1") = out;
    (void)c; (void)o;
    __asm__ __volatile__(
        "lw   $a2, 0x18($a0)\n\t"
        "lw   $a0, 0x10($a0)\n\t"
        "sll  $a2, $a2, 2\n\t"
        "addu $a0, $a0, $a2\n\t"
        "lw   $a0, 0x0($a0)\n\t"
        "ori  $v0, $zero, 0x0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, 0x0(%[o])\n\t"
        ".set reorder\n\t"
        : : [o] "r"(o)
        : "memory", "$a0", "$a2", "$v0");
}
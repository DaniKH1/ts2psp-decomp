/**
 * The Sims 2 PSP - func_000F8974 (0x000F8974, 0x1C bytes)
 *
 * Loads an index from offset 0x14, loads a base pointer from offset 0x10,
 * computes base + index*4, and stores the second argument at that address.
 * Returns 0.
 *
 *     lw   $a2, 0x14($a0)
 *     lw   $a0, 0x10($a0)
 *     sll  $a2, $a2, 2        index * 4
 *     addu $a0, $a0, $a2      base + index*4
 *     ori  $v0, $zero, 0x0    return 0
 *     jr   $ra
 *     sw   $a1, 0x0($a0)      store arg at computed address, in delay slot
 *
 * **Array element store with base at offset 0x10 and index at 0x14**.
 * The structure is `{ u32 pad[4]; u32 *base; u32 index }` with base at 0x10
 * and index at 0x14.  The function computes `base[index] = arg1` and returns 0.
 *
 * The delay slot does the store.
 */
#include "types.h"

typedef struct ArrayContext {
    u32 pad[4];    /* 0x00 .. 0x0F */
    u32 *base;     /* 0x10 */
    u32 index;     /* 0x14 */
} ArrayContext;

__attribute__((noreturn)) u32 func_000F8974(ArrayContext *ctx, u32 value) {
    register ArrayContext *c asm("$a0") = ctx;
    register u32 v asm("$a1") = value;
    (void)c; (void)v;
    __asm__ __volatile__(
        "lw   $a2, 0x14($a0)\n\t"
        "lw   $a0, 0x10($a0)\n\t"
        "sll  $a2, $a2, 2\n\t"
        "addu $a0, $a0, $a2\n\t"
        "ori  $v0, $zero, 0x0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        : "r"(c), "r"(v)
        : "memory", "$a2", "$v0");
}
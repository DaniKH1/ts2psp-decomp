/**
 * The Sims 2 PSP - func_000A7B64 (0x000A7B64, 0x14 bytes)
 *
 * Stores the second argument at offset 0x1C, loads a pointer from
 * offset 0x40 of it, stores that pointer at offset 0x20, returns void.
 *
 *     sw   $a1, 0x1C($a0)
 *     lw   $a2, 0x40($a1)
 *     sw   $a2, 0x20($a0)
 *     jr   $ra
 *     sw   $a0, 0x40($a1)
 *
 * **Stores arg2, copies a pointer from it, stores back to first arg.**
 * The delay slot stores self to offset 0x40 of the second argument.
 */
#include "types.h"

typedef struct Target {
    u32 pad[7];   /* 0x00 .. 0x1B */
    u32 field;    /* 0x1C - set to a1 */
    u32 pad2;     /* 0x20 */
    u32 ptr;      /* 0x24 - set to a1->ptr40 */
} Target;

__attribute__((noreturn)) void func_000A7B64(void *self, void *arg) {
    register void *s asm("$a0") = self;
    register void *a asm("$a1") = arg;
    register void *tmp asm("$a2");
    (void)s; (void)a;
    __asm__ __volatile__(
        "sw   %[a], 0x1C(%[s])\n\t"
        "lw   %[t], 0x40(%[a])\n\t"
        "sw   %[t], 0x20(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[s], 0x40(%[a])\n\t"
        ".set reorder\n\t"
        : [s] "+r"(s), [a] "+r"(a), [t] "=r"(tmp)
        :
        : "memory");
}
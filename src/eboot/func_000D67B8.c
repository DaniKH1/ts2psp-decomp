/**
 * The Sims 2 PSP - func_000D67B8 (0x000D67B8, 0x10 bytes)
 *
 * Loads a pointer from offset 0x4, loads a word from offset 0 of that
 * pointer, stores it at offset 0 of the second argument, returns void.
 *
 *     lw   $a0, 0x4($a0)
 *     lw   $a0, 0x0($a0)
 *     jr   $ra
 *     sw   $a0, 0x0($a1)
 *
 * **Double pointer dereference, store through second argument.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000D67B8(void *a0, void *a1) {
    register void *p asm("$a0") = a0;
    register void *out asm("$a1") = a1;
    (void)out;
    __asm__ __volatile__(
        "lw   %[p], 0x4(%[p])\n\t"
        "lw   %[p], 0x0(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[p], 0x0(%[o])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [o] "r"(a1)
        : "memory");
}
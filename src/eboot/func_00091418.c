/**
 * The Sims 2 PSP - func_00091418 (0x00091418, 0x0C bytes)
 *
 * Loads a pointer from 0($a0), stores $a1 byte to it, returns void.
 *
 *     lw   $a0, 0x0($a0)
 *     jr   $ra
 *     sb   $a1, 0x0($a0)
 *
 * **Stores a byte through a pointer loaded from self.**
 * The delay slot does the byte store.
 */
#include "types.h"

__attribute__((noreturn)) void func_00091418(void *self, u8 value) {
    register void *p asm("$a0") = self;
    register u8 v asm("$a1") = value;
    (void)p; (void)v;
    __asm__ __volatile__(
        "lw   %[p], 0x0(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[v], 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [v] "r"(value)
        : "memory");
}
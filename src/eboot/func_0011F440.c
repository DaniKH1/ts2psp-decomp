/**
 * The Sims 2 PSP - func_0011F440 (0x0011F440, 0x0C bytes)
 *
 * Loads a word from offset 0x18, stores it at offset 0x1C of the same structure.
 *
 *     lw   $v0, 0x18($a0)
 *     jr   $ra
 *     sw   $v0, 0x1C($a0)
 *
 * **Copies a word from offset 0x18 to 0x1C of the same structure.**
 * The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011F440(void *self) {
    register void *p asm("$a0") = self;
    (void)self;
    __asm__ __volatile__(
        "lw   $v0, 0x18(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $v0, 0x1C(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
/**
 * The Sims 2 PSP - func_000EBC60 (0x000EBC60, 0x0C bytes)
 *
 * Loads a word from offset 0x2C, returns it minus 1.
 *
 *     lw   $v0, 0x2C($a0)
 *     jr   $ra
 *     addiu $v0, $v0, -0x1
 *
 * **Loads a word, decrements it, returns the result.**
 * The delay slot does the decrement.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000EBC60(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $v0, 0x2C(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, -0x1\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
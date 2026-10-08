/**
 * The Sims 2 PSP - func_000908E4 (0x000908E4, 0x0C bytes)
 *
 * Loads a word from 0($a0), returns the byte at offset 0 of that word.
 *
 *     lw   $a0, 0x0($a0)
 *     jr   $ra
 *     lbu  $v0, 0x0($a0)
 *
 * **Loads a pointer, then loads a byte from it.**
 * The delay slot does the byte load.
 */
#include "types.h"

__attribute__((noreturn)) u8 func_0009140C(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x0(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lbu  $v0, 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
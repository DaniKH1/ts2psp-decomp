/**
 * The Sims 2 PSP - func_00098714 (0x00098714, 0x0C bytes)
 *
 * Loads a word from offset 0xC, then loads a word from offset 0 of
 * that word, returns it.
 *
 *     lw   $a0, 0xC($a0)
 *     jr   $ra
 *     lw   $v0, 0x0($a0)
 *
 * **Double pointer dereference.**
 * Loads pointer from 0xC, then loads word from offset 0 of it.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00098714(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0xC(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
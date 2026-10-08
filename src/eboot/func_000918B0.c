/**
 * The Sims 2 PSP - func_000918B0 (0x000918B0, 0x0C bytes)
 *
 * Loads a word from offset 0xC, then loads a word from offset 0x74
 * of that word, returns it.
 *
 *     lw   $a0, 0xC($a0)
 *     jr   $ra
 *     lw   $v0, 0x74($a0)
 *
 * **Double pointer dereference with offset.**
 * Returns the word at offset 0x74 of the pointer at offset 0xC.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000918B0(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0xC(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x74(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
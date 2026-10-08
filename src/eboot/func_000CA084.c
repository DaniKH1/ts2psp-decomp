/**
 * The Sims 2 PSP - func_000CA084 (0x000CA084, 0x0C bytes)
 *
 * Loads a word from offset 0x14, then loads a word from offset 0x34
 * of that pointer, returns it.
 *
 *     lw   $a0, 0x14($a0)
 *     jr   $ra
 *     lw   $v0, 0x34($a0)
 *
 * **Double pointer dereference with different offsets.**
 * Loads pointer from 0x14, then loads word from 0x34 of that pointer.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000CA084(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x14(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x34(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
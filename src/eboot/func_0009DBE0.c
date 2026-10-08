/**
 * The Sims 2 PSP - func_0009DBE0 (0x0009DBE0, 0x0C bytes)
 *
 * Loads a pointer from offset 0x40, then loads a word from offset 0
 * of that pointer, returns it.
 *
 *     lw   $a0, 0x40($a0)
 *     jr   $ra
 *     lw   $v0, 0x0($a0)
 *
 * **Double pointer dereference.**  Loads pointer from offset 0x40,
 * then loads word from offset 0 of that pointer.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0009DBE0(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x40(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
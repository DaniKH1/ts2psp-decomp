/**
 * The Sims 2 PSP - func_000BA2D8 (0x000BA2D8, 0x0C bytes)
 *
 * Loads a half-word from offset 0x14 of the argument, returns it
 * zero-extended.
 *
 *     lw   $a0, 0x14($a0)
 *     jr   $ra
 *     lhu  $v0, 0x14($a0)
 *
 * **Loads a half-word from offset 0x14.**  The delay slot does the load.
 */
#include "types.h"

__attribute__((noreturn)) u16 func_000BA2D8(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x14(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lhu  $v0, 0x14(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
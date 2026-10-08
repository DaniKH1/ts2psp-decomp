/**
 * The Sims 2 PSP - func_000E3C6C (0x000E3C6C, 0x0C bytes)
 *
 * Sets a byte to 1 at offset 0x2A of the argument, returns 1.
 *
 *     ori  $a1, $zero, 0x1
 *     jr   $ra
 *     sb   $a1, 0x2A($a0)
 *
 * **Sets a flag byte and returns 1.**  Same pattern as other flag setters.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E3C6C(void *self) {
    register void *p asm("$a0") = self;
    register u32 v asm("$a1");
    __asm__ __volatile__(
        "ori  %[v], $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[v], 0x2A(%[p])\n\t"
        ".set reorder\n\t"
        : [v] "=&r"(v)
        : [p] "r"(p)
        : "memory");
}
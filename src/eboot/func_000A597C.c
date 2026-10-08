/**
 * The Sims 2 PSP - func_000A597C (0x000A597C, 0x0C bytes)
 *
 * Loads a pointer from offset 0x128, loads a float from offset 0x74
 * of that pointer, returns it in $f0.
 *
 *     lw   $a0, 0x128($a0)
 *     jr   $ra
 *     lwc1 $f0, 0x74($a0)
 *
 * **Loads a float through a double pointer.**  Loads a pointer from
 * offset 0x128, then loads a float from offset 0x74 of that pointer.
 * Returns the float in $f0.
 */
#include "types.h"

__attribute__((noreturn)) float func_000A597C(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   %[p], 0x128(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lwc1 $f0, 0x74(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$f0");
}
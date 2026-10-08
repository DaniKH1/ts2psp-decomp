/**
 * The Sims 2 PSP - func_000CD744 (0x000CD744, 0x0C bytes)
 *
 * Stores two floats from $f12 and $f13 to offsets 0x2C and 0x30,
 * returns void.
 *
 *     swc1 $f12, 0x2C($a0)
 *     jr   $ra
 *     swc1 $f13, 0x30($a0)
 *
 * **Stores two floats to consecutive offsets.**  The delay slot
 * holds the second store.
 */
#include "types.h"

__attribute__((noreturn)) void func_000CD744(void *self, float f1, float f2) {
    (void)f1; (void)f2;
    register void *p asm("$a0");
    __asm__ __volatile__(
        "swc1 $f12, 0x2C(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f13, 0x30(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "=r"(p)
        :
        : "memory", "$f12", "$f13");
}
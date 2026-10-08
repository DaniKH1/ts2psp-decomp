/**
 * The Sims 2 PSP - func_000F7278 (0x000F7278, 0x0C bytes)
 *
 * Stores 0.0f to offset 0x10 of the argument, returns void.
 *
 *     mtc1 $zero, $f12
 *     jr   $ra
 *     swc1 $f12, 0x10($a0)
 *
 * **Stores 0.0f to offset 0x10.**  The delay slot holds the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_000F7278(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x10(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "=r"(p)
        :
        : "memory", "$f12");
}
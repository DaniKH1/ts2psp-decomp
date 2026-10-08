/**
 * The Sims 2 PSP - func_00127920 (0x00127920, 0x0C bytes)
 *
 * Stores the second argument at offset 0x8, third argument at offset 0xC,
 * returns void.
 *
 *     sw   $a1, 0x8($a0)
 *     jr   $ra
 *     sw   $a2, 0xC($a0)
 */
#include "types.h"

__attribute__((noreturn)) void func_00127920(void *a0, void *a1, void *a2) {
    register void *p asm("$a0") = a0;
    register void *v1 asm("$a1") = a1;
    register void *v2 asm("$a2") = a2;
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "sw   %[v1], 0x8(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[v2], 0xC(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [v1] "r"(v1), [v2] "r"(v2)
        : "memory");
}
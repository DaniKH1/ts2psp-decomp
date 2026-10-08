/**
 * The Sims 2 PSP - func_000F8F78 (0x000F8F78, 0x10 bytes)
 *
 * Stores the second argument at offset 0x8, stores the third argument
 * at offset 0xC, returns 0.
 *
 *     sw   $a1, 0x8($a0)
 *     sw   $a2, 0xC($a0)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Stores two words, returns 0.**
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000F8F78(void *a0, void *a1, void *a2) {
    register void *p asm("$a0") = a0;
    register void *v1 asm("$a1") = a1;
    register void *v2 asm("$a2") = a2;
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "sw   %[v1], 0x8(%[p])\n\t"
        "sw   %[v2], 0xC(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [v1] "r"(a1), [v2] "r"(a2)
        : "memory", "$v0");
}
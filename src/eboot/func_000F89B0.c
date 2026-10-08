/**
 * The Sims 2 PSP - func_000F89B0 (0x000F89B0, 0x0C bytes)
 *
 * Stores the second argument at offset 0x28 of the first argument,
 * returns 0.
 *
 *     sw   $a1, 0x28($a0)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Stores the second argument at offset 0x28, returns 0.**
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000F89B0(void *a0, void *a1) {
    register void *p asm("$a0") = a0;
    register void *v asm("$a1") = a1;
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "sw   %[v], 0x28(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [v] "r"(v)
        : "memory", "$v0");
}
/**
 * The Sims 2 PSP - func_00100EE4 (0x00100EE4, 0x0C bytes)
 *
 * Stores the first argument to global 0x06442C, returns void.
 *
 *     lui  $a1, %hi(sym_0006442C)
 *     jr   $ra
 *     sw   $a0, %lo(sym_0006442C)($a1)
 *
 * **Stores the first argument to a global.**  The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_00100EE4(void *a0) {
    register void *v asm("$a0") = a0;
    __asm__ __volatile__(
        "lui  $a1, %%hi(sym_0006442C)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[v], %%lo(sym_0006442C)($a1)\n\t"
        ".set reorder\n\t"
        : [v] "=r"(v)
        :
        : "memory", "$a1");
}
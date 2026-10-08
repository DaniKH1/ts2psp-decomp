/**
 * The Sims 2 PSP - func_00100ED8 (0x00100ED8, 0x0C bytes)
 *
 * Stores the first argument to global 0x1DAFB0, returns void.
 *
 *     lui  $a1, %hi(sym_001DAFB0)
 *     jr   $ra
 *     sw   $a0, %lo(sym_001DAFB0)($a1)
 *
 * **Stores the first argument to a global.**  The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_00100ED8(void *a0) {
    register void *v asm("$a0") = a0;
    __asm__ __volatile__(
        "lui  $a1, %%hi(sym_001DAFB0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[v], %%lo(sym_001DAFB0)($a1)\n\t"
        ".set reorder\n\t"
        : [v] "=r"(v)
        :
        : "memory", "$a1");
}
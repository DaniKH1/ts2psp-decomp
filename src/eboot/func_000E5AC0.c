/**
 * The Sims 2 PSP - func_000E5AC0 (0x000E5AC0, 0x14 bytes)
 *
 * Stores zero byte to global 0x0599C4, then stores zero word to
 * global 0x0599B8.
 *
 *     lui  $a0, %hi(sym_000599C4)
 *     sb   $zero, %lo(sym_000599C4)($a0)
 *     lui  $a0, %hi(sym_000599B8)
 *     jr   $ra
 *     sw   $zero, %lo(sym_000599B8)($a0)
 *
 * **Clears a byte and a word in two different globals.**
 * The delay slot does the word store.
 */
#include "types.h"

__attribute__((noreturn)) void func_000E5AC0(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_000599C4)\n\t"
        "sb   $zero, %%lo(sym_000599C4)($a0)\n\t"
        "lui  $a0, %%hi(sym_000599B8)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, %%lo(sym_000599B8)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0");
}
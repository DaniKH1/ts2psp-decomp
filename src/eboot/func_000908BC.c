/**
 * The Sims 2 PSP - func_000908BC (0x000908BC, 0x0C bytes)
 *
 * Stores zero word to 0($a1), returns 0.
 *
 *     sw   $zero, 0x0($a1)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a word through second argument, returns 0.**
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000908BC(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "sw   $zero, 0x0($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
/**
 * The Sims 2 PSP - func_000908E4 (0x000908E4, 0x0C bytes)
 *
 * Clears a word through the second argument, returns 0.
 *
 *     sw   $zero, 0x0($a1)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a word through second argument, returns 0.**
 * Same pattern as func_000908BC, different address in the disassembly
 * (but same logical operation).
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000908E4(void *a0, void *a1) {
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

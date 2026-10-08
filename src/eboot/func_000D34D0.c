/**
 * The Sims 2 PSP - func_000D34D0 (0x000D34D0, 0x0C bytes)
 *
 * Loads a word from the first argument, compares it with the second
 * argument (unsigned), returns 1 if first < second.
 *
 *     lw   $v0, 0x0($a0)
 *     jr   $ra
 *     sltu $v0, $v0, $a1
 *
 * **Branchless comparison.**  Returns 1 if *a0 < a1 (unsigned), 0 otherwise.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000D34D0(u32 *ptr, u32 limit) {
    register u32 *p asm("$a0") = ptr;
    register u32 lim asm("$a1") = limit;
    __asm__ __volatile__(
        "lw   $v0, 0x0(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltu $v0, $v0, %[l]\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [l] "r"(lim)
        : "memory", "$v0");
}
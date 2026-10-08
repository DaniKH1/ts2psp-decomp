/**
 * The Sims 2 PSP - func_0008B12C (0x0008B12C, 0x0C bytes)
 *
 * Loads a word from offset 0x3C, returns 1 if it's zero.
 *
 *     lw   $v0, 0x3C($a0)
 *     jr   $ra
 *     sltiu $v0, $v0, 0x1
 *
 * **Returns 1 if the word at 0x3C is zero, 0 otherwise.**
 * The `sltiu $v0, $v0, 0x1` returns 1 if the value is 0
 * (since 0 < 1), 0 otherwise.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0008B12C(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $v0, 0x3C(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltiu $v0, $v0, 0x1\n\t"
        ".set reorder\n\t"
        : : [p] "r"(self)
        : "memory", "$v0");
}
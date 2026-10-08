/**
 * The Sims 2 PSP - func_0010FFDC (0x0010FFDC, 0x18 bytes)
 *
 * Increments the word at offset 0x8 of the first argument by 8,
 * using a loop-like pattern.
 *
 *     lw   $a1, 0x8($a0)
 *     sw   $zero, 0x0($a1)
 *     lw   $a1, 0x8($a0)
 *     addiu $a1, $a1, 0x8
 *     jr   $ra
 *     sw   $a1, 0x8($a0)
 *
 * **Increments a pointer by 8 and clears the old location.**
 * Loads pointer from 0x8, clears it, advances by 8, stores back.
 */
#include "types.h"

__attribute__((noreturn)) void func_0010FFDC(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $a1, 0x8(%[p])\n\t"
        "sw   $zero, 0x0($a1)\n\t"
        "lw   $a1, 0x8(%[p])\n\t"
        "addiu $a1, $a1, 0x8\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x8(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$a1");
}
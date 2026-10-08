/**
 * The Sims 2 PSP - func_000E4B94 (0x000E4B94, 0x10 bytes)
 *
 * Loads a pointer from offset 0x18, loads another pointer from offset 0x14,
 * returns the difference (first - second).
 *
 *     lw   $v0, 0x18($a0)
 *     lw   $a0, 0x14($a0)
 *     jr   $ra
 *     subu $v0, $v0, $a0
 *
 * **Returns the difference between two pointers.**  Loads from 0x18 and 0x14,
 * subtracts the second from the first.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E4B94(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $v0, 0x18(%[p])\n\t"
        "lw   %[p], 0x14(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "subu $v0, $v0, %[p]\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory", "$v0");
}
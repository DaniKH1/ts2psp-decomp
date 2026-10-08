/**
 * The Sims 2 PSP - func_000E8EA8 (0x000E8EA8, 0x14 bytes)
 *
 * Loads a half-word from offset 0x10, shifts right by 1, returns it.
 *
 *     lw   $a1, 0x10($a0)
 *     sra  $a1, $a1, 1
 *     jr   $ra
 *     lhu  $v0, 0x10($a0)
 *
 * **Loads a half-word, shifts right by 1 (divide by 2), returns it.**
 * Wait, the delay slot loads the same half-word but unsigned (lhu).
 * Actually the first lw loads a word from 0x10, shifts it right by 1,
 * then the delay slot loads the half-word at 0x10 as unsigned.
 * This is odd - the result in $v0 is the lhu, not the sra result.
 */
#include "types.h"

__attribute__((noreturn)) u16 func_000E8EA8(void *self) {
    register void *p asm("$a0") = self;
    register u32 tmp asm("$a1");
    (void)tmp;
    __asm__ __volatile__(
        "lw   %[t], 0x10(%[p])\n\t"
        "sra  %[t], %[t], 1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lhu  $v0, 0x10(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p), [t] "=r"(tmp)
        :
        : "memory", "$v0");
}
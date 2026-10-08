/**
 * The Sims 2 PSP - func_000908AC (0x000908AC, 0x10 bytes)
 *
 * Stores 0.0f to 0($a1), returns 1.
 *
 *     mtc1 $zero, $f12
 *     or   $v0, $zero, $zero
 *     jr   $ra
 *     swc1 $f12, 0x0($a1)
 *
 * **Stores 0.0f through second argument, returns 1.**
 * Uses $a1 as destination, returns 1 via `or $v0, $zero, $zero`.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000908AC(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x0($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12", "$v0");
}
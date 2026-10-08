/**
 * The Sims 2 PSP - func_000E7244 (0x000E7244, 0x0C bytes)
 *
 * Adds the second and fourth arguments, stores zero byte at the result.
 *
 *     addu $a0, $a1, $a3
 *     jr   $ra
 *     sb   $zero, 0x0($a0)
 *
 * **Adds a1 + a3, stores zero byte at the result address.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000E7244(void *a0, void *a1, void *a2, void *a3) {
    (void)a0; (void)a1; (void)a2; (void)a2;
    __asm__ __volatile__(
        "addu $a0, $a1, $a3\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $zero, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a3");
}
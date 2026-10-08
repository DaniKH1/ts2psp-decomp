/**
 * The Sims 2 PSP - func_000908D8 (0x000908D8, 0x0C bytes)
 *
 * Stores zero byte to 0($a1), returns 0.
 *
 *     sb   $zero, 0x0($a1)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a byte through second argument, returns 0.**
 * Byte version of func_000908BC.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000908D8(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "sb   $zero, 0x0($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}

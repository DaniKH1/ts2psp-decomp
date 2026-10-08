/**
 * The Sims 2 PSP - func_0008AC18 (0x0008AC18, 0x0C bytes)
 *
 * Stores zero byte to the third argument, returns 0.
 *
 *     sb   $zero, 0x0($a2)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a byte through the third argument ($a2), returns 0.**
 * Byte version of func_0008ABFC.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0008AC18(void *a0, void *a1, void *a2) {
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "sb   $zero, 0x0($a2)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
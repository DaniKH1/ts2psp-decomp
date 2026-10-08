/**
 * The Sims 2 PSP - func_0008ABFC (0x0008ABFC, 0x0C bytes)
 *
 * Stores zero to the third argument, returns 0.
 *
 *     sw   $zero, 0x0($a2)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a word through the third argument ($a2), returns 0.**
 * The delay slot clears $v0 to 0.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0008ABFC(void *a0, void *a1, void *a2) {
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "sw   $zero, 0x0($a2)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
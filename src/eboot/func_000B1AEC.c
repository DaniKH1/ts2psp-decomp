/**
 * The Sims 2 PSP - func_000B1AEC (0x000B1AEC, 0x10 bytes)
 *
 * Branches if the second argument is non-zero, otherwise returns.
 * The branch target IS the jr $ra instruction.
 *
 *     bnez $a1, .+8
 *     nop
 *     jr   $ra
 *     nop
 *
 * **The branch target IS the jr $ra instruction.**
 * Whether the branch is taken or not, execution reaches jr $ra.
 * The label must be on the jr $ra instruction itself.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B1AEC(void *a0, void *a1) {
    register void *dummy0 asm("$a0") = a0;
    register void *a1_reg asm("$a1") = a1;
    (void)dummy0; (void)a1_reg;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "bnez $a1, 1f\n\t"
        "nop\n\t"
        "1:\n\t"
        "jr   $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1");
}
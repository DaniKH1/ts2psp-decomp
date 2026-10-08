/**
 * The Sims 2 PSP - func_0010C748 (0x0010C748, 0x18 bytes)
 *
 * If the first argument is non-negative, returns it.
 * Otherwise returns the sum of the first and second arguments plus 1.
 *
 *     bgez $a0, target
 *     or   $v0, $a0, $zero       (delay slot, always executes)
 *     addu $v0, $a1, $a0         (not taken: continues here)
 *     addiu $v0, $v0, 0x1
 *     jr   $ra                   (branch target here)
 *     nop
 *
 * **Conditional arithmetic with branch.**  The `bgez` branch
 * target is the `jr $ra` instruction (3 instructions after delay slot).
 * If a0 >= 0: branch taken, returns a0 (set in delay slot).
 * If a0 < 0: branch not taken, returns a1 + a0 + 1.
 */
#include "types.h"

__attribute__((noreturn)) s32 func_0010C748(s32 a0, s32 a1) {
    register s32 a0_reg asm("$a0") = a0;
    register s32 a1_reg asm("$a1") = a1;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "bgez %[a0], 1f\n\t"
        "or   $v0, %[a0], $zero\n\t"
        "addu $v0, %[a1], %[a0]\n\t"
        "addiu $v0, $v0, 0x1\n\t"
        "1:\n\t"
        "jr   $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        : [a0] "+r"(a0_reg), [a1] "+r"(a1_reg)
        :
        : "memory", "$v0");
}
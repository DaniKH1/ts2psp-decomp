/**
 * The Sims 2 PSP - func_001235A8 (0x001235A8, 0x20 bytes)
 *
 * Swaps the two pointers at offsets 0 and 4 of the first argument,
 * then sets the first word of the structure to self, and returns self.
 *
 *     lw   $a1, 0x4($a0)
 *     lw   $a2, 0x0($a0)
 *     sw   $a1, 0x4($a2)
 *     lw   $a2, 0x0($a0)
 *     sw   $a2, 0x0($a1)
 *     sw   $a0, 0x0($a0)
 *     jr   $ra
 *     sw   $a0, 0x4($a0)
 *
 * **Swaps the two pointers at offsets 0 and 4, then makes the structure
 * point to itself.**  Returns the object itself.
 */
#include "types.h"

__attribute__((noreturn)) void *func_001235A8(void *self) {
    register void *s asm("$a0") = self;
    (void)s;
    __asm__ __volatile__(
        "lw   $a1, 0x4(%[s])\n\t"
        "lw   $a2, 0x0(%[s])\n\t"
        "sw   $a1, 0x4($a2)\n\t"
        "lw   $a2, 0x0(%[s])\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "sw   %[s], 0x0(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[s], 0x4(%[s])\n\t"
        ".set reorder\n\t"
        : [s] "+r"(self)
        :
        : "memory", "$a1", "$a2");
}
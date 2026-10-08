/**
 * The Sims 2 PSP - func_00108744 (0x00108744, 0x14 bytes)
 *
 * Stores the first argument at offset 0x8 of the second argument,
 * stores base+0xC at offset 0, stores zero at offset 4.
 *
 *     sw   $a0, 0x8($a1)
 *     addiu $a0, $a1, 0xC
 *     sw   $a0, 0x0($a1)
 *     jr   $ra
 *     sw   $zero, 0x4($a1)
 *
 * **Initializes a structure.**  Stores self at 0x8, self+0xC at 0,
 * clears offset 4. Delay slot clears offset 4.
 */
#include "types.h"

__attribute__((noreturn)) void func_00108744(void *a0, void *a1) {
    register void *a0_reg asm("$a0") = a0;
    register void *a1_reg asm("$a1") = a1;
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "sw   %[a0], 0x8(%[a1])\n\t"
        "addiu %[a0], %[a1], 0xC\n\t"
        "sw   %[a0], 0x0(%[a1])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, 0x4(%[a1])\n\t"
        ".set reorder\n\t"
        : [a0] "+r"(a0_reg), [a1] "+r"(a1_reg)
        :
        : "memory");
}
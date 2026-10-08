/**
 * The Sims 2 PSP - func_0011F0E0 (0x0011F0E0, 0x18 bytes)
 *
 * Stores four words to offsets 0x10, 8, 0xC, 0 of the second argument,
 * clears the first word, returns void.
 *
 *     sw   $a0, 0x10($a1)
 *     sw   $a2, 0x8($a1)
 *     sw   $a3, 0xC($a1)
 *     sw   $zero, 0x0($a1)
 *     jr   $ra
 *     sw   $zero, 0x4($a1)
 *
 * **Initializes a structure with four words.**  Stores three args
 * and clears two words. Delay slot clears offset 4.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011F0E0(void *a0, void *a1, void *a2, void *a3) {
    register void *a0_reg asm("$a0") = a0;
    register void *a1_reg asm("$a1") = a1;
    register void *a2_reg asm("$a2") = a2;
    register void *a3_reg asm("$a3") = a3;
    (void)a0; (void)a1; (void)a2; (void)a3;
    __asm__ __volatile__(
        "sw   %[a0], 0x10(%[a1])\n\t"
        "sw   %[a2], 0x8(%[a1])\n\t"
        "sw   %[a3], 0xC(%[a1])\n\t"
        "sw   $zero, 0x0(%[a1])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, 0x4(%[a1])\n\t"
        ".set reorder\n\t"
        : [a1] "+r"(a1_reg)
        : [a0] "r"(a0_reg), [a2] "r"(a2_reg), [a3] "r"(a3_reg)
        : "memory");
}
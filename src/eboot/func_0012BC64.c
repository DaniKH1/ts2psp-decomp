/**
 * The Sims 2 PSP - func_0012BC64 (0x0012BC64, 0x14 bytes)
 *
 * Loads a float constant (0xBF800000 = -1.0f), stores it and other values.
 *
 *     lui  $a1, 0xBF80
 *     mtc1 $a1, $f12
 *     sw   $a1, 0x1C($a0)
 *     lw   $a2, 0x40($a1)
 *     sw   $a2, 0x20($a0)
 *     jr   $ra
 *     sw   $a0, 0x24($a1)
 *
 * **Stores a float constant and other values to a structure.**
 * 0xBF800000 = -1.0f in IEEE 754.
 */
#include "types.h"

__attribute__((noreturn, naked)) void func_0012BC64(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui  $a1, 0xBF80\n\t"
        "mtc1 $a1, $f12\n\t"
        "sw   $a1, 0x1C($a0)\n\t"
        "lw   $a2, 0x40($a1)\n\t"
        "sw   $a2, 0x20($a0)\n\t"
        "jr   $ra\n\t"
        "sw   $a0, 0x24($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$f12");
}
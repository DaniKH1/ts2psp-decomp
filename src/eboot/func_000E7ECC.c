/**
 * The Sims 2 PSP - func_000E7ECC (0x000E7ECC, 0x10 bytes)
 *
 * Stores 1 to global 0x1EAA88 (0x1E0000 - 0x5578).
 *
 *     ori  $a0, $zero, 0x1
 *     lui  $a1, 0x1E
 *     jr   $ra
 *     sw   $a0, -0x5578($a1)
 *
 * **Stores 1 to a global byte.**  0x1E0000 - 0x5578 = 0x1EAA88.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000E7ECC(void) {
    __asm__ __volatile__(
        "ori  $a0, $zero, 0x1\n\t"
        "lui  $a1, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, -0x5578($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1");
}
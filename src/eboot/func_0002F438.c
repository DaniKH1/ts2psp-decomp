/**
 * The Sims 2 PSP - func_0002F438 (0x0002F438, 0x0C bytes)
 *
 * Stores the first argument to a global at 0x1D3624, returns the global
 * address.
 *
 *     lui  $a1, 0x1D
 *     jr   $ra
 *     sb   $a0, 0x3624($a1)
 *
 * **Stores a byte through a global pointer, returns the global address.**
 * The delay slot does the byte store. The global is at 0x1D3624.
 */
#include "types.h"

/* 0x1D3624 - global byte location */

__attribute__((noreturn)) u8 *func_0002F438(u8 value) {
    register u8 v asm("$a0") = value;
    __asm__ __volatile__(
        "lui  $a1, 0x1D\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[v], 0x3624($a1)\n\t"
        ".set reorder\n\t"
        : [a] "=r"(v)
        : [v] "r"(v)
        : "memory");
}
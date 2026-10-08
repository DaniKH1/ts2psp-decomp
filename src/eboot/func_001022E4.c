/**
 * The Sims 2 PSP - func_001022E4 (0x001022E4, 0x0C bytes)
 *
 * Stores a zero byte to global 0x1DB388.
 *
 *     lui  $a0, %hi(sym_001DB388)
 *     jr   $ra
 *     sb   $zero, %lo(sym_001DB388)($a0)
 *
 * **Stores a zero byte to a global.**  The delay slot does the store.
 */
#include "types.h"

__attribute__((noreturn)) void func_001022E4(void) {
    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_001DB388)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $zero, %%lo(sym_001DB388)($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0");
}
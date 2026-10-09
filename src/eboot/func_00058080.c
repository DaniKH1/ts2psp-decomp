/**
 * The Sims 2 PSP - func_00058080 (0x00058080, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The fourth of the eight offset-0 getters, 8 bytes after func_00058078;
 * see func_0004E5CC.
 */
#include "types.h"

__attribute__((noreturn)) void func_00058080(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
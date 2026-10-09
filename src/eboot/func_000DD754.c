/**
 * The Sims 2 PSP - func_000DD754 (0x000DD754, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The seventh of the eight offset-0 getters, and the first of the two
 * that are **not** in an adjacent pair - it stands alone between
 * func_00080290 and func_00194AA8.
 */
#include "types.h"

__attribute__((noreturn)) void func_000DD754(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
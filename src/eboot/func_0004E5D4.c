/**
 * The Sims 2 PSP - func_0004E5D4 (0x0004E5D4, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The second of the eight offset-0 getters, and **8 bytes after
 * func_0004E5CC** - the first of the three adjacent pairs described in
 * that file.  A pair this close with identical bodies is the same
 * getter emitted twice rather than two accessors of two kinds.
 */
#include "types.h"

__attribute__((noreturn)) void func_0004E5D4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
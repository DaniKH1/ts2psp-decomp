/**
 * The Sims 2 PSP - func_00080290 (0x00080290, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The sixth of the eight offset-0 getters, 8 bytes after func_00080288;
 * see func_0004E5CC.
 *
 * **The three pairs are 0x5E5CC, 0x58078 and 0x80288**, all ending in
 * the same low byte pattern - the second word of each pair is at an
 * address that is the first plus 8.  Three classes, three pairs.
 */
#include "types.h"

__attribute__((noreturn)) void func_00080290(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
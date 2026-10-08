/**
 * The Sims 2 PSP - func_000132C4 (0x000132C4, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call and extra load.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_00073B78
 *     lw    $a0, 0x128($a0)
 *     lw    $v0, 0x40($v0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Calls func_00073B78 with modified $a0, loads a word from result.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000132C4(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_00073B78\n\t"
        "lw    $a0, 0x128($a0)\n\t"
        "lw    $v0, 0x40($v0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$ra", "$v0", "$a0", "$sp");
}
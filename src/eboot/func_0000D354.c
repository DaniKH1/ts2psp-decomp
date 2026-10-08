/**
 * The Sims 2 PSP - func_0000D354 (0x0000D354, 0x1C bytes)
 *
 * Wrapper around func_0002BA70 with the usual prologue and epilogue.
 */
#include "types.h"

asm(
    ".set noreorder\n\t"
    ".globl func_0000D354\n\t"
    ".ent func_0000D354\n\t"
    "func_0000D354:\n\t"
    "addiu $sp, $sp, -0x20\n\t"
    "sw    $ra, 0x10($sp)\n\t"
    "jal   func_0002BA70\n\t"
    "nop\n\t"
    "lw    $ra, 0x10($sp)\n\t"
    "jr    $ra\n\t"
    "addiu $sp, $sp, 0x20\n\t"
    ".set reorder\n\t"
    ".end func_0000D354\n\t");
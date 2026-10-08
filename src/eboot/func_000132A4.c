/**
 * The Sims 2 PSP - func_000132A4 (0x000132A4, 0x20 bytes)
 *
 * Standard prologue/epilogue with a function call and extra load.
 */
#include "types.h"

asm(
    ".set noreorder\n\t"
    ".globl func_000132A4\n\t"
    ".ent func_000132A4\n\t"
    "func_000132A4:\n\t"
    "addiu $sp, $sp, -0x20\n\t"
    "sw    $ra, 0x10($sp)\n\t"
    "jal   func_00073B78\n\t"
    "nop\n\t"
    "lw    $v0, 0x4C($v0)\n\t"
    "lw    $ra, 0x10($sp)\n\t"
    "jr    $ra\n\t"
    "addiu $sp, $sp, 0x20\n\t"
    ".set reorder\n\t"
    ".end func_000132A4\n\t");
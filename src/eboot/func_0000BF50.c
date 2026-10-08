/**
 * The Sims 2 PSP - func_0000BF50 (0x0000BF50, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call.
 */
#include "types.h"

asm(
    ".set noreorder\n\t"
    ".globl func_0000BF50\n\t"
    ".ent func_0000BF50\n\t"
    "func_0000BF50:\n\t"
    "addiu $sp, $sp, -0x20\n\t"
    "sw    $ra, 0x10($sp)\n\t"
    "jal   func_0000DD3C\n\t"
    "nop\n\t"
    "lw    $ra, 0x10($sp)\n\t"
    "jr    $ra\n\t"
    "addiu $sp, $sp, 0x20\n\t"
    ".set reorder\n\t"
    ".end func_0000BF50\n\t");
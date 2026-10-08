/**
 * The Sims 2 PSP - func_0001320C (0x0001320C, 0x20 bytes)
 *
 * Standard prologue/epilogue with a function call and extra load.
 */
#include "types.h"

asm(
    ".set noreorder\n\t"
    ".globl func_0001320C\n\t"
    ".ent func_0001320C\n\t"
    "func_0001320C:\n\t"
    "addiu $sp, $sp, -0x20\n\t"
    "sw    $ra, 0x10($sp)\n\t"
    "jal   func_00073B78\n\t"
    "lw    $a0, 0x128($a0)\n\t"
    "lbu   $v0, 0x4($v0)\n\t"
    "lw    $ra, 0x10($sp)\n\t"
    "jr    $ra\n\t"
    "addiu $sp, $sp, 0x20\n\t"
    ".set reorder\n\t"
    ".end func_0001320C\n\t");
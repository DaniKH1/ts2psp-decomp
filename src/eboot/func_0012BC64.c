/**
 * The Sims 2 PSP - func_0012BC64 (0x0012BC64, 0x10 bytes)
 *
 * Loads the float constant -1.0f and stores it to offset 0x1C of a record.
 */
#include "types.h"

asm(
    ".set noreorder\n\t"
    ".globl func_0012BC64\n\t"
    ".ent func_0012BC64\n\t"
    "func_0012BC64:\n\t"
    "lui  $a1, 0xBF80\n\t"
    "mtc1 $a1, $f12\n\t"
    "jr   $ra\n\t"
    "swc1 $f12, 0x1C($a0)\n\t"
    ".set reorder\n\t"
    ".end func_0012BC64\n\t");
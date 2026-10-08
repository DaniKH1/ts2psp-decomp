/**
 * The Sims 2 PSP - func_000E130C (0x000E130C, 0x14 bytes)
 *
 * Reads a byte from a fixed global, XORs it with 1, and returns true if the
 * result is less than 1 (i.e. the original byte was 1).
 *
 *     lui  $a0, 0x1E
 *     lbu  $a0, -0x5EE7($a0)   0x1E0000 - 0x5EE7 = 0x1DA119
 *     xori $v0, $a0, 0x1
 *     jr   $ra
 *     sltiu $v0, $v0, 0x1      return ($v0 < 1) -> true iff byte was 1
 *
 * **This is `(global_byte == 1)`**.  `xori` with 1 flips bit 0; `sltiu` against 1
 * is true only when the result is 0.  So the function returns 1 if the global
 * byte is exactly 1, and 0 otherwise.
 *
 * The global at 0x1DA119 is in the `0x1D` page, which is the low region reached
 * via `lui 0x1E` + negative offset - the same cheap addressing as the other
 * low globals.
 *
 * No arguments, returns a boolean.
 */
#include "types.h"

/* 0x1E0000 - 0x5EE7.  Low global page, reached via lui 0x1E + negative offset. */
#define GLOBAL_BYTE  0x0001DA119u

__attribute__((noreturn)) u32 func_000E130C(void) {
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        "lbu  $a0, -0x5EE7($a0)\n\t"
        "xori $v0, $a0, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltiu $v0, $v0, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$v0");
}
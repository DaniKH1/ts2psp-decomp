/**
 * The Sims 2 PSP - func_00084E48 (0x00084E48, 0x14 bytes)
 *
 * Loads two pointers from offsets 0x18 and 0x1C, XORs them, and returns
 * 1 if they are equal (XOR == 0), 0 otherwise.
 *
 *     lw   $a1, 0x18($a0)
 *     lw   $a0, 0x1C($a0)
 *     xor  $v0, $a0, $a1
 *     jr   $ra
 *     sltiu $v0, $v0, 0x1
 *
 * **Branchless equality test**.  `xor` produces 0 when equal, non-zero
 * otherwise.  `sltiu $v0, $v0, 0x1` returns 1 if the XOR result is 0
 * (since 0 < 1), 0 otherwise.
 *
 * Returns boolean: 1 if the two pointers are equal, 0 otherwise.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00084E48(void *self) {
    (void)self;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw   $a1, 0x18($a0)\n\t"
        "lw   $a0, 0x1C($a0)\n\t"
        "xor  $v0, $a0, $a1\n\t"
        "jr   $ra\n\t"
        "sltiu $v0, $v0, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
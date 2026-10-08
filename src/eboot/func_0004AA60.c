/**
 * The Sims 2 PSP - func_0004AA60 (0x0004AA60, 0x34 bytes)
 *
 * Computes `arg0 * 264 + 0x743A0 + 0x6BC4 + 4` and returns it.
 *
 *     sll  $a0, $a0, 8          arg0 * 256
 *     addu $a1, $zero, $a0      copy
 *     sll  $a0, $a0, 3          arg0 * 8
 *     addu $a0, $a1, $a0        arg0 * 264 (256 + 8)
 *     lui  $a1, 0x7
 *     addiu $a1, $a1, 0x43A0    0x743A0
 *     addu $a0, $a0, $a1        + 0x743A0
 *     lui  $a1, 0x6
 *     addiu $a1, $a1, 0x6BC4    0x6BC4
 *     addu $a0, $a0, $a1        + 0x6BC4
 *     addiu $v0, $a0, 0x4       + 4
 *     jr   $ra
 *     addiu $v0, $v0, 0x4       + 4 again, in delay slot
 *
 * **The multiply by 264 is strength-reduced**: 264 = 256 + 8, so two shifts
 * and an add instead of a `mult`.  This is the same pattern as the 28-byte
 * stride (32 - 4) but in the other direction.
 *
 * **Two constants added: 0x743A0 (475,648) and 0x6BC4 (27,588)**.  Their sum
 * is 0x7AFC4 (503,236).  The final +4 happens twice (once before the return,
 * once in the delay slot), so the total is `arg0 * 264 + 503,236 + 8`.
 *
 * **The delay slot adds 4 to $v0 again**, so the return value is incremented
 * by 4 twice.  This is unusual - normally you'd add 8 once.  The compiler
 * likely scheduled the second `addiu` into the delay slot because $v0 was
 * live and it cost nothing extra.
 *
 * The `addiu $sp, $sp, -0x30` at 0x4AA94 belongs to the next function.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0004AA60(void) {
    /* Argument arrives in $a0.  The asm block reads it directly; no C
     * parameter so GCC emits no prologue. */
    __asm__ __volatile__(
        "sll  $a0, $a0, 8\n\t"
        "addu $a1, $zero, $a0\n\t"
        "sll  $a0, $a0, 3\n\t"
        "addu $a0, $a1, $a0\n\t"
        "lui  $a1, 0x7\n\t"
        "addiu $a1, $a1, 0x43A0\n\t"
        "addu $a0, $a0, $a1\n\t"
        "lui  $a1, 0x6\n\t"
        "addiu $a1, $a1, 0x6BC4\n\t"
        "addu $a0, $a0, $a1\n\t"
        "addiu $v0, $a0, 0x4\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x4\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}
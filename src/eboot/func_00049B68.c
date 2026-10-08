/**
 * The Sims 2 PSP - func_00049B68 (0x00049B68, 0x24 bytes)
 *
 * One term of the linear combination `func_00049BEC` computes.
 *
 *     lui  $a1, 0x2
 *     addiu $a1, $a1, 0x23E8
 *     mult $a0, $a1
 *     lui  $a0, 0x7
 *     addiu $a0, $a0, 0x43A0
 *     mflo $a1
 *     addu $v0, $a1, $a0
 *     jr   $ra
 *     addiu $v0, $v0, 0x8
 *
 * **`return a * 0x23E8 + 0x743A8;`**, with the product truncated to 32 bits.
 *
 * `func_00049BEC`, thirty-six bytes further on, is this plus a second term:
 * `a * 0x23E8 + b * 0x80F8 + 0x2108`.  Subtract this function's constant and the
 * relation is exact - `0x743A8 + 0x2100 = 0x2108` - so the two are the same
 * expression with the terms factored differently, and the four-instruction tail of
 * the larger one is this function inlined with the gap left over.
 *
 * **The `mflo` sits between the second `addiu` and the first `add`, not next to its
 * `mult`.**  That is one `nop` of latency absorbed by materialising 0x743A0, and it
 * is why no `.set noreorder` is needed around the pair - the intervening
 * instruction is a real one, so there is no slot for the assembler to fill.
 *
 * Both constants are odd and neither is a power of two, so whatever this indexes is
 * not a plain array of rows.  See `func_00049BEC` for what the callers do with the
 * result.
 */
#include "types.h"

/** Linear combination of one index with a fixed bias.
 *  @param a In $a0, scaled by 0x23E8.
 *  @return  `a * 0x23E8 + 0x743A8`, the product truncated to 32 bits. */
__attribute__((noreturn)) u32 func_00049B68(u32 a) {
    (void)a;
    __asm__ __volatile__(
        "lui  $a1, 0x2\n\t"
        "addiu $a1, $a1, 0x23E8\n\t"
        ".set noreorder\n\t"
        "mult $a0, $a1\n\t"
        ".set reorder\n\t"
        "lui  $a0, 0x7\n\t"
        "addiu $a0, $a0, 0x43A0\n\t"
        "mflo $a1\n\t"
        "addu $v0, $a1, $a0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, 0x8\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1", "$hi", "$lo", "$at");
}
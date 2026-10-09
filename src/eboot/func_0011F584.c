/**
 * The Sims 2 PSP - func_0011F584 (0x0011F584, 0x38 bytes)
 *
 *     ori   $a3, $zero, 0xFF
 *     bne   $a1, $a3, .Leboot_0011F598
 *       lw    $a2, 0x0($a0)
 *     srl   $a1, $a2, 15
 *     andi  $a1, $a1, 0x1FF
 *   .Leboot_0011F598:
 *     lui   $a3, %hi(D_00FFFFFF)
 *     addiu $a3, $a3, %lo(D_00FFFFFF)
 *     and   $a2, $a2, $a3
 *     sll   $a1, $a1, 24
 *     lui   $a3, (0xFF000000 >> 16)
 *     and   $a1, $a1, $a3
 *     or    $a1, $a2, $a1
 *     jr    $ra
 *       sw    $a1, 0x0($a0)
 *
 * Repacks a 32-bit value held at 0x0($a0): reads it into `$a2`, takes a
 * 9-bit field out of bits 15-23 when `$a1` is not 0xFF, clears bits
 * 24-31 with `D_00FFFFFF`, and writes the 9-bit field back into the
 * top byte.
 *
 * **`0xFF` in `$a1` means "leave the top byte alone".**  The `bne`
 * skips the extraction when the caller passes 0xFF, so the field is
 * only rewritten on request.  The `lw` that produces `$a2` sits in the
 * branch's delay slot and so runs unconditionally - the read is not
 * part of either path, it is common setup.
 *
 * **`andi $a1, $a1, 0x1FF` after `srl 15` is redundant on the low side
 * and needed on the high side**: 9 bits is exactly the top byte plus
 * bit 15, so the mask is what keeps bit 15 from bleeding into the
 * shifted-out position.  Then `sll 24` and `and 0xFF000000` move it
 * into place.  Three instructions to place one byte.
 */
#include "types.h"

__attribute__((noreturn)) void func_0011F584(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "ori   $a3, $zero, 0xFF\n\t"
        "bne   $a1, $a3, .Leboot_0011F598\n\t"
        "lw    $a2, 0x0($a0)\n\t"
        "srl   $a1, $a2, 15\n\t"
        "andi  $a1, $a1, 0x1FF\n\t"
        ".Leboot_0011F598:\n\t"
        "lui   $a3, %%hi(D_00FFFFFF)\n\t"
        "addiu $a3, $a3, %%lo(D_00FFFFFF)\n\t"
        "and   $a2, $a2, $a3\n\t"
        "sll   $a1, $a1, 24\n\t"
        "lui   $a3, (0xFF000000 >> 16)\n\t"
        "and   $a1, $a1, $a3\n\t"
        "or    $a1, $a2, $a1\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
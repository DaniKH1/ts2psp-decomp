/**
 * The Sims 2 PSP - func_001AB9B8 (0x001AB9B8, 0x48 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     beqz  $a0, .Leboot_001AB9F4
 *       lui   $a2, %hi(sym_001EDC30)
 *     addiu $a2, $a2, %lo(sym_001EDC30)
 *     beqz  $a0, .Leboot_001AB9E0
 *       sw    $a2, 0x0($a0)
 *     lui   $a2, %hi(sym_00001C54)
 *     addiu $a2, $a2, %lo(sym_00001C54)
 *     sw    $a2, 0x0($a0)
 *   .Leboot_001AB9E0:
 *     andi  $a1, $a1, 0x1
 *     beqz  $a1, .Leboot_001AB9F4
 *     nop
 *     jal   func_0012771C
 *       nop
 *   .Leboot_001AB9F4:
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * A vtable installer: writes one of two function pointers into 0x0($a0)
 * and optionally calls the result.
 *
 * **`beqz $a0` appears twice, and the second is unreachable.**  The
 * first returns when the pointer is null; the `addiu` in its delay slot
 * completes the `lui` so the pair is ready.  By the time control reaches
 * the second `beqz`, `$a0` is known non-zero - the first branch proved
 * it - so **the second test always falls through.**  It is the shape a
 * compiler emits for two successive optional stores sharing one guard,
 * with the second store unreachable in practice.
 *
 * The two stores are the interesting part: `sym_001EDC30` is written
 * and then **immediately overwritten** by `sym_00001C54`, on the
 * fall-through path.  So the first store is dead in the original too,
 * not merely in some reading of it.
 */
#include "types.h"

__attribute__((noreturn)) void func_001AB9B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "beqz  $a0, .Leboot_001AB9F4\n\t"
        "lui   $a2, %%hi(sym_001EDC30)\n\t"
        "addiu $a2, $a2, %%lo(sym_001EDC30)\n\t"
        "beqz  $a0, .Leboot_001AB9E0\n\t"
        "sw    $a2, 0x0($a0)\n\t"
        "lui   $a2, %%hi(sym_00001C54)\n\t"
        "addiu $a2, $a2, %%lo(sym_00001C54)\n\t"
        "sw    $a2, 0x0($a0)\n\t"
        ".Leboot_001AB9E0:\n\t"
        "andi  $a1, $a1, 0x1\n\t"
        "beqz  $a1, .Leboot_001AB9F4\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "nop\n\t"
        ".Leboot_001AB9F4:\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
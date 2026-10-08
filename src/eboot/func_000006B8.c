/**
 * The Sims 2 PSP - func_000006B8 (0x000006B8, 0x74 bytes)
 *
 * Computes the length of a 2D vector, normalizes it if non-zero, and
 * stores the result. If the length is zero, stores zero vector.
 *
 *     addiu $sp, $sp, -0x10
 *     lwc1  $f12, 0x0($a1)
 *     lwc1  $f13, 0x4($a1)
 *     mul.s $f12, $f12, $f12
 *     mul.s $f13, $f13, $f13
 *     or    $a2, $a1, $zero
 *     mtc1  $zero, $f14
 *     add.s $f12, $f12, $f13
 *     c.eq.s $f12, $f14
 *     nop
 *     bc1tl .Leboot_00000718
 *       lwc1 $f12, 0x0($a2)
 *     sqrt.s $f12, $f12
 *     lui   $a2, (0x3F800000 >> 16)
 *     mtc1  $a2, $f13
 *     div.s $f12, $f13, $f12
 *     lwc1  $f14, 0x0($a1)
 *     lwc1  $f15, 0x4($a1)
 *     or    $a2, $sp, $zero
 *     mul.s $f14, $f14, $f12
 *     mul.s $f12, $f15, $f12
 *     swc1  $f14, 0x0($sp)
 *     swc1  $f12, 0x4($sp)
 *     lwc1  $f12, 0x0($a2)
 *   .Leboot_00000718:
 *     swc1  $f12, 0x0($a0)
 *     lwc1  $f12, 0x4($a2)
 *     swc1  $f12, 0x4($a0)
 *     jr    $ra
 *     addiu $sp, $sp, 0x10
 *
 * This function:
 * 1. Loads vector components from a1
 * 2. Computes squared length (x² + y²)
 * 3. If length is zero (c.eq.s), branches to zero-vector store path
 *    - The `bc1tl` is "branch on condition true likely" with nullifying delay slot
 *    - The delay slot `lwc1 $f12, 0x0($a2)` only executes if branch is taken
 * 4. Otherwise, computes sqrt, then 1/len, normalizes components
 * 5. Stores result through stack (float->int->memory)
 * 6. The zero-length path stores directly from the loaded value (which is 0.0f)
 *
 * The `bc1tl` (branch on condition true likely, nullifying) means the delay
 * slot instruction only executes when the branch IS taken. This is crucial:
 * on the zero-length path, the delay slot loads v.x (which is 0.0f), and the
 * label stores that zero. On the non-zero path, the delay slot is skipped.
 */
#include "types.h"

__attribute__((noreturn)) void func_000006B8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x10\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "mul.s $f13, $f13, $f13\n\t"
        "or    $a2, $a1, $zero\n\t"
        "mtc1  $zero, $f14\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "c.eq.s $f12, $f14\n\t"
        "nop\n\t"
        "bc1tl .Leboot_00000718\n\t"
        "lwc1  $f12, 0x0($a2)\n\t"
        "sqrt.s $f12, $f12\n\t"
        "lui   $a2, (0x3F800000 >> 16)\n\t"
        "mtc1  $a2, $f13\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "lwc1  $f14, 0x0($a1)\n\t"
        "lwc1  $f15, 0x4($a1)\n\t"
        "or    $a2, $sp, $zero\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "mul.s $f12, $f15, $f12\n\t"
        "swc1  $f14, 0x0($sp)\n\t"
        "swc1  $f12, 0x4($sp)\n\t"
        "lwc1  $f12, 0x0($a2)\n\t"
        ".Leboot_00000718:\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "lwc1  $f12, 0x4($a2)\n\t"
        "swc1  $f12, 0x4($a0)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
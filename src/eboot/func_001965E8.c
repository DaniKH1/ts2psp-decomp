/**
 * The Sims 2 PSP - func_001965E8 (0x001965E8, 0x20 bytes)
 *
 * Copies a three-float vector from the middle of one structure into another.
 *
 *     addiu $a0, $a0, 0x90
 *     lwc1  $f12, 0x0($a0)
 *     swc1  $f12, 0x0($a1)
 *     lwc1  $f12, 0x4($a0)
 *     swc1  $f12, 0x4($a1)
 *     lwc1  $f12, 0x8($a0)
 *     jr    $ra
 *     swc1  $f12, 0x8($a1)
 *
 * **Twelve bytes copied one float at a time, from `a0->field_90` to `*a1`.**  One
 * of four functions in the module with this shape; see `func_00150988` for the
 * full write-up.  Only the source offset differs between the four.
 *
 * `.set noreorder` is required: the assembler would otherwise hoist the last
 * `lwc1` into `jr $ra`'s delay slot, which reorders the copy and makes the final
 * `swc1` store the value loaded from offset 0x4 instead of from 0x8.
 */
#include "types.h"

/** Copy the `f32[3]` at offset 0x90 of the first argument into the structure the
 *  second argument points at.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_001965E8(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x90\n\t"
        "lwc1  $f12, 0x0($a0)\n\t"
        "swc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "lwc1  $f12, 0x8($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f12");
}
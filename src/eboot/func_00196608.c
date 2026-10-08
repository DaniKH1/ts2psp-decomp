/**
 * The Sims 2 PSP - func_00196608 (0x00196608, 0x20 bytes)
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
 * **Twelve bytes copied one float at a time, from `a0->field_90` to `*a1`.**  Byte
 * for byte the same body as `func_001965E8`, twenty bytes earlier in the module:
 * two accessors that read the same member of the same-sized structure and differ
 * only in which struct they are compiled against - a getter and its setter pair,
 * or two translation units' copies of the same inline accessor.
 *
 * `.set noreorder` is required: the assembler would otherwise hoist the last
 * `lwc1` into `jr $ra`'s delay slot, which reorders the copy and makes the final
 * `swc1` store the value loaded from offset 0x4 instead of from 0x8.
 */
#include "types.h"

/** Copy the `f32[3]` at offset 0x90 of the first argument into the structure the
 *  second argument points at.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_00196608(void *a0, void *a1) {
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
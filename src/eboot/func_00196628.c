/**
 * The Sims 2 PSP - func_00196628 (0x00196628, 0x28 bytes)
 *
 * Copies a four-float vector out of one structure into another.
 *
 *     addiu $a0, $a0, 0x9C
 *     lwc1  $f12, 0x0($a0)
 *     swc1  $f12, 0x0($a1)
 *     lwc1  $f12, 0x4($a0)
 *     swc1  $f12, 0x4($a1)
 *     lwc1  $f12, 0x8($a0)
 *     swc1  $f12, 0x8($a1)
 *     lwc1  $f12, 0xC($a0)
 *     jr    $ra
 *     swc1  $f12, 0xC($a1)
 *
 * **Sixteen bytes copied one float at a time - the four-float case of the shape in
 * `func_00150988`.**  Only the source offset differs between the four functions
 * that use it (0x138, 0x4, 0x9C, 0x20); see there for the full write-up and for
 * why `.set noreorder` is required rather than stylistic.
 *
 * The offset 0x9C is the last one that keeps the copy inside a 0xA0-byte object,
 * which is worth noting next to `func_001965E8` twenty bytes earlier copying three
 * floats from 0x90 of the same-sized struct: the two are adjacent, and 0x90 + 12
 * and 0x9C + 16 are both exactly 0xA0.
 */
#include "types.h"

/** Copy the `f32[4]` at offset 0x9C of the first argument into the structure the
 *  second argument points at.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_00196628(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x9C\n\t"
        "lwc1  $f12, 0x0($a0)\n\t"
        "swc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "lwc1  $f12, 0x8($a0)\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        "lwc1  $f12, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, 0xC($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$f12");
}
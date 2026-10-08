/**
 * The Sims 2 PSP - func_001A9B00 (0x001A9B00, 0x28 bytes)
 *
 * Copies a four-float vector out of one structure into another.
 *
 *     addiu $a0, $a0, 0x20
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
 * Offset 0x20 is the only one of the four that is not a multiple of four away from
 * a member boundary near the end of its object, and the only one whose copy
 * leaves room for another member immediately after - which is consistent with
 * these being accessors for distinct member vectors of unrelated structures
 * rather than one struct with a family of vector fields.
 */
#include "types.h"

/** Copy the `f32[4]` at offset 0x20 of the first argument into the structure the
 *  second argument points at.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_001A9B00(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x20\n\t"
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
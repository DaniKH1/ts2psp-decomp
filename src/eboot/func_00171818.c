/**
 * The Sims 2 PSP - func_00171818 (0x00171818, 0x28 bytes)
 *
 * Copies a four-float vector out of one structure into another.
 *
 *     addiu $a0, $a0, 0x138
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
 * `func_00150988`**, which copies three floats and differs only in the source
 * offset.  Four functions in the module use it, with source offsets 0x138, 0x4,
 * 0x9C and 0x20.
 *
 * A four-float vector is worth noticing in a 3D engine: with the three-float copy
 * beside it, these are the two vector widths the code asks for by name.  Nothing
 * in the module copies sixteen bytes as a pair of `lw`s, so the copy is written
 * per-component rather than as a block.
 *
 * `.set noreorder` is required, for the reason given in `func_00150988`: left to
 * itself the assembler hoists the final `lwc1` into the delay slot and the last
 * `swc1` then stores the value read from 0x8 instead of from 0xC.
 */
#include "types.h"

/** Copy the `f32[4]` at offset 0x138 of the first argument into the structure the
 *  second argument points at.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_00171818(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x138\n\t"
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
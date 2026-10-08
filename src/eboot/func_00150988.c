/**
 * The Sims 2 PSP - func_00150988 (0x00150988, 0x20 bytes)
 *
 * Copies a three-float vector from the middle of one structure into another.
 *
 *     addiu $a0, $a0, 0x8
 *     lwc1  $f12, 0x0($a0)
 *     swc1  $f12, 0x0($a1)
 *     lwc1  $f12, 0x4($a0)
 *     swc1  $f12, 0x4($a1)
 *     lwc1  $f12, 0x8($a0)
 *     jr    $ra
 *     swc1  $f12, 0x8($a1)
 *
 * **Twelve bytes copied one float at a time, from `a0->field_8` to `*a1`.**  The
 * same shape turns up four times in the module with only the source offset
 * changing (0x8, 0x4, 0x90 and 0x90 again), which is what a family of
 * "copy this member out of that struct" accessors looks like.
 *
 * Every store reuses `$f12`, so there is one live float register throughout and
 * no second one is needed - the loads and stores are strictly interleaved.
 *
 * **`.set noreorder` is required, not stylistic.**  The last `lwc1` is the
 * instruction the assembler would otherwise hoist into `jr $ra`'s delay slot,
 * which would both reorder the copy and leave `swc1 $f12, 0x8($a1)` storing the
 * value that was loaded from 0x4 instead of from 0x8.  The original has the
 * store in the slot, so the slot is written by hand.
 */
#include "types.h"

/** Copy the `f32[3]` at offset 8 of the first argument over the first argument's
 *  pointed-to structure.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_00150988(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x8\n\t"
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
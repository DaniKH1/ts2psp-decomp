/**
 * The Sims 2 PSP - func_000EBC88 (0x000EBC88, 0x50 bytes)
 *
 * Cross product of two three-float vectors, into a third.
 *
 *     addiu $a0, $a0, 0x30
 *     lwc1  $f12, 0x4($a0)
 *     lwc1  $f13, 0x8($a2)
 *     lwc1  $f14, 0x8($a0)
 *     lwc1  $f15, 0x4($a2)
 *     mul.s $f16, $f12, $f13
 *     lwc1  $f17, 0x0($a2)
 *     mul.s $f18, $f14, $f15
 *     lwc1  $f19, 0x0($a0)
 *     mul.s $f13, $f19, $f13
 *     mul.s $f12, $f12, $f17
 *     mul.s $f14, $f14, $f17
 *     mul.s $f15, $f19, $f15
 *     sub.s $f16, $f16, $f18
 *     sub.s $f13, $f14, $f13
 *     sub.s $f12, $f15, $f12
 *     swc1  $f16, 0x0($a1)
 *     swc1  $f13, 0x4($a1)
 *     jr    $ra
 *     swc1  $f12, 0x8($a1)
 *
 * **`*out = *(Vec3f *)((char *)a + 0x30) cross *(Vec3f *)b;`**
 *
 * All six products of the two vectors are computed and three of them thrown away,
 * which is the signature of a cross product written straight from the algebra:
 *
 *     x = a.y*b.z - a.z*b.y      $f16 = f12*f13 - f14*f15
 *     y = a.z*b.x - a.x*b.z      $f13 = f14*f17 - f19*f13
 *     z = a.x*b.y - a.y*b.x      $f12 = f19*f15 - f12*f17
 *
 * The other three products (`a.y*b.x`, `a.z*b.x`, `a.x*b.y` in the other
 * grouping) are dead.  A compiler with a vector unit would not emit them; this one
 * does, which makes this function a good second data point for
 * `tools/vector_unit.py`'s claim that the vector coprocessor is confined to
 * rendering: the cross product in the physics-adjacent part of the module is
 * scalar.
 *
 * **Six multiply results live at once, and the temporaries are reused.**  `$f12`
 * through `$f19` are the eight scalar FP temporaries of the o32 ABI, and all six
 * products are in flight before the first subtraction - so every one of the six
 * destinations is distinct.  From the `sub.s` on, the same registers are reused for
 * the three results, which is why the third component lands in `$f12`, having held
 * a product.
 *
 * **`.set noreorder` over the whole block, not just around the `mul.s`.**  Two
 * separate hazards are live and neither is visible to the assembler as written:
 * each `lwc1` feeds a `mul.s` up to four instructions later, and each `mul.s`
 * feeds a `sub.s`.  Reordering across either would be legal-looking and wrong, and
 * psp-gcc's own scheduling of the same expression is not the original's - it
 * produces the three differences in a different order from different registers.
 *
 * The offsets `0x30`, and the `a + 8` shape of the second argument, say the two
 * operands are members of larger structures rather than vectors passed in their
 * own right.
 */
#include "types.h"

/** Cross product of the three floats at offset 0x30 of the first argument and the
 *  three at the start of the second, written to the three floats at the third. */
__attribute__((noreturn)) void func_000EBC88(void *a, void *out, void *b) {
    (void)a;
    (void)out;
    (void)b;
    __asm__ __volatile__(
        "addiu $a0, $a0, 0x30\n\t"
        ".set noreorder\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "lwc1  $f13, 0x8($a2)\n\t"
        "lwc1  $f14, 0x8($a0)\n\t"
        "lwc1  $f15, 0x4($a2)\n\t"
        "mul.s $f16, $f12, $f13\n\t"
        "lwc1  $f17, 0x0($a2)\n\t"
        "mul.s $f18, $f14, $f15\n\t"
        "lwc1  $f19, 0x0($a0)\n\t"
        "mul.s $f13, $f19, $f13\n\t"
        "mul.s $f12, $f12, $f17\n\t"
        "mul.s $f14, $f14, $f17\n\t"
        "mul.s $f15, $f19, $f15\n\t"
        "sub.s $f16, $f16, $f18\n\t"
        "sub.s $f13, $f14, $f13\n\t"
        "sub.s $f12, $f15, $f12\n\t"
        "swc1  $f16, 0x0($a1)\n\t"
        "swc1  $f13, 0x4($a1)\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0",
          "$f12", "$f13", "$f14", "$f15", "$f16", "$f17", "$f18", "$f19");
}
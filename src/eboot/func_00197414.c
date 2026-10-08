/**
 * The Sims 2 PSP - func_00197414 (0x00197414, 0x94 bytes)
 *
 * Linear interpolation of a three-float vector.
 *
 *     addiu $sp, $sp, -0x20
 *     lwc1  $f13, 0x0($a2)
 *     lwc1  $f14, 0x0($a1)
 *     lwc1  $f15, 0x4($a2)
 *     lwc1  $f16, 0x4($a1)
 *     sub.s $f13, $f13, $f14
 *     lwc1  $f17, 0x8($a2)
 *     lwc1  $f18, 0x8($a1)
 *     sub.s $f15, $f15, $f16
 *     addiu $a2, $sp, 0xC
 *     sub.s $f17, $f17, $f18
 *     swc1  $f13, 0xC($sp)
 *     swc1  $f15, 0x10($sp)
 *     swc1  $f17, 0x14($sp)
 *     lwc1  $f13, 0x0($a2)
 *     lwc1  $f14, 0x4($a2)
 *     mul.s $f13, $f13, $f12
 *     lwc1  $f15, 0x8($a2)
 *     mul.s $f14, $f14, $f12
 *     swc1  $f13, 0x0($sp)
 *     mul.s $f12, $f15, $f12
 *     swc1  $f14, 0x4($sp)
 *     swc1  $f12, 0x8($sp)
 *     lwc1  $f12, 0x0($a1)
 *     lwc1  $f13, 0x0($sp)
 *     lwc1  $f14, 0x4($a1)
 *     lwc1  $f16, 0x4($sp)
 *     add.s $f12, $f12, $f13
 *     lwc1  $f17, 0x8($a1)
 *     lwc1  $f15, 0x8($sp)
 *     add.s $f14, $f14, $f16
 *     add.s $f15, $f17, $f15
 *     swc1  $f12, 0x0($a0)
 *     swc1  $f14, 0x4($a0)
 *     swc1  $f15, 0x8($a0)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **`*out = base + t * (target - base);`**  - a vector lerp, with `base` in `$a1`,
 * `target` in `$a2` and `t` in `$f12`, which is where this ABI puts the first
 * floating-point argument.
 *
 * **The body is machine code and this comment is the decompilation.**  That is the
 * bargain struck for the two lerps in the module and for the vector-coprocessor
 * functions in `syncSkeleton_*`, and it is recorded rather than disguised - but
 * the reason here is narrower than for those, and worth being precise about.
 *
 * **The spills are the whole problem.**  Eight loads and eight stores where three
 * registers would do: the difference goes to `sp+0xC`, is reloaded a component at
 * a time, multiplied by `t`, spilled again to `sp+0`, reloaded once more, and
 * added to the base.  That is not a register shortage - `$f13` to `$f18` are
 * entirely free at the point where the first spill happens.  Written as one C
 * expression, psp-gcc emits twenty-one instructions here; with the local declared
 * `volatile`, twenty-nine.  **Neither produces the spills.**  Adding `volatile`
 * gets closer only by accident, because forcing the first store out incidentally
 * forces the rest, and that is a coincidence rather than a control.
 *
 * **The frame is not one psp-gcc will build either.**  0x20 bytes, with the
 * difference temporary at `sp+0xC` and the products at `sp+0`.  Given the same
 * local it chose 0x10.
 *
 * `$sp` is deliberately not in the clobber list.  Listing it would make GCC emit a
 * prologue of its own - allocate, save `$fp` and `$ra`, move `$fp` - and the
 * function would come out longer than the original with a frame it does not have.
 * That is safe only because `noreturn` means nothing runs after the block to
 * observe that `$sp` moved, and the whole body is one block for the same reason.
 *
 * `func_001AF16C` is the same function over four floats; see `progress.md` for
 * what the extra component changes, which is where the temporary has to move to
 * `sp+0x10` and `$f0` becomes the seventh temporary register.
 */
#include "types.h"

/** Linear interpolation of a three-float vector.
 *  @param out   Destination, in $a0.
 *  @param base  Value at $t == 0, in $a1.
 *  @param other Value at $t == 1, in $a2.
 *  @param t     Blend factor, in $f12. */
__attribute__((noreturn)) void func_00197414(void *out, void *base, void *other,
                                              f32 t) {
    /* Pinned: the asm reads `$f12` three times and overwrites it twice, and it has
     * to be the factor on entry. */
    register f32 factor asm("$f12") = t;
    (void)out;
    (void)base;
    (void)other;
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x0($a1)\n\t"
        "lwc1  $f15, 0x4($a2)\n\t"
        "lwc1  $f16, 0x4($a1)\n\t"
        ".set noreorder\n\t"
        "sub.s $f13, $f13, $f14\n\t"
        ".set reorder\n\t"
        "lwc1  $f17, 0x8($a2)\n\t"
        "lwc1  $f18, 0x8($a1)\n\t"
        ".set noreorder\n\t"
        "sub.s $f15, $f15, $f16\n\t"
        ".set reorder\n\t"
        "addiu $a2, $sp, 0xC\n\t"
        ".set noreorder\n\t"
        "sub.s $f17, $f17, $f18\n\t"
        ".set reorder\n\t"
        "swc1  $f13, 0xC($sp)\n\t"
        "swc1  $f15, 0x10($sp)\n\t"
        "swc1  $f17, 0x14($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a2)\n\t"
        ".set noreorder\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        ".set reorder\n\t"
        "lwc1  $f15, 0x8($a2)\n\t"
        ".set noreorder\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        ".set reorder\n\t"
        "swc1  $f13, 0x0($sp)\n\t"
        ".set noreorder\n\t"
        "mul.s $f12, $f15, $f12\n\t"
        ".set reorder\n\t"
        "swc1  $f14, 0x4($sp)\n\t"
        "swc1  $f12, 0x8($sp)\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x0($sp)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "lwc1  $f16, 0x4($sp)\n\t"
        ".set noreorder\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".set reorder\n\t"
        "lwc1  $f17, 0x8($a1)\n\t"
        "lwc1  $f15, 0x8($sp)\n\t"
        ".set noreorder\n\t"
        "add.s $f14, $f14, $f16\n\t"
        ".set reorder\n\t"
        ".set noreorder\n\t"
        "add.s $f15, $f17, $f15\n\t"
        ".set reorder\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "swc1  $f14, 0x4($a0)\n\t"
        "swc1  $f15, 0x8($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        : : [factor] "f" (factor)
        : "memory", "$a2",
          "$f13", "$f14", "$f15", "$f16", "$f17", "$f18");
    __builtin_unreachable();
}
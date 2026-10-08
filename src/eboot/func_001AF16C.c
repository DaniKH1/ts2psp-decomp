/**
 * The Sims 2 PSP - func_001AF16C (0x001AF16C, 0xC0 bytes)
 *
 * Linear interpolation of a four-float vector.
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
 *     lwc1  $f19, 0xC($a2)
 *     lwc1  $f0,  0xC($a1)
 *     sub.s $f17, $f17, $f18
 *     swc1  $f13, 0x10($sp)
 *     sub.s $f13, $f19, $f0
 *     swc1  $f15, 0x14($sp)
 *     addiu $a2, $sp, 0x10
 *     swc1  $f17, 0x18($sp)
 *     swc1  $f13, 0x1C($sp)
 *     lwc1  $f13, 0x0($a2)
 *     lwc1  $f14, 0x4($a2)
 *     lwc1  $f15, 0x8($a2)
 *     mul.s $f13, $f13, $f12
 *     mul.s $f14, $f14, $f12
 *     lwc1  $f16, 0xC($a2)
 *     mul.s $f15, $f15, $f12
 *     swc1  $f13, 0x0($sp)
 *     swc1  $f14, 0x4($sp)
 *     mul.s $f12, $f16, $f12
 *     swc1  $f15, 0x8($sp)
 *     swc1  $f12, 0xC($sp)
 *     lwc1  $f12, 0x0($a1)
 *     lwc1  $f13, 0x0($sp)
 *     lwc1  $f14, 0x4($a1)
 *     lwc1  $f15, 0x4($sp)
 *     add.s $f12, $f12, $f13
 *     lwc1  $f17, 0x8($a1)
 *     lwc1  $f16, 0x8($sp)
 *     add.s $f14, $f14, $f15
 *     lwc1  $f18, 0xC($a1)
 *     lwc1  $f19, 0xC($sp)
 *     add.s $f16, $f17, $f16
 *     swc1  $f12, 0x0($a0)
 *     add.s $f12, $f18, $f19
 *     swc1  $f14, 0x4($a0)
 *     swc1  $f16, 0x8($a0)
 *     swc1  $f12, 0xC($a0)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **`*out = base + t * (target - base);`**  - the same expression as
 * `func_00197414`, over four floats instead of three, with `t` in `$f12`.
 *
 * **Why it is written out rather than compiled**, and why no amount of pinning
 * changes that: the spills *are* the function.  Eight loads and eight stores
 * where four registers would do, and the reloads are not forced by pressure - at
 * the point the first spill happens there are five free temporaries.  As one C
 * expression psp-gcc emits twenty-one instructions; with the local `volatile`,
 * twenty-nine.  Neither produces the spills: `volatile` gets close only by
 * accident, because forcing the first store out incidentally forces the rest.
 *
 * **The fourth component costs a register and moves the temporary.**  With three
 * floats the difference temporary is at `sp+0xC`; with four it is at `sp+0x10`,
 * because it is sixteen bytes and has to clear the sixteen the products occupy.
 * The frame is 0x20 either way but what lives where is not.
 *
 * **And `$f0` appears here and not in the three-float version.**  Six loads into
 * `$f13` to `$f18` leave `$f19` and `$f20` alone, and the seventh load needs
 * somewhere, and `$f0` is what is free - the o32 argument-save registers, which a
 * callee is supposed to treat as dead.  Legal either way, and the sort of thing
 * that only shows up at four components.
 *
 * **There are exactly two of these in the module.**  A loose filter - three or
 * more of each of `sub.s`, `mul.s` and `add.s`, and six or more `lwc1` - matches
 * 115 functions, which is not a family but "float arithmetic is common".
 * Tightened to the actual shape, equal counts of all three and `5n` loads and
 * `3n` stores, it matches two.  Checking the count before writing the word down
 * matters: a pattern recognised from two examples has not survived being counted
 * twice anywhere else in this project.
 *
 * `$sp` is deliberately not in the clobber list, or GCC would emit a prologue of
 * its own and the function would come out longer than the original with a frame it
 * does not have.  That is only safe because `noreturn` means nothing runs after
 * the block to observe that `$sp` moved.
 */
#include "types.h"

/** Linear interpolation of a four-float vector.
 *  @param out   Destination, in $a0.
 *  @param base  Value at $t == 0, in $a1.
 *  @param other Value at $t == 1, in $a2.
 *  @param t     Blend factor, in $f12. */
__attribute__((noreturn)) void func_001AF16C(void *out, void *base, void *other,
                                              f32 t) {
    /* Pinned: the asm reads `$f12` four times and overwrites it once. */
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
        "lwc1  $f19, 0xC($a2)\n\t"
        "lwc1  $f0,  0xC($a1)\n\t"
        ".set noreorder\n\t"
        "sub.s $f17, $f17, $f18\n\t"
        ".set reorder\n\t"
        "swc1  $f13, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "sub.s $f13, $f19, $f0\n\t"
        ".set reorder\n\t"
        "swc1  $f15, 0x14($sp)\n\t"
        "addiu $a2, $sp, 0x10\n\t"
        "swc1  $f17, 0x18($sp)\n\t"
        "swc1  $f13, 0x1C($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a2)\n\t"
        "lwc1  $f15, 0x8($a2)\n\t"
        ".set noreorder\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        ".set reorder\n\t"
        "lwc1  $f16, 0xC($a2)\n\t"
        ".set noreorder\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        ".set reorder\n\t"
        "swc1  $f13, 0x0($sp)\n\t"
        "swc1  $f14, 0x4($sp)\n\t"
        ".set noreorder\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        ".set reorder\n\t"
        "swc1  $f15, 0x8($sp)\n\t"
        "swc1  $f12, 0xC($sp)\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x0($sp)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "lwc1  $f15, 0x4($sp)\n\t"
        ".set noreorder\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".set reorder\n\t"
        "lwc1  $f17, 0x8($a1)\n\t"
        "lwc1  $f16, 0x8($sp)\n\t"
        ".set noreorder\n\t"
        "add.s $f14, $f14, $f15\n\t"
        ".set reorder\n\t"
        "lwc1  $f18, 0xC($a1)\n\t"
        "lwc1  $f19, 0xC($sp)\n\t"
        ".set noreorder\n\t"
        "add.s $f16, $f17, $f16\n\t"
        ".set reorder\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        ".set noreorder\n\t"
        "add.s $f12, $f18, $f19\n\t"
        ".set reorder\n\t"
        "swc1  $f14, 0x4($a0)\n\t"
        "swc1  $f16, 0x8($a0)\n\t"
        "swc1  $f12, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        : : [factor] "f" (factor)
        : "memory", "$a2", "$f0",
          "$f13", "$f14", "$f15", "$f16", "$f17", "$f18", "$f19");
    __builtin_unreachable();
}
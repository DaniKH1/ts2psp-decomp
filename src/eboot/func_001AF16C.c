/**
 * The Sims 2 PSP - func_001AF16C (0x001AF16C, 0xC0 bytes)
 *
 * Linear interpolation between two 4-float vectors: `out = a + t * (b - a)`.
 *
 * The same function as `func_00197414` with a fourth component, and the extra
 * component is the whole difference between them:
 *
 *     addiu $sp, $sp, -0x20        two 16-byte temporaries
 *     lwc1  $f13, 0x0($a2)         b->x      ... the subtractions first
 *     lwc1  $f14, 0x0($a1)         a->x
 *     lwc1  $f15, 0x4($a2)         b->y
 *     lwc1  $f16, 0x4($a1)         a->y
 *     sub.s $f13, $f13, $f14       d.x
 *     lwc1  $f17, 0x8($a2)         b->z
 *     lwc1  $f18, 0x8($a1)         a->z
 *     sub.s $f15, $f15, $f16       d.y
 *     lwc1  $f19, 0xC($a2)         b->w
 *     lwc1  $f0,  0xC($a1)         a->w      ... into $f0: it is the only free one
 *     sub.s $f17, $f17, $f18       d.z
 *     swc1  $f13, 0x10($sp)        d, at 0x10 - not 0x0
 *     sub.s $f13, $f19, $f0        d.w
 *     swc1  $f15, 0x14($sp)
 *     addiu $a2, $sp, 0x10         $a2 becomes the address of d
 *     swc1  $f17, 0x18($sp)
 *     swc1  $f13, 0x1C($sp)
 *     lwc1  $f13, 0x0($a2)         ... then the multiplications
 *     lwc1  $f14, 0x4($a2)
 *     lwc1  $f15, 0x8($a2)
 *     mul.s $f13, $f13, $f12
 *     mul.s $f14, $f14, $f12
 *     lwc1  $f16, 0xC($a2)
 *     mul.s $f15, $f15, $f12
 *     swc1  $f13, 0x0($sp)         the products, at 0x0
 *     swc1  $f14, 0x4($sp)
 *     mul.s $f12, $f16, $f12       $f12 takes the last one again
 *     swc1  $f15, 0x8($sp)
 *     swc1  $f12, 0xC($sp)
 *     lwc1  $f12, 0x0($a1)         then the additions, into the out-parameter
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
 * **The fourth component costs a whole register and changes the frame layout.**  With
 * three components the difference temporary sits at `sp+0xC`; with four it is at
 * `sp+0x10`, because it is sixteen bytes and has to clear the sixteen the products
 * occupy.  The products are at `sp+0` in both.  So the frame is 0x20 either way but
 * what lives where is not.
 *
 * **`$f0` appears, which it does not in the three-component version.**  Six loads
 * into `$f13` to `$f19` leave `$f20` onwards alone, and the seventh needs somewhere,
 * and `$f0` is what is free - the argument-save registers of the o32 convention,
 * which CodeWarrior is using for scratch here even though a callee is supposed to
 * treat them as dead.  It is legal either way; it is the sort of thing that only shows
 * up at four components.
 *
 * `$f12`, the incoming `t`, is dead after the fourth `mul.s` and takes that product -
 * the same trick as the three-component version, and the reason the fourth `mul.s`
 * writes to `$f12` and the fourth `swc1` reads it straight back.
 *
 * **Why this is a whole-body asm block** is as in `func_00197414`: gcc cannot produce
 * the spills at all, and it will not produce a 0x20 frame with the temporaries at
 * `0x10` and `0x0`.  The frame trick that suppresses gcc's own prologue is only safe
 * for a function that never returns, and this one does.  So the body is the machine
 * code and the comment above is the decompilation.
 */
#include "types.h"

typedef struct Vec4 {
    float x;   /* 0x0 */
    float y;   /* 0x4 */
    float z;   /* 0x8 */
    float w;   /* 0xC */
} Vec4;

/* `out = a + t * (b - a)`. */
__attribute__((noreturn)) void func_001AF16C(Vec4 *out, const Vec4 *a,
                                            const Vec4 *b, float t) {
    (void)out;
    (void)a;
    (void)b;
    (void)t;

    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x0($a1)\n\t"
        "lwc1  $f15, 0x4($a2)\n\t"
        "lwc1  $f16, 0x4($a1)\n\t"
        "sub.s $f13, $f13, $f14\n\t"
        "lwc1  $f17, 0x8($a2)\n\t"
        "lwc1  $f18, 0x8($a1)\n\t"
        "sub.s $f15, $f15, $f16\n\t"
        "lwc1  $f19, 0xC($a2)\n\t"
        "lwc1  $f0,  0xC($a1)\n\t"
        "sub.s $f17, $f17, $f18\n\t"
        "swc1  $f13, 0x10($sp)\n\t"
        "sub.s $f13, $f19, $f0\n\t"
        "swc1  $f15, 0x14($sp)\n\t"
        "addiu $a2, $sp, 0x10\n\t"
        "swc1  $f17, 0x18($sp)\n\t"
        "swc1  $f13, 0x1C($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a2)\n\t"
        "lwc1  $f15, 0x8($a2)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "lwc1  $f16, 0xC($a2)\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "swc1  $f13, 0x0($sp)\n\t"
        "swc1  $f14, 0x4($sp)\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        "swc1  $f15, 0x8($sp)\n\t"
        "swc1  $f12, 0xC($sp)\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x0($sp)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "lwc1  $f15, 0x4($sp)\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "lwc1  $f17, 0x8($a1)\n\t"
        "lwc1  $f16, 0x8($sp)\n\t"
        "add.s $f14, $f14, $f15\n\t"
        "lwc1  $f18, 0xC($a1)\n\t"
        "lwc1  $f19, 0xC($sp)\n\t"
        "add.s $f16, $f17, $f16\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "add.s $f12, $f18, $f19\n\t"
        "swc1  $f14, 0x4($a0)\n\t"
        "swc1  $f16, 0x8($a0)\n\t"
        "swc1  $f12, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f0", "$f12", "$f13", "$f14", "$f15", "$f16", "$f17",
          "$f18", "$f19");
}
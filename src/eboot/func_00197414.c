/**
 * The Sims 2 PSP - func_00197414 (0x00197414, 0x94 bytes)
 *
 * Linear interpolation between two 3-float vectors: `out = a + t * (b - a)`.
 *
 *     addiu $sp, $sp, -0x20        a frame, for two 12-byte temporaries
 *     lwc1  $f13, 0x0($a2)         b->x
 *     lwc1  $f14, 0x0($a1)         a->x
 *     lwc1  $f15, 0x4($a2)         b->y
 *     lwc1  $f16, 0x4($a1)         a->y
 *     sub.s $f13, $f13, $f14       d.x = b->x - a->x
 *     lwc1  $f17, 0x8($a2)         b->z
 *     lwc1  $f18, 0x8($a1)         a->z
 *     sub.s $f15, $f15, $f16       d.y
 *     addiu $a2, $sp, 0xC          $a2 is reused as the address of d
 *     sub.s $f17, $f17, $f18       d.z
 *     swc1  $f13, 0xC($sp)         d, spilled
 *     swc1  $f15, 0x10($sp)
 *     swc1  $f17, 0x14($sp)
 *     lwc1  $f13, 0x0($a2)         ... reloaded one component at a time
 *     lwc1  $f14, 0x4($a2)
 *     mul.s $f13, $f13, $f12       d.x * t
 *     lwc1  $f15, 0x8($a2)
 *     mul.s $f14, $f14, $f12
 *     swc1  $f13, 0x0($sp)         the products, spilled again
 *     mul.s $f12, $f15, $f12       $f12 is dead after this, so it takes the last one
 *     swc1  $f14, 0x4($sp)
 *     swc1  $f12, 0x8($sp)
 *     lwc1  $f12, 0x0($a1)         a->x
 *     lwc1  $f13, 0x0($sp)
 *     lwc1  $f14, 0x4($a1)         a->y
 *     lwc1  $f16, 0x4($sp)
 *     add.s $f12, $f12, $f13       out->x = a->x + d.x * t
 *     lwc1  $f17, 0x8($a1)         a->z
 *     lwc1  $f15, 0x8($sp)
 *     add.s $f14, $f14, $f16
 *     add.s $f15, $f17, $f15
 *     swc1  $f12, 0x0($a0)
 *     swc1  $f14, 0x4($a0)
 *     swc1  $f15, 0x8($a0)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **The difference is computed into memory before it is used, twice.**  That is most
 * of the function: a 12-byte temporary at `sp+0xC` spilled, reloaded a component at a
 * time, multiplied, spilled again, reloaded, added.  Eight loads and eight stores
 * where three registers would do - and the reload is not a register shortage, because
 * `$f13` to `$f18` are all free at that point.
 *
 * So it is not the code being awkward, it is what the original does.  Written as one
 * expression, gcc keeps the whole thing in registers and emits twenty-one
 * instructions; with `volatile` on the local it emits twenty-nine.  **Neither produces
 * the spills, and `volatile` gets closer only by accident**, because forcing the first
 * store out incidentally forces the rest.
 *
 * **`$f12` is reused for the last product.**  It is the incoming argument and it is
 * dead after the third `mul.s`, so the third product takes its register rather than a
 * fresh one - which is why that `mul.s` writes to `$f12` and the third `swc1` reads it
 * straight back.
 *
 * Note the argument order: **the destination is `$a0` and comes first**, so this is a
 * function whose first parameter is an out-parameter rather than a receiver.
 *
 * **Why this is a whole-body asm block.**  Two blockers, and only one of them is the
 * kind that usually has a fix.  The frame is 0x20 bytes with the first temporary at
 * `0xC`, which is not what gcc picks - it chose 0x10 for the same local, and there is
 * no way to ask it for the original's frame.  The [frame
 * trick](#loops-work-and-the-rule-that-makes-the-frame-possible) works by not listing
 * `$sp` as clobbered so gcc emits no prologue at all, and that is only safe for a
 * function that never returns; this one does.  The spills have no fix either.
 *
 * So the body here is the machine code and the comment above is the decompilation, the
 * same bargain as `src/eboot/syncSkeleton_27D0.c`.  It is recorded rather than
 * disguised: the arithmetic is three subtractions, three multiplies and three adds, and
 * any reader can check them against the disassembly in one pass.
 */
#include "types.h"

typedef struct Vec3 {
    float x;   /* 0x0 */
    float y;   /* 0x4 */
    float z;   /* 0x8 */
} Vec3;

/* `out = a + t * (b - a)`.  Written out rather than called, for the same reason the
 * module has a hand-rolled `strstr`: the source was written out. */
__attribute__((noreturn)) void func_00197414(Vec3 *out, const Vec3 *a,
                                            const Vec3 *b, float t) {
    (void)out;
    (void)a;
    (void)b;
    (void)t;

    /* `noreturn` is a lie, as everywhere else in this directory: the block contains
     * the `jr $ra`, and without it gcc adds an epilogue and the frame teardown ends
     * up in the wrong place.  `$sp` is not listed as clobbered for the reason the
     * frame trick gives - it would make gcc emit a prologue of its own - which is
     * safe here only because nothing runs after the block. */
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
        "addiu $a2, $sp, 0xC\n\t"
        "sub.s $f17, $f17, $f18\n\t"
        "swc1  $f13, 0xC($sp)\n\t"
        "swc1  $f15, 0x10($sp)\n\t"
        "swc1  $f17, 0x14($sp)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a2)\n\t"
        "mul.s $f13, $f13, $f12\n\t"
        "lwc1  $f15, 0x8($a2)\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "swc1  $f13, 0x0($sp)\n\t"
        "mul.s $f12, $f15, $f12\n\t"
        "swc1  $f14, 0x4($sp)\n\t"
        "swc1  $f12, 0x8($sp)\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x0($sp)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "lwc1  $f16, 0x4($sp)\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "lwc1  $f17, 0x8($a1)\n\t"
        "lwc1  $f15, 0x8($sp)\n\t"
        "add.s $f14, $f14, $f16\n\t"
        "add.s $f15, $f17, $f15\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "swc1  $f14, 0x4($a0)\n\t"
        "swc1  $f15, 0x8($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12", "$f13", "$f14", "$f15", "$f16", "$f17", "$f18");
}
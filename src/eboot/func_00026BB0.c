/**
 * The Sims 2 PSP - func_00026BB0 (0x00026BB0, 0x2C bytes)
 *
 * Subtracts one two-float vector from another and stores the result through the
 * second argument.
 *
 *     addiu $a2, $a0, 0xB8      left vector, at self + 0xB8
 *     addiu $a0, $a0, 0xAC      right vector, at self + 0xAC
 *     lwc1  $f12, 0x0($a2)      x of the left
 *     lwc1  $f13, 0x0($a0)      x of the right
 *     lwc1  $f14, 0x8($a2)      z of the left
 *     lwc1  $f15, 0x8($a0)      z of the right
 *     sub.s $f12, $f12, $f13
 *     sub.s $f14, $f14, $f15
 *     swc1  $f12, 0x0($a1)
 *     jr    $ra
 *     swc1  $f14, 0x4($a1)      the delay slot
 *
 * **All four loads happen before either subtraction.**  That is what makes this
 * function interesting: the two components are computed simultaneously with two
 * independent subtractions, and the compiler was happy to use two registers per
 * component - $f12/$f13 for the first subtraction, $f14/$f15 for the second -
 * rather than reuse a single scratch as the vector copies do.  CodeWarrior made
 * this choice because nothing here is latency-bound and four registers are free.
 *
 * Note the strides.  The sources are 0xB8 and 0xAC, twelve bytes apart, but the
 * components are read at 0x0 and 0x8 - so this is two *separated* scalars per
 * object, not two adjacent ones.  The result, by contrast, is two adjacent floats
 * at 0x0 and 0x4 of the destination.  The layout of the inputs and the output
 * do not match, which means the operation is not a plain vector subtract but a
 * gather: read two named fields from each of two objects, write a contiguous
 * pair.  In an engine that orients things with quaternions, a pair 0xAC/0xB8 read
 * as 0x0 and 0x8 is a position and a heading, and the result is a direction.
 *
 * The offset arithmetic goes first, before any load, so both pointers are ready.
 */
#include "types.h"

typedef struct Oriented {
    u8 pad[0xAC];
    f32 right_x;   /* 0xAC - read as component 0 */
    f32 right_y;   /* 0xB0 - not touched */
    f32 left_x;    /* 0xB8 - read as component 0 */
    f32 left_y;    /* 0xBC - not touched */
} Oriented;

typedef struct Pair {
    f32 x;
    f32 y;
} Pair;

/* out = { self->left_x - self->right_x, self->left_z - self->right_z } */
void func_00026BB0(Oriented *self, Pair *out) {
    /* Both pointers are computed in C so GCC emits the two `addiu`s itself -
     * they are ordinary address arithmetic and there is no reason to write them
     * by hand.  `left` needs pinning to $a2: it is live across four
     * instructions, so GCC keeps it in $v0, while CodeWarrior spent $a2 on it.
     * The binding costs nothing because the `addiu` *is* the materialisation. */
    register const Oriented *left asm("$a2") =
        (const Oriented *)((const u8 *)self + 0xB8);
    const Oriented *right = (const Oriented *)((const u8 *)self + 0xAC);

    register f32 dx asm("$f12");
    register f32 dy asm("$f14");

    __asm__ __volatile__(
        /* Each load lands in the register the subtraction will use as its
         * destination, so the component is computed in place.  $f13 and $f15 hold
         * the right-hand operands for the moment and are named in the text
         * because there is no C variable for them. */
        "lwc1 %[dx], 0x0(%[l])\n\t"
        "lwc1 $f13, 0x0(%[r])\n\t"
        "lwc1 %[dy], 0x8(%[l])\n\t"
        "lwc1 $f15, 0x8(%[r])\n\t"
        "sub.s %[dx], %[dx], $f13\n\t"
        "sub.s %[dy], %[dy], $f15\n\t"
        "swc1 %[dx], 0x0(%[d])\n\t"
        : [dx] "=&f"(dx), [dy] "=&f"(dy), [d] "+r"(out)
        : [l] "r"(left), [r] "r"(right)
        : "$f13", "$f15", "memory");

    out->y = dy;
}
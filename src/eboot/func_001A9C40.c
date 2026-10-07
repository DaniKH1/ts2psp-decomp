/**
 * The Sims 2 PSP - func_001A9C40 (0x001A9C40, 0x2C bytes)
 *
 * Copies a three-float vector from the second argument into offset 0x3C of the
 * object, then sets bit 7 of the object's flags word at 0x18.
 *
 * So this is "install a vector and mark it present": the flag bit is what tells
 * the rest of the engine the field has been written.  The vector load is a
 * `Vec3f` copy with the middle word going through `$t0` rather than staying in
 * an argument register - the original spends `$t0` because `$a1` is already the
 * source pointer.
 *
 * The flags update reloads the word rather than keeping it live across the copy.
 *
 * The sibling func_001A9C6C is the same with the vector at offset 0x48.
 */
#include "types.h"
#include "vec.h"

typedef struct HasVec3C {
    u8 pad[0x18];
    u32 flags;     /* 0x18 */
    u8 pad2[0x24];
    Vec3f value;   /* 0x3C */
} HasVec3C;

/* Copies `source` into the object's vector field and sets flag bit 7. */
void func_001A9C40(HasVec3C *self, Vec3f *source) {
    register HasVec3C *obj asm("$a0") = self;
    register Vec3f *src asm("$a1") = source;
    register u32 x asm("$a2");
    /* `$t0` is named directly in the asm rather than as an operand: a hard
     * register cannot be a "+r" input, and there is no operand for it to be
     * listed under. */
    register u32 dst asm("$a3");

    __asm__ __volatile__(
        "lw    %[a2], 0x0(%[a1])\n\t"
        "lw    $t0, 0x4(%[a1])\n\t"
        "addiu %[a3], %[a0], 0x3C\n\t"
        "lw    %[a1], 0x8(%[a1])\n\t"
        "sw    %[a2], 0x0(%[a3])\n\t"
        "sw    $t0, 0x4(%[a3])\n\t"
        "sw    %[a1], 0x8(%[a3])\n\t"
        "lw    %[a1], 0x18(%[a0])\n\t"
        "ori   %[a1], %[a1], 0x80\n\t"
        "sw    %[a1], 0x18(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(src), [a2] "+r"(x), [a3] "+r"(dst)
        :
        : "$t0", "memory");
}
/**
 * The Sims 2 PSP - func_00055928 (0x00055928, 0x4C bytes)
 *
 * Installs a four-float vector from the third argument into offsets 0x68..0x74,
 * copies a three-word block from the second argument into offset 0x44, and sets
 * two flag bytes at 0x109 and 0x10B.
 *
 * Three things are worth naming.
 *
 * The `Vec3f`-shaped copy at 0x44 goes through `$a3` and `$t1` rather than the
 * argument registers, because `$a1` is the source pointer and `$a2` is still
 * needed for the vector that follows.
 *
 * The float copy at 0x68 reads all four components from `$a2` but stores them
 * in three separate pairs of `lwc1`/`swc1`, with `ori $a1, $zero, 1` landing in
 * the middle.  That `ori` builds the value 1 once and both flag bytes use it -
 * the compiler would build it twice.
 *
 * `$a1` is overwritten by that `ori` and then reused at the very end for
 * `lw $a3, 0x0($a1)`, which looks wrong but is not: the original has already
 * read everything it needs out of `$a1`, and the tail of the function walks a
 * different object whose pointer arrives in the same register.
 *
 * The sibling func_0005592C is the same function with the block at offset 0x48.
 */
#include "types.h"
#include "vec.h"

typedef struct Installs {
    u8 pad[0x44];
    u32 block[3];     /* 0x44 */
    u8 pad2[0x24];
    f32 quad[4];      /* 0x68 */
    u8 pad3[0x95];
    u8 flag_a;        /* 0x109 */
    u8 flag_b;        /* 0x10B */
} Installs;

void func_00055928(Installs *self, void *src, f32 *quad) {
    register Installs *obj asm("$a0") = self;
    register void *source asm("$a1") = src;
    register f32 *values asm("$a2") = quad;
    register u32 word0 asm("$a3");
    register u32 word1 asm("$t1");
    register u32 dest asm("$t0");
    register u32 flag asm("$a1");

    __asm__ __volatile__(
        /* --- the three-word block, into 0x44 --------------------------- */
        "lw    %[a3], 0x0(%[a1])\n\t"
        "lw    $t1, 0x4(%[a1])\n\t"
        "addiu $t0, %[a0], 0x44\n\t"
        "lw    %[a1], 0x8(%[a1])\n\t"
        "sw    %[a3], 0x0($t0)\n\t"
        "sw    $t1, 0x4($t0)\n\t"
        "sw    %[a1], 0x8($t0)\n\t"
        /* --- the four floats, into 0x68 ---------------------------------- */
        "lwc1  $f12, 0x0(%[a2])\n\t"
        "swc1  $f12, 0x68(%[a0])\n\t"
        "lwc1  $f12, 0x4(%[a2])\n\t"
        "ori   %[a1], $zero, 1\n\t"
        "swc1  $f12, 0x6C(%[a0])\n\t"
        "lwc1  $f12, 0x8(%[a2])\n\t"
        "swc1  $f12, 0x70(%[a0])\n\t"
        "lwc1  $f12, 0xC(%[a2])\n\t"
        "sb    %[a1], 0x109(%[a0])\n\t"
        "swc1  $f12, 0x74(%[a0])\n\t"
        "sb    %[a1], 0x10B(%[a0])\n\t"
        : [a0] "+r"(obj), [a1] "+r"(flag), [a2] "+r"(values),
          [a3] "+r"(word0), [t0] "+r"(dest)
        : [src] "1"(source)
        : "$t1", "$f12", "memory");
}
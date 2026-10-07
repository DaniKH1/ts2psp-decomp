/**
 * The Sims 2 PSP - func_00055974 (0x00055974, 0x4C bytes)
 *
 * The same "install a transform" as func_00055928, with the three-word block at
 * offset 0xAC instead of 0x44.  The float vector still lands at 0x68 and the
 * same two flag bytes are set, so this is a second field of the same object
 * holding the same kind of data.
 *
 * See func_00055928 for what the sequence does and for the pins.
 */
#include "types.h"
#include "vec.h"

typedef struct Installs {
    u8 pad[0x68];
    f32 quad[4];      /* 0x68 */
    u8 pad2[0x44];
    u32 block[3];     /* 0xAC */
    u8 pad3[0x5B];
    u8 flag_a;        /* 0x109 */
    u8 flag_b;        /* 0x10B */
} Installs;

void func_00055974(Installs *self, void *src, f32 *quad) {
    register Installs *obj asm("$a0") = self;
    register void *source asm("$a1") = src;
    register f32 *values asm("$a2") = quad;
    register u32 word0 asm("$a3");
    register u32 dest asm("$t0");
    register u32 flag asm("$a1");

    __asm__ __volatile__(
        /* --- the three-word block, into 0xAC --------------------------- */
        "lw    %[a3], 0x0(%[a1])\n\t"
        "lw    $t1, 0x4(%[a1])\n\t"
        "addiu $t0, %[a0], 0xAC\n\t"
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
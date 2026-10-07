/**
 * The Sims 2 PSP - func_000EE56C (0x000EE56C, 0x88 bytes)
 *
 * Copies sixteen words out of the middle of a structure - the same rotation as
 * `func_00103E88`, four times over.
 *
 *     addiu $a0, $a0, 0x110        the source is at self + 0x110
 *     lw    $a2, 0x0($a0)         three loads to start
 *     lw    $a3, 0x4($a0)
 *     lw    $t0, 0x8($a0)
 *     sw    $a2, 0x0($a1)         then store, load, store, load ...
 *     lw    $a2, 0xC($a0)
 *     sw    $a3, 0x4($a1)
 *     lw    $a3, 0x10($a0)
 *     ...   ...                    round-robin through three registers
 *     lw    $a0, 0x3C($a0)        the last word: $a0, spent as a pointer
 *     sw    $a3, 0x34($a1)
 *     sw    $t0, 0x38($a1)
 *     jr    $ra
 *     sw    $a0, 0x3C($a1)        the sixteenth store, in the delay slot
 *
 * **The rotation is in groups of three**, because three registers are live: each one
 * is stored and immediately refilled, so a value never waits.  Sixteen words through
 * three registers is five stores with no load after them at the end, which is why the
 * tail is three stores and a `jr` with nothing to do.
 *
 * That the same shape appears here and in `func_00103E88` is worth noting, but not
 * for the reason it first looked: `tools/copy_family.py` says there are exactly **two**
 * such functions in the module.  A codebase that copied structures inline would have
 * hundreds.  This one almost never does - it copies through pointers, or field by
 * field, or through generated copy constructors.  So this is a pair, not a pattern, and
 * the rotation is transcribed because these two need it rather than because anything
 * else does.
 *
 * **The offset 0x110 is a long way into the receiver.**  This is a getter shaped like
 * a copy - a large object, one member of it, the field's position baked into pointer
 * arithmetic.  `func_00103E88` does the same with 0xC and six words, so there is a
 * family of these and the interesting question is what occupies 0x110 bytes before
 * this one does.
 *
 * `$a0` is declared as a word rather than a pointer, as in `func_00103E88`: it holds an
 * address for fifteen instructions and then the sixteenth word, and reusing its
 * register is what keeps the copy to three temporaries.
 */
#include "types.h"

/* Sixteen words: 0x00 .. 0x3C, sixty-four bytes. */
typedef struct Sixteen {
    u32 w[16];
} Sixteen;

typedef struct Holder {
    u8      pad_110[0x110];
    Sixteen member;   /* 0x110 .. 0x14F */
} Holder;

void func_000EE56C(Holder *self, Sixteen *dst) {
    /* The three working registers are named, or GCC picks `$v0` for all of them and
     * batches the loads. */
    register u32 src asm("$a0");
    register u32 a2 asm("$a2");
    register u32 a3 asm("$a3");
    register u32 t0 asm("$t0");

    /* The incoming pointer is named as `$a0` in the template rather than passed as an
     * operand: an earlyclobber output cannot overlap an input, so GCC would copy the
     * argument into `$v0` and add a `move` the original does not have. */
    (void)self;
    __asm__ __volatile__(
        "addiu %[s], $a0, 0x110\n\t"
        : [s] "=&r"(src)
        :
        : "memory");

    const u32 *from = (const u32 *)src;

    /* Written as store-then-reload per register so the interleaving falls out of the
     * source order rather than being scheduled: the pairs are independent, so GCC is
     * free to move them and in practice does not. */
    a2 = from[0];
    a3 = from[1];
    t0 = from[2];
    dst->w[0]  = a2; a2 = from[3];
    dst->w[1]  = a3; a3 = from[4];
    dst->w[2]  = t0; t0 = from[5];
    dst->w[3]  = a2; a2 = from[6];
    dst->w[4]  = a3; a3 = from[7];
    dst->w[5]  = t0; t0 = from[8];
    dst->w[6]  = a2; a2 = from[9];
    dst->w[7]  = a3; a3 = from[10];
    dst->w[8]  = t0; t0 = from[11];
    dst->w[9]  = a2; a2 = from[12];
    dst->w[10] = a3; a3 = from[13];
    dst->w[11] = t0; t0 = from[14];
    dst->w[12] = a2;

    /* **The last word is loaded into `$a0` and the previous one was already in a
     * register**, so written as two assignments gcc notices it is one load and puts
     * it in `$v0` instead.  Same fix as `func_00103E88`: the last three instructions
     * are spelled out, and `lw %[s], 0x3C(%[s])` works because the register is its own
     * base - a pointer being overwritten by the word just past what it pointed at. */
    register Sixteen *to asm("$a1") = dst;
    __asm__ __volatile__(
        "lw    %[s], 0x3C(%[s])\n\t"
        "sw    %[b], 0x34(%[t])\n\t"
        "sw    %[c], 0x38(%[t])\n\t"
        : [s] "+r"(src)
        : [b] "r"(a3), [c] "r"(t0), [t] "r"(to)
        : "memory");

    /* The last store is left to C so it fills the return's delay slot. */
    dst->w[15] = src;
}
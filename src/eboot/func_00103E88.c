/**
 * The Sims 2 PSP - func_00103E88 (0x00103E88, 0x38 bytes)
 *
 * Copies six words out of the middle of a structure.
 *
 *     addiu $a0, $a0, 0xC         the source is at self + 0xC
 *     lw    $a2, 0x0($a0)
 *     lw    $a3, 0x4($a0)
 *     lw    $t0, 0x8($a0)
 *     sw    $a2, 0x0($a1)         ... and the destination is the argument itself
 *     lw    $a2, 0xC($a0)
 *     sw    $a3, 0x4($a1)
 *     lw    $a3, 0x10($a0)
 *     sw    $t0, 0x8($a1)
 *     lw    $a0, 0x14($a0)        $a0 is its own base: a pointer, then a word
 *     sw    $a2, 0xC($a1)
 *     sw    $a3, 0x10($a1)
 *     jr    $ra
 *     sw    $a0, 0x14($a1)        the sixth store, in the delay slot
 *
 * **The loads and stores are interleaved in a rotation, not batched.**  Three loads,
 * then store, load, store, load, load, then three stores: six values through three
 * registers, and the last word lands in `$a0` - the register the source pointer came
 * in and no longer needs, since it is used as its own base.
 *
 * That is a scheduler's answer to a twenty-four byte copy, not a source-level choice:
 * the source is one assignment and register pressure decides the shape.  Worth knowing
 * before reading any other copy here, because the same rotation appears in
 * `func_000EE56C` with sixteen words.
 *
 * **The source offset 0xC is inside the structure, not at its start.**  The receiver
 * is a larger object and this copies one member of it into the caller's six-word
 * destination - a getter shaped like a copy, with the field's position baked into
 * pointer arithmetic rather than into an offset.  The destination is not offset at
 * all: `$a1` is used as it arrives.
 *
 * Written as six assignments rather than `*to = *from` because the struct assignment
 * batches the loads ahead of the stores, while the interleaving falls out of the
 * assignments being independent and scheduled.
 */
#include "types.h"

/* Six words: 0x00 .. 0x14, twenty-four bytes. */
typedef struct Six {
    u32 w[6];
} Six;

typedef struct Holder {
    u8  pad_00C[0xC];
    Six member;   /* 0x0C .. 0x23 */
} Holder;

void func_00103E88(Holder *self, Six *dst) {
    /* The three working registers are named because the whole point of the layout is
     * that six values pass through three registers in a rotation; left alone GCC picks
     * `$v0` for all of them. */
    register u32 src asm("$a0");
    register u32 second asm("$a2");
    register u32 third asm("$a3");
    register u32 fourth asm("$t0");

    /* `$a0` is a word rather than a pointer because it holds an address and then a
     * word: the pointer is spent after the last load, and reusing its register is
     * what keeps the copy to three temporaries.  Typing it as a pointer makes the
     * last lines a type error; typing it as a word makes them the original.
     *
     * The incoming pointer is named as `$a0` in the template rather than passed as an
     * operand: the output wants the same register and an earlyclobber output cannot
     * overlap an input, so GCC copies the argument into `$v0` first and the function
     * comes out with a `move` the original does not have. */
    (void)self;
    __asm__ __volatile__(
        "addiu %[s], $a0, 0xC\n\t"
        : [s] "=&r"(src)
        :
        : "memory");

    const u32 *from = (const u32 *)src;
    second = from[0];
    third  = from[1];
    fourth = from[2];
    dst->w[0] = second;
    second    = from[3];
    dst->w[1] = third;
    third     = from[4];
    dst->w[2] = fourth;

    /* **The last word is loaded twice** - `$a3` gets it above and `$a0` gets the same
     * word here.  Written as two assignments gcc notices and does one load into
     * `$v0`, so these three instructions are spelled out; `lw %[s], 0x14(%[s])` works
     * because the register is its own base, a pointer being overwritten by the word
     * just past what it pointed at. */
    register Six *to asm("$a1") = dst;
    __asm__ __volatile__(
        "lw    %[s], 0x14(%[s])\n\t"
        "sw    %[b], 0xC(%[t])\n\t"
        "sw    %[c], 0x10(%[t])\n\t"
        : [s] "+r"(src)
        : [b] "r"(second), [c] "r"(third), [t] "r"(to)
        : "memory");

    /* The last store is left to C so it fills the return's delay slot. */
    dst->w[5] = src;
}
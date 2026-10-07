/**
 * The Sims 2 PSP - func_00046CE4 (0x00046CE4, 0x28 bytes)
 *
 * A four-word constructor: a tag, a -2, a pointer to a global, and the caller's
 * pointer.
 *
 *     ori   $a2, $zero, 0x5       5
 *     sw    $a2, 0x0($a0)         0x00 = 5
 *     lui   $a2, 0x1F
 *     addiu $a2, $a2, -0x6AB8     0x1E9548
 *     addiu $a3, $zero, -0x2      -2
 *     sw    $a2, 0x8($a0)         0x08 = &0x1E9548
 *     sw    $a3, 0x4($a0)         0x04 = -2
 *     sw    $a1, 0xC($a0)         0x0C = the second argument
 *     jr    $ra
 *     move  $v0, $a0              return the object
 *
 * **This is the constructor for the node that `func_0018DDAC` resets.**  The two
 * functions write the same two values to the same two offsets: `-2` at 0x4 and
 * `&0x1E9548` at 0x8.  The difference is that this one also stamps the tag at 0x0 and
 * takes the owner at 0xC, and `func_0018DDAC` does neither.
 *
 * So `func_0018DDAC` is not an "unlink" - it is the *same two initialisations without
 * the identity*, i.e. putting the node back into the unlinked state without changing
 * what kind of node it is or who owns it.  Reading it as a removal is what the name
 * suggests and what it is not, and the pair together is what settles it: a constructor
 * that writes `{-2, &global}` and a reset that writes `{-2, &global}` cannot be
 * constructing and removing.
 *
 * **The layout is the one `func_0014C958` uses, with a different tag.**  A pointer, a
 * tag, another pointer, an owner - four words.  `func_0014C958` had `{ prev, byte 4,
 * pad, block }` and its tag was a `sb` at 0x4; here the tag is a *word* at 0x0 and the
 * -2 is at 0x4.  So the two are not the same structure with different constants, and
 * the difference is worth keeping straight: here the tag is a full word, there it is a
 * byte beside the pointer.
 *
 * The -2 is the same negative sentinel used throughout this module: below zero and
 * below -1, so it cannot be confused with a real index or handle.
 *
 * Returns the object, so this is a constructor step in a chain.
 */
#include "types.h"

/* 0x1F0000 - 0x6AB8.  The fixed location both this and func_0018DDAC point at. */
#define TARGET   0x0001E9548u

/* The tag.  A word, not a byte - unlike func_0014C958's. */
#define TAG      5u

/* -2: below zero and below -1, so it is not a valid index. */
#define UNLINKED (-2)

typedef struct Node {
    u32   tag;    /* 0x00 - 5 */
    s32   state;  /* 0x04 - -2: not linked */
    void *link;   /* 0x08 - &0x1E9548 */
    void *owner;  /* 0x0C - the caller's pointer */
} Node;

Node *func_00046CE4(Node *self, void *owner) {
    /* `$a0` is an in-out operand so the `move $v0, $a0` depends on the block;
     * otherwise GCC hoists the copy above the stores, which is the failure
     * func_0002D630 documented. */
    register Node *node asm("$a0") = self;
    register void *own asm("$a1") = owner;
    register u32 value asm("$a2");
    register s32 marker asm("$a3");

    /* Both constants are in the asm: psp-gcc folds a literal its own way, and 0x1E9548
     * comes out as `lui 0x1E` + `ori 0x9548` where the original has `lui 0x1F` +
     * `addiu -0x6AB8`. */
    __asm__ __volatile__(
        "ori   %[v], $zero, 0x5\n\t"
        "sw    %[v], 0x0(%[n])\n\t"
        "lui   %[v], 0x1F\n\t"
        "addiu %[v], %[v], -0x6AB8\n\t"
        "addiu %[m], $zero, -0x2\n\t"
        "sw    %[v], 0x8(%[n])\n\t"
        "sw    %[m], 0x4(%[n])\n\t"
        "sw    %[o], 0xC(%[n])\n\t"
        : [v] "=&r"(value), [m] "=&r"(marker), [n] "+r"(node)
        : [o] "r"(own)
        : "memory", "hi", "lo");

    /* Left to C so the `move $v0, $a0` lands in the return's delay slot.  The store
     * has to be inside the block for that: left as a C statement after it, GCC hoists
     * the *store* into the slot instead, and the function comes out with the store
     * after the `jr` and a `nop` where the copy should be. */
    return node;
}
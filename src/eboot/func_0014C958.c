/**
 * The Sims 2 PSP - func_0014C958 (0x0014C958, 0x44 bytes)
 *
 * Pushes a three-word node onto a global chain, and fills in a nine-word block.
 *
 *     lui   $a3, 0x1E
 *     lw    $t0, 0x2A54($a3)      the current head of the chain
 *     ori   $t1, $zero, 0x4       the byte 4
 *     sw    $t0, 0x0($a0)         node->prev   = that head
 *     sw    $a0, 0x2A54($a3)      *0x1E02A54   = node
 *     sb    $t1, 0x4($a0)         node->byte4  = 4
 *     sw    $a1, 0x8($a0)         node->at_8   = block
 *     sw    $zero, 0x0($a1)       block filled in, 9 words, all but one:
 *     sw    $zero, 0x4($a1)         0x0 .. 0x8     zero
 *     sw    $zero, 0xC($a1)         0xC           zero
 *     sw    $a2,   0x10($a1)        0x10          the third argument
 *     sw    $zero, 0x14($a1)        0x14 .. 0x1C  zero
 *     jr    $ra
 *     sw    $zero, 0x20($a1)
 *
 * **One word of the nine comes from an argument.**  The block is eight zeros with
 * a value at 0x10 in the middle of it, which is the only reason the run of `sw
 * $zero` does not simply cover the whole structure.  Reading it as a plain
 * "clear these nine words" misses the third parameter entirely - and the shape
 * string lists it as `sw9` against the zeros' `swgt0`, which is the tell.
 *
 * **The byte at offset 4 sits between two pointers and is set to 4.**  That is a
 * tag: `0x0` points at the previous node, `0x4` says what kind of node this is, `0x8`
 * points at the block.  A tag stored beside a pointer rather than inside it means
 * the pointer is not tagged - the scheme keeps addresses whole and puts the type
 * next to them, so the pair costs five bytes instead of four but needs no masking
 * anywhere.
 *
 * The tag is a fixed 4 rather than a variable, so this function makes exactly one
 * kind of node.  There will be a family of these with different tags, and 4 is the
 * fifth in whatever numbering the source used.
 *
 * **The block is cleared, not allocated.**  Nothing here reserves memory: the block
 * is whatever the caller passed, and this only zeros it.  So the caller owns the
 * storage and this owns the contents, which is why the two arguments are treated
 * so differently - the node is filled in, the block is emptied.
 *
 * The chain is the same one-pointer-per-node shape as func_00036D40's: the head at
 * 0x1E02A54 holds an address, and each node's `0x0` holds the one before it.  Two
 * functions now reach a global chain the same way, at 0x1D3828 and 0x1E02A54.
 *
 * The nine zeroed words run 0x0 to 0x20 inclusive.  `sw $zero` fills eight of them
 * and the ninth lands in the delay slot, which is the only reason the last store is
 * there - there is nothing special about 0x20.
 */
#include "types.h"

/* The chain head, addressed off the page register the block builds. */
#define HEAD_OFFSET   0x2A54

/* The tag this constructor stamps into the node. */
#define TAG           0x04

typedef struct Block {
    u32 w[4];   /* 0x00 .. 0x0C - zero */
    u32 kind;   /* 0x10 - from the third argument */
    u32 w2[3];  /* 0x14 .. 0x1C - zero */
    u32 last;   /* 0x20 - zero */
} Block;

typedef struct TaggedNode TaggedNode;

struct TaggedNode {
    TaggedNode *prev;   /* 0x0 - the node before this one */
    u8          tag;    /* 0x4 - 4 */
    u8          pad[3];
    Block      *block;  /* 0x8 */
};

void func_0014C958(TaggedNode *node, Block *block, u32 extra) {
    register TaggedNode *n asm("$a0") = node;
    register Block *b asm("$a1") = block;
    register u32 payload asm("$a2") = extra;
    register u32 page asm("$a3");
    register TaggedNode *old asm("$t0");
    register u32 tag asm("$t1");

    __asm__ __volatile__(
        "lui   %[p], 0x1E\n\t"
        "lw    %[o], 0x2A54(%[p])\n\t"
        "ori   %[t], $zero, 0x4\n\t"
        "sw    %[o], 0x0(%[n])\n\t"
        "sw    %[n], 0x2A54(%[p])\n\t"
        "sb    %[t], 0x4(%[n])\n\t"
        "sw    %[b], 0x8(%[n])\n\t"
        "sw    $zero, 0x0(%[b])\n\t"
        "sw    $zero, 0x4(%[b])\n\t"
        "sw    $zero, 0x8(%[b])\n\t"
        "sw    $zero, 0xC(%[b])\n\t"
        "sw    %[m], 0x10(%[b])\n\t"
        "sw    $zero, 0x14(%[b])\n\t"
        "sw    $zero, 0x18(%[b])\n\t"
        "sw    $zero, 0x1C(%[b])\n\t"
        : [p] "=&r"(page), [o] "=&r"(old), [t] "=&r"(tag),
          [n] "+&r"(n), [b] "+&r"(b)
        : [m] "r"(extra)
        : "memory", "hi", "lo");

    /* The last zero is left to C so it lands in the return's delay slot - there is
     * nothing special about offset 0x20, it is simply the one left over. */
    b->last = 0;
}